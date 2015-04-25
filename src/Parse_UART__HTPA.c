/////////////////////////////////////////////////////////////////////////////////////
//File Name: Parse_UART_HTPA.c
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
//	Description: This file contains the UART interrrupt, UART initialization
#include "main_HTPA.h"				 
	
//-----------------------------------------------------------------------------
// file defined, global scope variables
//-----------------------------------------------------------------------------


////-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// variable externs
//-----------------------------------------------------------------------------

////-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// variable externs
//-----------------------------------------------------------------------------
unsigned int j;
unsigned int Dummy;
unsigned int MemoryLength;
//-----------------------------------------------------------------------------
// function pointer externs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void ParsingRXMsg(void)
{
    switch(PacketType)
    {
    case STATUS_REQUEST:
        TX_Counter=0;
        while(!U1STAbits.TRMT);
		Txdata[TX_Counter] = SER_SYNCH_CODE;
		IFS0bits.U1TXIF = 1;
        while(!U1STAbits.TRMT);
		Txdata[TX_Counter] = STATUS_REQUEST;
		IFS0bits.U1TXIF = 1;
		while(!U1STAbits.TRMT);
		Txdata[TX_Counter] = 0;
		IFS0bits.U1TXIF = 1;
		while(!U1STAbits.TRMT);
		Txdata[TX_Counter] = 1;
		IFS0bits.U1TXIF = 1;
		while(!U1STAbits.TRMT);
		Txdata[TX_Counter] = ( readADC(1)/28) * 50;// 5 volts for 250 bits
		IFS0bits.U1TXIF = 1;
		while(!U1STAbits.TRMT);
		Txdata[TX_Counter] = SER_SYNCH_CODE+1;
		IFS0bits.U1TXIF = 1;
    	break;
    case STREAM_REQUEST:
    	switch(Rxdata[0])
    	{
    		case _12BIT:
        		_system_flags.Streamflag = 1;
        		_system_flags.TwelveBitflag = 1;
        		FrameNumber = 5;
    			break;
    		case _16BIT:
        		_system_flags.Streamflag = 1;
        		_system_flags.SixteenBitflag = 1;
        		FrameNumber = 10;
    			break;
    		case STOP_STREAMING:
    			_system_flags.Stopflag=1;
    			break;
    		case SEND_SINGLE_12BIT:
        		_system_flags.Streamflag = 1;
        		_system_flags.TwelveBitflag = 1;
        		FrameNumber = 2;
    			break;
    		case SEND_SINGLE_16BIT:
        		_system_flags.Streamflag = 1;
        		_system_flags.SixteenBitflag = 1;
        		FrameNumber = 2;
    			break;
    		case SEND_FRAMES_ONE_DELAY:
    			break;
    		if(Rxdata[0]> 5)
    		{
    		}
    	}//end of STREAM_REQUEST switch
    	break;
    case CAL_CONSTANTS:
    	if(ReadWord(FIRST_ADDRESS )!= 0)
    	{
    		uart_sendHeader(MemoryLength, 0x0C);
    		for(j=0;j<MemoryLength;j++)
    		{
    			Dummy = ReadWord(FIRST_ADDRESS + (j + 1 ));
    			TxPacket_Stream[0] = Dummy >> 8;
    			uart_sendStreamData(0);
    			TxPacket_Stream[0] = Dummy;
    			uart_sendStreamData(0);
    		}
    	}//
	    break;
    case CAL_CONSTANTS_UPDATE:
        if(ChunkCounter==0) MemoryLength = 0;
        ChunkCounter++;
        for(j=0;j< 270;j++)// 540/2
        {
            Dummy = (Rxdata[j*2]<< 8) + Rxdata[1+(j* 2)];
            WriteWord((FIRST_ADDRESS + j) + (ChunkCounter * 270) - 269, Dummy);
        }
        MemoryLength = MemoryLength + 270;
        if(ChunkCounter== 38)
        {
            WriteWord(FIRST_ADDRESS , MemoryLength);
            ChunkCounter = 0;
        }
        TxPacket_Stream[0] = 0xFF;
        uart_sendStreamData(0);
        break;
    case HALT_DATA_STANDBY:
        _system_flags.Stopflag=1;
        break;
    case HALT_DATA_SLEEPMODE:
    	_system_flags.Stopflag =1;
        break;
    case CONFIGURE_BLUETOOTH:
        // set flags to change UART speed when done with current image and stop the HTPA
        _system_flags.ChangeUARTSpeed = 1;
        _system_flags.Stopflag =1;
		break;
 	case LASER_ON:
        LaserOutlat = ~LaserOutlat;
        // default 5 second timer
        LaserCounter=5;
        break;
    case BATTERY:
        break;
    }
    unsigned int i;
    // clear out the Rx buffer
    for(i=0;i<256;i++)
    {
        Rxdata[i]   = 0x00;
    }
}//end of Parsing


