/////////////////////////////////////////////////////////////////////////////////////
//File Name: UART_HTPA.c
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
#include<uart.h>


	
//-----------------------------------------------------------------------------
// file defined, global scope variables
//-----------------------------------------------------------------------------


////-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// variable externs
//-----------------------------------------------------------------------------
unsigned char uartDataAvailable_receive; // Indicates that the UART has processed data.
unsigned char Rxdata[512];
unsigned char Txdata[100];
unsigned int TX_Counter = 0;
unsigned int TX_ToSend_Counter = 0;
unsigned int TX_Sent_Counter = 0;
unsigned int RX_Counter = 0;
unsigned int OERR_Counter = 0;	
unsigned int datalength_RX = 0 ;
unsigned int datalength_TX ;
unsigned char PacketType;
unsigned char DataByte;
unsigned char ChunkCounter;

//unsigned char Type;
//unsigned char Length;
//-----------------------------------------------------------------------------
// function pointer externs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------

 //initializing UART
void initUART(unsigned char BaudRate)
{
   U1BRG = BaudRate; 
   IPC3bits.U1TXIP2 = 1; //Set Uart TX Interrupt Priority (examples show 1,0,0 for bits 2:0)
   IPC3bits.U1TXIP1 = 0;
   IPC3bits.U1TXIP0 = 0;
   IPC2bits.U1RXIP2 = 1; //Set Uart RX Interrupt Priority
   IPC2bits.U1RXIP1 = 0;
   IPC2bits.U1RXIP0 = 0;
   U1MODE = 0b1000001000001000;//enable the flow control, 8n1, high baud rate and U1RX is idle high
   // to enable flow control, read CTSlat or CTSport whenever we want to load TXbuf with data
   // and wait til CTS is 0. while (CTSport);
   // similarly, when the RTSMD mode = 0, the peripheral will set RTS high whenever buffer is full or shifting
   // and UEN<1:0> = 10 or 01

   U1STA =  0b0000000000000000;
   U1STAbits.UTXEN = 1; //Enable Transmit (bit 10 of U1STA)
   IFS0bits.U1RXIF = 0; // clear  UART Rx interrupt flag
   IFS0bits.U1TXIF = 0; // clear UART TX flag
   IEC0bits.U1TXIE = 1; //Enable Transmit Interrupt
   IEC0bits.U1RXIE = 1; //Enable Receive Interrupt
   U1STAbits.OERR = 0; // reset Overflow bit
} 

void uart_getPacket(unsigned char reset)
{
	{
		RX_Counter = 0;
		static STATES_GET_PACKET UState = GET_SYNC_STATE; // initialization to GET_SYNC_STATE
		static unsigned int Count1 = 0;//! Nr. of bytes received
		if (reset==1)
		{
			UState = GET_SYNC_STATE;
			Count1 = 0;
			return;
		}
		switch(UState)
		{
			// Waiting for packet sync byte
			case GET_SYNC_STATE:
				if (DataByte == SER_SYNCH_CODE)
				{
					UState = GET_HEADER_STATE;
					Count1 = 0;
				}
				break;
			case GET_HEADER_STATE:
				Count1++;
				if(Count1== 1)
				{
					if(_system_flags.XORflag)
					{
						DataByte ^= 0x20;	
						PacketType = DataByte;
						_system_flags.XORflag =0;	
					}
					else
					{		
						if(SpecialCharacterTest(DataByte)== FALSE)
						{
							PacketType = DataByte;
						}
						else
						{
							_system_flags.XORflag = 1;
							Count1--;	
						}
					}
					PacketType = DataByte;
				}
				if(Count1== 2)
				{
					if(_system_flags.XORflag)
					{
						DataByte ^= 0x20;
						datalength_RX = DataByte;
						datalength_RX = (datalength_RX<<8)&0xFF00;
						_system_flags.XORflag =0;	
					}
					else
					{
						if(SpecialCharacterTest(DataByte)== FALSE)
						{
							datalength_RX = DataByte;
							datalength_RX = (datalength_RX<<8)&0xFF00;
						}
						else
						{
							_system_flags.XORflag = 1;
							Count1--;
						}
					}
					datalength_RX = DataByte;					
				}
				if(Count1== 3)
				{
					if(_system_flags.XORflag)
					{
						DataByte ^= 0x20;
						datalength_RX = DataByte + datalength_RX;
						_system_flags.XORflag =0;	
					}
					else
					{
						if(SpecialCharacterTest(DataByte)== FALSE)
							datalength_RX = DataByte + datalength_RX;
						else
						{
							_system_flags.XORflag = 1;
							Count1--;
						}	
					}
					datalength_RX = DataByte + datalength_RX;							
					UState = GET_DATA_STATE;
					Count1 = 0;
				}
				break;
			case GET_DATA_STATE:
				Count1++;
				if(Count1 <= datalength_RX)
				{
					if(_system_flags.XORflag)
					{
						DataByte ^= 0x20;
						Rxdata[Count1-1] = DataByte;
						_system_flags.XORflag =0;	
					}
					else
					{
						if(SpecialCharacterTest(DataByte)== FALSE) {
							Rxdata[Count1-1] = DataByte;
						}
						else
						{
							_system_flags.XORflag = 1;
							Count1--;
						}	
					}
					Rxdata[Count1-1] = DataByte;			
				}
				if(Count1 == datalength_RX)
				{
					UState = GET_SYNC_STATE;
					Count1 = 0;
					_system_flags.PacketisReadyforParse = 1;
				}
				break;
		}
	}// empty brace
	return;	
}//end of getPacket     


