/////////////////////////////////////////////////////////////////////////////////////
//File Name: main_HTPA.c
//Processor: PIC24FJ64GA104
//Compiler: MPLAB C30
//Linker: MPLAB LINK30
//Company: Hema Imaging

//
//Author               Date            Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//Mehrdad Ramazanali   11-04=2013
// Erik Beall          10-25-2014
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/////////////////////////////////////////////////////////////////////////////////////

#include "main_HTPA.h"

/////////////////////////Cofiguration//////////////////
// 
  
_CONFIG1( FWDTEN_OFF & ICS_PGx2 & JTAGEN_OFF)    
_CONFIG2(FNOSC_PRIPLL & POSCMOD_XT & IESO_OFF & IOL1WAY_ON  & OSCIOFNC_ON ) // Primary XT OSC with PLL
//_CONFIG2( FNOSC_FRCPLL  & IESO_OFF & IOL1WAY_ON  & OSCIOFNC_ON   & POSCMOD_NONE  )
 _CONFIG3( SOSCSEL_IO  )     

//#Defines
#define OFF 0
#define _1MHZ_DUTY 9
#define _1MHZ_PERIOD 18 // 18 divided by 16 = 1.125usec per HTPA clock, or 889kHz, 225usec per frame, 118.8msec per image, or 8.42fps
#define _UART_INIT 16  // (34=115,200bps, 16=230,400bps), also change MCLK for HTPA to 9,18 for 230kbps (9fps)
//variables
unsigned char ResetCounter;
unsigned char adc_array[4];
unsigned char TxPacket_Stream[101];
unsigned char TxPacket_Data[101];
unsigned char TxPacket[101];
unsigned int ButtonCounter;
unsigned int ButtonTimer;
unsigned int LED1BlinkingCounter;
unsigned int SleepCounter;
unsigned int LaserCounter;
//unsigned int ByteStuffingCounter;
//unsigned int TwelveBitModeCounter;
//unsigned int StopTimer;

//int __attribute__((space(prog),aligned(_FLASH_PAGE*2))) dat[_FLASH_PAGE];

