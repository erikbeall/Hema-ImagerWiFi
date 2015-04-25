/////////////////////////////////////////////////////////////////////////////////////
//File Name: SPI_HTPA.c
//Processor: PIC24FJ64GA104
//Compiler: MPLAB C30
//Linker: MPLAB LINK30
//Company: Hema Imaging

//
//Author               Date            Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//Mehrdad Ramazanali   11-04=2013
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/////////////////////////////////////////////////////////////////////////////////////
#include "main_HTPA.h"
#include "SPI_HTPA.h"
//#include "globals.h"

//Global Variables
unsigned int FrameCounter = 0;
unsigned int PixelCounter = 0;
unsigned char whatsnext = 0;
unsigned int HTPA_counter = 0;
unsigned int ADCCounter = 0;
unsigned int HTPA_BUF1;
unsigned int HTPA_BUF2;
unsigned int Empty_Buf;
unsigned char FrameNumber;
unsigned char StreamBuffer[101];
unsigned int HTPAErrorCtr=0;

void ADCInit(void)
{
	// Initialize SPI
    SPI2STAT = 0;
    SPI2CON1 = 0b0000000000111010; //0000000000011010
    SPI2CON2 = 0;
    // interrupt when transfer has finished, because we are master, and all transfers are synchronous
        
    SPI2CON1bits.MODE16 = 0;
    
    // we should not need to do any of this, just enable the interrupts
    SPI2CON1bits.CKE = 0;
    SPI2CON1bits.CKP = 1;
    SPI2CON1bits.SMP = 1;
    _SPI2IF = 0;
    _SPI2IE = 1;
    _SPI2IP = 6; // high priority for ADC reading SPI interrupts
    SPI2CON1bits.MSTEN = 1; // set the above settings and enable the module in master mode
    SPI2STATbits.SPIEN = 1;
}

void HTPAInit(void)
{
	// Initialize SPI
	SPI1BUF = 0x00;
	SPI1STAT = 0;
    SPI1CON1 = 0b0001100000011010;
    SPI1CON1bits.MSTEN = 0; 
    SPI1CON2 = 0;
    SPI1CON1bits.MODE16 = 0;
    SPI1CON1bits.CKE = 1;
    SPI1CON1bits.CKP = 0;
    SPI1CON1bits.SMP = 0;
    SPI1STATbits.SPIROV = 0;
    
    T5CONbits.TON = 1; // start timer5 for UART data transmission if any data available
 
    _SPI1IF = 0;
    _SPI1IE = 1;
    _SPI1IP = 4;
    SPI1STATbits.SPIEN = 1; // last operation should be to enable the port
}


void SPI1Put(BYTE data)
{
    // Wait for free buffer
    while(SPI1STATbits.SPITBF);
    SPI1BUF = data;
    // Wait for a data byte reception
    while(!SPI1STATbits.SPIRBF);
}


#define SPI1Get() SPI1BUF

void SPI2Put(BYTE data)
{
    // Wait for free buffer
    while(SPI2STATbits.SPITBF);
    SPI2BUF = data;
    // Wait for a data byte reception - EBB not necessary in interrupt mode
    //while(!SPI2STATbits.SPIRBF);
}

#define SPI2Get() SPI2BUF

void InputCapInit(void)
{
	IPC0bits.IC1IP = 5; // Setup Input Capture 1 interrupt for desired priority level 5 (EBB)
	IFS0bits.IC1IF = 0; // Clear the IC1 interrupt status flag
	IEC0bits.IC1IE = 0; // don't enable until HTPA last frame read from SPI port
	//IEC0bits.IC1IE = 1; // Enable IC1 interrupts
	IC1CON1 = 0x0000; // Turn off Input Capture 1 Module
	IC1CON1 = 0b0001110000000011; // Turn on Input Capture 1 Module
	IC1CON2 = 0b0000000000000000;
}

// HTPA_VALID pin input capture interrupt - starts the HTPA_ADC timer, does nothing else
void __attribute__ ((__interrupt__,__auto_psv__)) _IC1Interrupt(void)
{
	TMR4 = 0x0000;
	T4CONbits.TON = 1; // Start Timer4 with assumed settings
	Empty_Buf = IC1BUF;
	Empty_Buf = IC1BUF;
	Empty_Buf = IC1BUF;
	Empty_Buf = IC1BUF;
	IFS0bits.IC1IF = 0; // Reset respective interrupt flag
}

void __attribute__((interrupt,auto_psv)) _OC1Interrupt(void)
{
	IFS0bits.OC1IF = 0;	
} // end of isr for oc1