void uart_sendPacket(void)
{			
	unsigned int j = 0;
	TX_Counter = 0;
	//data
	for (j=0 ; j< (datalength_TX  ) ; j++)
 	{
	 	if (Txdata[j]==0x7D || Txdata[j]==0xa5 || Txdata[j]==0xa6)
		{
			Txdata[j] = 0x7D;
			IFS0bits.U1TXIF = 1;//enable the interrupt
		    TxPacket[j] ^= 0x20;
			Txdata[j] = TxPacket[j];
 			IFS0bits.U1TXIF = 1;//enable the interrupt	
		}
		else
		{
		 	Txdata[j] = TxPacket[j];
	 		IFS0bits.U1TXIF = 1;//enable the interrupt
 		}			 	
	}
	//crc check for data
}

void uart_sendStreamData(unsigned int counter)
{
	TX_Counter = 0;	
	Txdata[TX_Counter]= TxPacket_Stream[counter] ; 
	IFS0bits.U1TXIF = 1;//enable the interrupt
}


void uart_sendData(void)
{
	TX_Counter = 0;	
	unsigned char j;
	for (j=0 ; j< (datalength_TX) ; j++)
 	{
		Txdata[j] = TxPacket_Data[j];
 		IFS0bits.U1TXIF = 1;//enable the interrupt	
	}
}


void uart_sendHeader(unsigned int datalength, unsigned char Type)
{
	TX_Counter = 0;
	// //transmit sync code
	Txdata[0]= SER_SYNCH_CODE ;//send the sync code 
	IFS0bits.U1TXIF = 1;//enable the interrupt
	Txdata[1] = Type;
	IFS0bits.U1TXIF = 1;//enable the interrupt
	// //datalength byte 1	
	Txdata[2] = (datalength >>8)&0xFF;
	if (Txdata[2]==0x7D || Txdata[2]==0xa5 || Txdata[2]==0xa6)
	{
		Txdata[2] = 0x7D;
		IFS0bits.U1TXIF = 1;//enable the interrupt		
		Txdata[3]= (datalength >>8)&0xFF;
		Txdata[3]^=0x20;
  		IFS0bits.U1TXIF = 1;//enable the interrupt
  		Txdata[4] = datalength ;
  		if (Txdata[4]==0x7D || Txdata[4]==0xa5 || Txdata[4]==0xa6)
		{
			Txdata[4] = 0x7D;
			IFS0bits.U1TXIF = 1;//enable the interrupt
			Txdata[5] = datalength ;
			Txdata[5]^=0x20;
			IFS0bits.U1TXIF = 1;//enable the interrupt
		}
		else
		{
			IFS0bits.U1TXIF = 1;//enable the interrupt	
		}			
	}
	else
	{	
		IFS0bits.U1TXIF = 1;//enable the interrupt	
		Txdata[3] = datalength ;
		if (Txdata[3]==0x7D || Txdata[3]==0xa5 || Txdata[3]==0xa6)
		{
			Txdata[3] = 0x7D;
			IFS0bits.U1TXIF = 1;//enable the interrupt
			Txdata[4] = datalength ;
			Txdata[4]^=0x20;
			IFS0bits.U1TXIF = 1;//enable the interrupt
		}
		else
		{
			IFS0bits.U1TXIF = 1;//enable the interrupt
		}			
	}  
}

unsigned char SpecialCharacterTest(unsigned char databyte)
{
	if (databyte==0x7D)
		return TRUE;
	else
		return FALSE;
}

// ISR for transmit UART
// NOTE: we have separated the stream buffer from the command/header transmit buffer, use sent and to-send counter equality to end out a stream
// and disable/re-enable the TX ISR
void __attribute__((interrupt,shadow,   no_auto_psv)) _U1TXInterrupt()
{
	while(!U1STAbits.TRMT);
	U1TXREG = Txdata[TX_Counter];
	TX_Counter++;
	IFS0bits.U1TXIF = 0;//clear interrupt flag
}  // end	U1TxInterrupt

////-----------------------------------------------------------------------------
void __attribute__((interrupt,shadow,   no_auto_psv)) _U1RXInterrupt()
{
	if (U1STAbits.OERR || U1STAbits.FERR)
	{
  		DataByte = U1RXREG;
		U1STAbits.OERR = 0; // reset Overflow bit
		U1STAbits.FERR = 0; // reset Overflow bit
   	} // end	("check for errors")
	else
	{
		DataByte = U1RXREG;
		uartDataAvailable_receive = 1; // set flag to let us know data is available.	
	}
	IFS0bits.U1RXIF = 0; // clear  UART Rx interrupt flag		
} // end	_U1RXInterrupt()
//-----------------------------------------------------------------------------