STATE_MACHINE   state;       		// Global state machine variable
//*****************************************************************************
// Structure: FLAGS _system_flags
//
// Overview: the structure provides access to system flags (bit fields)
//
//****************************************************************************/
FLAGS _system_flags;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     FLAGInit
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Initialize FLAG bit fields
//Note:         
//~~~~~~~~~
void FLAGInit(void)
{
	_system_flags.PacketisSentflag = 0;
	_system_flags.PacketisReadyforParse =0;//ready to parse
	_system_flags.PacketisReadytoSend = 0;
	_system_flags.EndofFrameflag = 0;
	_system_flags.SW1released = 0;
	_system_flags.Streamflag = 0;
	_system_flags.Standbyflag = 0;
	_system_flags.Sleepflag = 0;
	_system_flags.SendByteStuffingflag = 0;
	_system_flags.TwelveBitflag = 0;
	_system_flags.SixteenBitflag = 0;
	_system_flags.XORflag = 0;
	_system_flags.Stopflag =0;
	_system_flags.TurnLaserONflag = 0;
	_system_flags.RefreshStatusflag = 0;
	_system_flags.PacketisBeingReceived=0;
	_system_flags.ChangeUARTSpeed=0;
	_system_flags.StopHTPAflag=0;
	_system_flags.InSleepWake=0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     SYSTEMInit
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Initialize system variables
//Note:         
//~~~~~~~~~
void SYSTEMInit(void)
{
	state = STATE_STANDBY;
	ResetCounter = 0;
	ButtonCounter = 0;
	LED1BlinkingCounter = 0;
	SleepCounter = SLEEPTIME;//180
	FrameNumber = 0;
	PixelCounter = 0;
	ChunkCounter = 0;
}

int main (void)
{
//	_DOZE = 0;
   	_RCDIV = 0;
   	//pin mapping
    PinMapping();
    //Initialize the outputs
	OutputInit();
	// Set up the timer
	TimerInit();
	_system_flags.uart_hi_speed = 1;
	if (_system_flags.uart_hi_speed) {
		// divisor=8 for 460800bps
		initUART(8);
	} else {
    	initUART(34);
    }
	//Initialize the system
	SYSTEMInit();	
	ADCinit();	
	//Initialize the button processes	
	BtnInit();
	//Initialize the system flags
	FLAGInit();
	// variable to hold number of times we've passed the 50 msec counter without completing a UART receive
	char receivedCtr=0;

	while (1)
	{
		// check continuity to PIC
		if(uartDataAvailable_receive )
		{
			// set a flag if we're inside a receive packet
			// then, in 50msec timer, if we've not received anything after ~150-200msec, 
			// clear it and tell uart_getPacket to reset state
			_system_flags.PacketisBeingReceived=1;
	        uart_getPacket(0);
            uartDataAvailable_receive = 0;
		}
		if(_system_flags.TwelveBitflag)
		{
			if(_system_flags.EndofFrameflag)
			{
				// if stopped, and all data already pushed out, turn off HTPA
				if(_system_flags.Stopflag)
				{
					while (TX_Sent_Counter!=TX_ToSend_Counter);
					TurnOffHTPA();
					IEC0bits.U1TXIE = 1; // re-enable UART TX ISR
					_system_flags.Standbyflag=1;
					_system_flags.RefreshStatusflag = 1;
					_system_flags.Stopflag = 0;
				}
				_system_flags.EndofFrameflag = 0;
			}
		}
		if (ms_flag)// 1ms loop
		{	
			BtnProcessEvents();  	// Button debounce processing
			StateMachine();	
			if((_button_press.SW1DBB)&&(!_system_flags.SW1released))
			{
				SleepCounter = SLEEPTIME;
				LaserOutlat = ~LaserOutlat;
				LaserCounter=10;
				ButtonTimer = 3;
				_system_flags.SW1released = 1;
			}
			if( _system_flags.PacketisReadyforParse)
			{
				_system_flags.PacketisBeingReceived=0;
				receivedCtr=0;
				_system_flags.PacketisReadyforParse = 0;
				ParsingRXMsg();	
			}    
			if (TimeBase50ms())   //increment and check for 50ms software timer
			{
				if (_system_flags.PacketisBeingReceived)
				{
					// increment counter
					receivedCtr++;
					if (receivedCtr>2)
					{
						receivedCtr=0;
						// reset uart_getPacket
						_system_flags.PacketisBeingReceived=0;
						uart_getPacket(1);
					}
				}
			}//end of 50ms interval
    		if (TimeBase100ms())   //increment and check for 100ms software timer
    		{
    			// if we're in standby, and the change UART flag has been set, change it
    			if (state==STATE_STANDBY)
    			{
	    			if (_system_flags.ChangeUARTSpeed)
	          		{
		          		if (_system_flags.PacketisBeingReceived)
				    	{
				    		uart_getPacket(1);
				    	}
						_system_flags.ChangeUARTSpeed=0;
						// simply toggle UART speed if we get this command
						if (_system_flags.uart_hi_speed==1) {
            				_system_flags.uart_hi_speed=0;
            				initUART(34);
        				} else {
            				_system_flags.uart_hi_speed=1;
            				initUART(8);
        				}
        				FLAGInit();
        				SYSTEMInit();
        			}
       			}
			}//end of 100ms
			if (TimeBase250ms())   //increment and check for 250ms software timer   
			{
				LED1BlinkingCounter++;
				if(LED1BlinkingCounter == 1)
				{
					LED2lat = 1;//green off
					LED1lat = 1; // red is off
				}
				// blink at double speed if in high speed mode
				if(LED1BlinkingCounter > (10-(5*_system_flags.uart_hi_speed))) {
					if(state ==STATE_STREAM )
					{
						LED2lat = 0; // green flashing during streaming
						LED1lat = 1; // red is off
					}
					if(state ==STATE_STANDBY )
					{
						LED1lat = 0; // red flashing during standby
						LED2lat = 1; // green is off	
					}
					LED1BlinkingCounter = 0;
				}
				if((!_button_press.SW1DBB)&&(_system_flags.SW1released))
				{
					_system_flags.SW1released = 0;
					SleepCounter = SLEEPTIME;
				}
				if((ButtonTimer==0)&&(_system_flags.SW1released)&&(!_system_flags.InSleepWake))
				{
					if((_button_press.SW1DBB))
					{
						if(state==STATE_STREAM)
						{
							TurnOffHTPA();
						}	
						// nuclear option, reset firmware and start over with clean plate. 
						asm ("reset");
					}
				}
			}
			if (TimeBase1s())   //increment and check for 1000ms software timer
			{
				if (LaserCounter!=0) LaserCounter--;
				if (LaserCounter==0)
				{
					LaserOutlat = 0;
    			}    
				if(ButtonTimer>0) 
				{
					ButtonTimer=ButtonTimer-1;
				}
				_system_flags.InSleepWake=0;
				if(state == STATE_STANDBY)
				{
					if(SleepCounter>0) 
					{
						SleepCounter=SleepCounter-1;
					}
					if((SleepCounter==0))
					{
						// sleep flag will call state=SLEEP above
						_system_flags.Sleepflag = 1;
						_system_flags.RefreshStatusflag=1;
					}
				}//stand by mode
			}//End of TimeBase1s
			if (TimeBase1h())
			{				
			}//end of 1 hour base	
			ms_flag = 0;
		}// End of msflag 1 ms
	}// End of while (1)
} //End of main()

void StateMachine(void)
{
	// first section to handle state changes
	switch (state)
	{
		case(STATE_STANDBY):
		{
			if((_system_flags.Streamflag)&&(_system_flags.RefreshStatusflag))
			{
				state = STATE_STREAM;
				_system_flags.RefreshStatusflag = 0;
			}
			if((_system_flags.Sleepflag)&&(_system_flags.RefreshStatusflag))
			{
				state = STATE_SLEEP;
				_system_flags.RefreshStatusflag = 0;
			}
			break;
		}
		case (STATE_STREAM):
		{
			if((_system_flags.Standbyflag)&&(_system_flags.RefreshStatusflag))
			{
				state = STATE_STANDBY;
				_system_flags.RefreshStatusflag = 0;
			}
			break;
		}
		case (STATE_SLEEP):
		{
			if((_system_flags.Standbyflag)&&(_system_flags.RefreshStatusflag))
			{
				state = STATE_STANDBY;
				_system_flags.RefreshStatusflag = 0;
			}
			break;
		}
	}
	// second section to actually set the flags appropriate for current state
	if(!_system_flags.RefreshStatusflag)
	{
	    switch (state)
    	{
        	case STATE_STANDBY:
                _system_flags.Sleepflag = 0;
                _system_flags.Standbyflag = 1;
                _system_flags.Streamflag = 0;
                _system_flags.TwelveBitflag = 0;
                TurnOffHTPA();
                SleepCounter = SLEEPTIME;
            	break;
        	case STATE_STREAM:
                _system_flags.TwelveBitflag = 1;
                if(_system_flags.TwelveBitflag) {
                    //  LED2lat = 0;//green
                    //uart_sendHeader(528* 3, STREAM_REQUEST);
                    uart_sendHeader(2048* 3, STREAM_REQUEST);
                }
                if(_system_flags.SixteenBitflag) {
                    uart_sendHeader(2048* 4, STREAM_REQUEST);
                    //uart_sendHeader(528* 4, STREAM_REQUEST);
                }
                // block until header is sent
                while(!U1STAbits.TRMT);
                // then turn off UART TX interrupt
                IEC0bits.U1TXIE = 0;
                TurnOnHTPA();
                _system_flags.EndofFrameflag = 0;
                _system_flags.Standbyflag = 0;
                _system_flags.Sleepflag = 0;
                _system_flags.Streamflag = 1;
            	break;
        	case STATE_SLEEP:
        		// turn off both LEDs
                LED2lat = 1;
                LED1lat = 1;
                // put RN42 into ultra-low power
                SleepCounter = SLEEPTIME;
                T4CONbits.TON = 0;
                T5CONbits.TON = 0;
                T1CONbits.TON = 0;
                IEC0bits.T1IE = 0; 	// disable TMR1 interrupt
                IFS1bits.INT1IF = 0;
                IEC0bits.U1TXIE = 0; // disable uart TX
                IEC0bits.U1RXIE = 0; // and RX ISRs
                // clear other flags
                IFS0bits.U1RXIF = 0;
                IFS0bits.U1TXIF = 0;
                IFS0bits.IC1IF = 0;
                IFS0bits.OC1IF = 0;
                _SPI1IF = 0;
                _SPI2IF = 0;
                // next line sets interrupt on positive edge for button press
                INTCON2bits.INT1EP = 0;
                IPC5bits.INT1IP = 1;
                IEC1bits.INT1IE = 1;
                _system_flags.InSleepWake=1;
                // only interrupt set is button, now go to sleep
                Sleep(); 
                // on wakeup, resume and disable button interrupt
                IEC1bits.INT1IE = 0;
                IEC0bits.T1IE = 1; // re-enable timer 1 interrupt
                T1CONbits.TON = 1; // re-enable main timer
                IEC0bits.U1TXIE = 1; // enable uart TX
                IEC0bits.U1RXIE = 1; // and RX ISRs
                LED2lat = 0;
                _system_flags.Sleepflag = 0;
                _system_flags.Standbyflag = 1;
                _system_flags.Streamflag = 0;
                ButtonTimer=3;
            	break;
     	}//end of switch
     	_system_flags.RefreshStatusflag = 1;
    }// end of refresh flag
}//end of state machine


void TurnOnHTPA(void)
{
	PWMinit();
	_5V_ENlat = 1;
	Delay10us(15);
	InputCapInit();
	ADCInit(); // enables SPI2 interrupt
	HTPAInit(); // enables SPI1 interrupt and Timer 5 init
    // set delay for analog signal to settle and low-pass filter cutoff
	if (_system_flags.uart_hi_speed) {
        PR5 = 60;
        PR4 = 1080;
        PWMGenerate(4,8);
    } else {
		PR4 = 4250;
		PR5 = 160;
        PWMGenerate(16,32);
    }
	HTPA_CONT_lat = 1;
	HTPA_counter = 0;
	_system_flags.Stopflag =0;
	_system_flags.StopHTPAflag=0;
}

void TurnOffHTPA(void)
{
	// turn off 5V enable to boost regulator
	_5V_ENlat = 0;
	HTPA_CONT_lat = 0;
	// turn off the VALID and UART-stream-mode timers
	T4CONbits.TON = 0;
	// wait until transmit completed on StreamBuffer
	while (TX_Sent_Counter!=TX_ToSend_Counter) ;
	T5CONbits.TON = 0;
	// switch off the MCLK to the HTPA
	PWMGenerate(OFF,OFF);
	ADC_CS_lat = 0;
	IEC0bits.IC1IE = 0; // disable IC1 interrupts
	SPI1STATbits.SPIEN = 0;
	SPI2STATbits.SPIEN = 0;
	PixelCounter = 0;
	_system_flags.Standbyflag = 1;
	// turn off the HTPA SPI ISR
	_SPI1IF = 0;
	_SPI1IE = 0;
	// turn off the ADC SPI ISR
	_SPI2IF = 0;
	_SPI2IE = 0;
	SleepCounter = SLEEPTIME;
 	LED1lat = 1;
 	LED2lat = 1;//green
 	ButtonCounter = 0;
 	_system_flags.TwelveBitflag = 0;
	_system_flags.Stopflag =0;
	FrameCounter = 0;		
}