void ConfigBlueTooth(void)//building a config packet
{

}//end of config message	


void StatusMessage(void)//building a data packet
{
TxPacket[0] = 0xAA;
TxPacket[1] = 0x0B;
TxPacket[2] = 0x01;
TxPacket[3] = 0xD1;
TxPacket[4] = 0xAF;
TxPacket[5] = 0xC3;//MAC Address
TxPacket[6] = 0x01;
TxPacket[7] = 0x01;//device name
TxPacket[8] = 0x00;
TxPacket[9] = 0x00;//firmware version
TxPacket[10] = 0x00;//battery
TxPacket[11] = 0x00;
TxPacket[12] = 0x00;
TxPacket[13] = 0x00;
TxPacket[14] = 0x00;
TxPacket[15] = 0x00;
TxPacket[16] = 0x00;
TxPacket[17] = 0x00;
TxPacket[18] = 0x00;
TxPacket[19] = 0x00;
TxPacket[20] = 0x00;
TxPacket[21] = 0x00;
datalength_TX = 22;
}
void StreamMessage(void)
{

	
}	
//
void PTATMessage(void)
{
}	
//
void PixelSenMessage(void)
{
}	
//
void PixelSlopeMessage(void)
{
}	
//
void PixelOffsetMessage(void)
{
}	
//
void ArrayTempMessage(void)
{
}	
//
void InterpretFactorMessage(void)
{
}	
//
void AbsoluteTempMessage(void)
{
}	
//
void ThermalArrayMessage(void)
{
}	
//
void PasswordMessage(void)
{
}	
void ChangeClassofDevice(void)
{
	
	
}
void SetNameofDevice(void)
{
	TxPacket_Data[0] = 'S' ; 
	TxPacket_Data[1] = 'N';
	TxPacket_Data[2] = ',' ;
	TxPacket_Data[3] = 'H';
	TxPacket_Data[4] = 'e' ;
	TxPacket_Data[5] = 'm' ;
	TxPacket_Data[6] = 'a' ;
	//TxPacket_Data[7] = 0x49 ;
	//TxPacket_Data[8] = 0x4D ;
	//TxPacket_Data[9] = 0x41 ;
	//TxPacket_Data[10] = 0x47;
	//TxPacket_Data[11] = 0x49 ;
	//TxPacket_Data[12] = 0x4E ;
	//TxPacket_Data[13] = 0x47 ;
	datalength_TX = 7;	
	uart_sendData();	
}
void ChangeBaudRateTempTo230K(void)
{
	TxPacket_Data[0] = 'S' ; 
	TxPacket_Data[1] = 'U';
	TxPacket_Data[2] = ',' ;
	TxPacket_Data[3] = '2';
	TxPacket_Data[4] = '3' ;
	TxPacket_Data[5] = '0' ;
	TxPacket_Data[6] = 'k' ;
	TxPacket_Data[7] = '\n' ;
	datalength_TX = 8;
	uart_sendData();
}

void CommandMode(void)
{
	TxPacket_Data[0] = '$' ;
	TxPacket_Data[1] = '$' ;
	TxPacket_Data[2] = '$' ;
	datalength_TX = 3;	
	uart_sendData();	
}

void ExitCommandMode(void)
{
	TxPacket_Data[0] = 0x2D ;
	TxPacket_Data[1] = 0x2D ;
	TxPacket_Data[2] = 0x2D ;
	TxPacket_Data[3] = '\n' ;
	datalength_TX = 4;
	uart_sendData();
}
void Reboot(void)
{
	TxPacket_Data[0] = 'R' ;
	TxPacket_Data[1] = ',';
	TxPacket_Data[2] = '1' ;
	//TxPacket_Data[0] = 'Z' ;
	datalength_TX = 3;
	uart_sendData();
}
						
//-----------------------------------------------------------------------------





