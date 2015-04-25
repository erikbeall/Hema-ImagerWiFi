/////////////////////////////////////////////////////////////////////////////////////
//File Name: UART_HTPA.h
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
//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define SER_SYNCH_CODE   0xA5
#define SER_INTERBYTE_TIME_OUT  150
#define FALSE 0
#define TRUE 1

//-----------------------------------------------------------------------------
// Structs
//-----------------------------------------------------------------------------
   typedef enum
{
//! Waiting for the synchronisation byte 0xA5
GET_SYNC_STATE=0,

GET_HEADER_STATE,
//
GET_DATA_STATE,

} STATES_GET_PACKET;

 typedef enum
{

BYTE1_STATE=0,

BYTE2_STATE,
//
BYTE3_STATE,

BYTE4_STATE

} STATES_SEND_PACKET;
//    
//-----------------------------------------------------------------------------
// Unions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Enums
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// typedef externs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// variable externs
//-----------------------------------------------------------------------------

extern unsigned char uartDataAvailable_receive;
extern unsigned char Rxdata[512];
extern unsigned char Txdata[100];
extern unsigned int TX_Counter;
extern unsigned int TX_ToSend_Counter;
extern unsigned int TX_Sent_Counter;
extern unsigned int RX_Counter;
extern unsigned int datalength_RX ;
extern unsigned int datalength_TX ;
extern unsigned char PacketType;
extern unsigned char DataByte;
extern unsigned char ChunkCounter;
//extern unsigned char Type;
//extern unsigned char Length;
//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
//this function gets the packet from dolphin
void uart_getPacket(unsigned char reset);


//this function is initializing UART
void initUART(unsigned char BaudRate);

// this function sends the packet to dolphin
void uart_sendPacket(void);
//
void uart_sendStreamData(unsigned int counter);
//
void uart_sendHeader(unsigned int datalength, unsigned char Type);

//
unsigned char SpecialCharacterTest(unsigned char databyte);

void uart_sendData(void);
//-----------------------------------------------------------------------------
//#endif
//-----------------------------------------------------------------------------