// HTPA slave SPI receive interrupt (will run at priority 4)
void __attribute__((interrupt,auto_psv)) _SPI1Interrupt(void)
{
	HTPA_counter++;
	if(HTPA_counter==1)
	{
		HTPA_BUF1 = SPI1Get();
		HTPA_BUF1 = HTPA_BUF1 << 8 & 0xFF00;
	}
	if(HTPA_counter==2)
	{
		HTPA_BUF2 = SPI1Get();
		HTPA_BUF2 = HTPA_BUF2 + HTPA_BUF1;
		// test if pixel counter is off
        // note, we have not yet seen this in practice
		/*
		if (PixelCounter>1 && (HTPA_BUF2>PixelCounter))
		{
			// set internal flag to ignore subsequent VALIDs until we're back in synch
			// increment a counter, in case we miss one or two HTPA SPI interrupts
			HTPAErrorCtr++;
		}
		if (HTPAErrorCtr>2)
		{
			// disable VALID ISR until we're at start of frame again
			IEC0bits.IC1IE = 0;
			// send the 3 0xffs and reset the error counter
			_system_flags.PacketisReadytoSend = 1;
			_system_flags.EndofFrameflag = 1;
			PixelCounter=0;
			// send triplet of 0xff's to indicate end of frame
			StreamBuffer[TX_ToSend_Counter++]=0xff;
			if (TX_ToSend_Counter>100) { TX_ToSend_Counter=0; }
			StreamBuffer[TX_ToSend_Counter++]=0xff;
			if (TX_ToSend_Counter>100) { TX_ToSend_Counter=0; }
			StreamBuffer[TX_ToSend_Counter++]=0xff;
			if (TX_ToSend_Counter>100) { TX_ToSend_Counter=0; }
			HTPAErrorCtr=0;
		} // */
		/*if(HTPA_BUF2 == 0)
		{
			PixelCounter=0;
		}*/
		// check if we're on last pixel
		//if(HTPA_BUF2 == 527) {
		if(HTPA_BUF2 == 2047) {
			if (_system_flags.StopHTPAflag)
			{
				// if stop is set, disable the VALID ISR so no further ADC reads happen
				IEC0bits.IC1IE = 0; // disable IC1 HTPA_VALID ISR
			}
			else
			{
				// if we're on last frame of HTPA data, then next VALID refers to first image pixel
				// results in discarding the first 528 HTPA frames = 1 HTPA image
				IEC0bits.IC1IE = 1; // enable VALID ISR, so next pair of ADC reads are the first two pixels (this only changes on end of first frame after startup)
				// reset the HTPA SPI frame number
				HTPA_BUF2 = 0;
				HTPA_BUF1 = 0;
				PixelCounter=0;
			}
			//HTPAErrorCtr=0;
		}	
		HTPA_counter = 0;
	}		
	_SPI1IF = 0;
} // end of isr for HTPA SPI

// HTPA ADC SPI interrupt (we are master, interrupt is called at completion of each transmission we initiate)
void __attribute__((interrupt,auto_psv)) _SPI2Interrupt(void)
{
	// everytime we get a new value, always read the SPI2Buf and increment pixel counter
	adc_array[ADCCounter++] = SPI2Get();
	if (ADCCounter==1) {
	    SPI2Put(0x00);
	} else if (ADCCounter==2) {
		SPI2Put(0x00);
		if (adc_array[ADCCounter-2]==0xf)
		{
			if (adc_array[ADCCounter-1]==0xff) {
				adc_array[ADCCounter-1]=0xfe; 
			}
		}
		StreamBuffer[TX_ToSend_Counter++]=((adc_array[ADCCounter-2]<<4)&0xf0)+((adc_array[ADCCounter-1]>>4)&0x0f);
		// wrap cyclic buffer
		if (TX_ToSend_Counter>100) { TX_ToSend_Counter=0; }
	} else if (ADCCounter==3) {
		SPI2Put(0x00);
	} else if (ADCCounter==4) {
		StreamBuffer[TX_ToSend_Counter++]=((adc_array[ADCCounter-3]<<4)&0xf0)+(adc_array[ADCCounter-2]&0x0f);
		// wrap cyclic buffer
		if (TX_ToSend_Counter>100) { TX_ToSend_Counter=0; }
		if (adc_array[ADCCounter-2]==0xf)
		{
			if (adc_array[ADCCounter-1]==0xff) {
				adc_array[ADCCounter-1]=0xfe;
			}
		}
		StreamBuffer[TX_ToSend_Counter++]=(adc_array[ADCCounter-1]&0xff);
		// wrap cyclic buffer
		if (TX_ToSend_Counter>100) { TX_ToSend_Counter=0; }
		ADC_CS_lat = 1;
		ADCCounter=0;
		PixelCounter++;
	}
	//if(PixelCounter >= 528) {
	if(PixelCounter >= 2048) {
		// reset and flag that frame is done
		_system_flags.PacketisReadytoSend = 1;
		_system_flags.EndofFrameflag = 1;
		PixelCounter=0;
		// send triplet of 0xff's to indicate end of frame
		StreamBuffer[TX_ToSend_Counter++]=0xff;
		if (TX_ToSend_Counter>100) { TX_ToSend_Counter=0; }
		StreamBuffer[TX_ToSend_Counter++]=0xff;
		if (TX_ToSend_Counter>100) { TX_ToSend_Counter=0; }
		StreamBuffer[TX_ToSend_Counter++]=0xff;
		if (TX_ToSend_Counter>100) { TX_ToSend_Counter=0; }
		// if on last round, send an extra triplet of 0xff's to exit stream mode
		if (_system_flags.StopHTPAflag)
		{
			StreamBuffer[TX_ToSend_Counter++]=0xff;
			if (TX_ToSend_Counter>100) { TX_ToSend_Counter=0; }
			StreamBuffer[TX_ToSend_Counter++]=0xff;
			if (TX_ToSend_Counter>100) { TX_ToSend_Counter=0; }
			StreamBuffer[TX_ToSend_Counter++]=0xff;
			if (TX_ToSend_Counter>100) { TX_ToSend_Counter=0; }
		}
	}
	_SPI2IF = 0;
}

// VALID timer to wait until last third of VALID cycle before starting the ADC read
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void)// 
{
	T4CONbits.TON = 0; // stop timer4, we WONT be starting it again here - that happens in IC1
	// initiate the HTPA ADC reading by pulling CS latch low, then putting address of second channel on ADC SPI port
	ADC_CS_lat = 0;
 	SPI2Put(0x08);//0x08, if SPI port is running at 2MHz, then 4usec later, _SPI2Interrupt will trigger
 	//TMR5 = 0x0000;
	T5CONbits.TON = 1; // start the UART timer, in case it isn't already started
	IFS1bits.T4IF = 0; //Reset Timer4 interrupt flag and Return from ISR
}

// timer5 interrupt to send out any data remaining on the uart
void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void)
{
	// don't turn off this timer as long as we're in streammode
	// reset timer count register
	TMR5 = 0x0000;
	// check if data needs to be sent out over UART
	if (TX_Sent_Counter!=TX_ToSend_Counter)
	{
		// only push data into TX buffer if CTS is low, otherwise continue back to main program
		if (CTSport==0)
		{
			if (U1STAbits.UTXBF==0) {
				U1TXREG = StreamBuffer[TX_Sent_Counter++];
				// wrap cyclic buffer on sent side to follow whats to be sent
				if (TX_Sent_Counter>100) { TX_Sent_Counter=0; }
			}	
		}
	}
	IFS1bits.T5IF = 0; //Reset Timer5 interrupt flag and Return from ISR
}

void PWMinit(void)
{
	OC1CON1 = 0b00000000;//
	OC1CON2 = 0x1F;//0x1F
	OC1CON1bits.OCTSEL = 0b111; // This selects the peripheral clock as the clock input to the OC 
	OC1CON1bits.OCM = 0b110; // Load new compare mode to OC1CON			
}

void PWMGenerate( unsigned int duty, unsigned int period)
{
	OC1R = duty; // DUTY CYCLE
	OC1RS = period; // 32000x 62.5 nsec is the period based on PLL with external 
}

void PinMapping(void)
{
	// setup UART pins
	UART_TX_TRIS = 0;
	UART_RX_TRIS = 1;
	UART_TX_AN = 1; // setting analog function to 1 disables the analog function
	UART_RX_AN = 1;

	//flow control
    RTSAN = 1; // disable analog function shared on this pin
    CTSAN = 1; // ditto
    RTStris = 0; 
    CTStris = 1;
    // RN171's CTS
    RTSlat=0;
	// end changes for flow control
	
	ADC_CS_tris	= 0;
	ADC_CS_lat = 0;
	ADC_SCK_tris = 0;
	ADC_SDO_tris = 0;
	ADC_SDI_tris = 1;
	
	HTPA_1MHZ_tris = 0;
	
	HTPA_DATA_tris = 1;
	HTPA_SCK_tris = 1;
	
	HTPA_VALID_tris = 1;
	HTPA_CONT_tris = 0;

	// assign remappable peripheral input functions and RPs to various outputs
	_U1RXR = 15; // B15 = RP15 is PIC_UART_RX
	_RP14R = 3; // RP14 is PIC_UART_TX, assign to 3 for UART1 TX
	_U1CTSR = 12; // B12 is CTS on board
	_RP13R = 4; // RP12 is RTS, assign to 4 for UART1 RTS

	// ADC SPI
	_SDI2R = 21; // ADC SPI2 IN from ADC = RP21
	_RP19R = 10; // ADC SPI2 OUT from ADC to PIC = RP19;, assign to 10 for SPI2 OUT
	_RP4R = 11; // ADC SPI2 CLK = RP4, assign to 8
	
	// HTPA SPI
	_SDI1R = 6; // HTPA SPI1 IN from HTPA = RP6, same pin for output - this is half duplex
	//_RP6R = 7; // set HTPA SPI1 OUT from RP6 (only when PIC is SPI master)
	//_RP20R = 8; // HTPA SPI1 clock = RP20, assign to function 8 for SPI1_clock
	_SCK1R = 20;
	
	// button, valid, HTPA_MCLK
	_INT1R = 25; // button interrupt = RP25
	_IC1R = 7; // HTPA_VALID = RP7 = RB7
	_RP9R = 18; // OC1 out for HTPA RP9 pin, reworked from A8
	TRISAbits.TRISA8 = 1; 
	// end assign functions

	__builtin_write_OSCCONL(OSCCON & 0xbf);//locking the pins
	__builtin_write_OSCCONL(OSCCON | 0x40);
}

