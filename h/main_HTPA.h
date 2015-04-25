/////////////////////////////////////////////////////////////////////////////////////
//File Name: main_HTPA.h
//Processor: PIC24FJ64GA104
//Compiler: MPLAB C30
//Linker: MPLAB LINK30
//Company: Hema Imaging

//
//Author               Date            Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//Mehrdad Ramazanali   11-04=2013
//Erik Beall           10-25-2014 HTPA_PIC_v8
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/////////////////////////////////////////////////////////////////////////////////////
extern unsigned char TxPacket_Stream[101];
extern unsigned char adc_array[4];
extern unsigned char TxPacket[101];
extern unsigned char TxPacket_Data[101];
extern unsigned int SleepCounter;
extern unsigned int LaserCounter;
extern unsigned int StopTimer;

//#include <libpic30.h>
//#include <p24Fxxxx.h>



//#include <p24fxxxx.h>
//#include <p24F16KL402.h>
#include <p24FJ64GA104.h>
#include "GenericTypeDefs.h"
#include "Buttons_HTPA.h"
#include "Outputs_HTPA.h"
#include "Timer_HTPA.h"
#include "SPI_HTPA.h"
#include "Parse_UART_HTPA.h"
#include "UART_HTPA.h"
#include "TimeDelay.h"
#include "Adc_HTPA.h"
#include "DEE Emulation 16-bit.h"
#include <stdio.h>


//State Machine for thermostat
typedef enum
{
    STATE_STANDBY = 0,
    STATE_STREAM,
	STATE_SLEEP
	
} STATE_MACHINE;


//*****************************************************************************
// Structure: FLAGS _system_flags
//
// Overview: the structure provides access to bit flags
//
//****************************************************************************/
typedef struct tagFLAGS {
	
		unsigned PacketisReadyforParse:1;//ready to parse
		unsigned PacketisSentflag:1;    
		unsigned PacketisReadytoSend:1;
		unsigned EndofFrameflag:1;
		unsigned SW1released:1;
		unsigned Streamflag:1;
		unsigned Standbyflag:1;
		unsigned Sleepflag:1;
		unsigned SendByteStuffingflag:1;
		unsigned TwelveBitflag:1;
		unsigned SixteenBitflag:1;
		//
		unsigned SendStatusMessage:1;
		unsigned XORflag:1;
		unsigned Stopflag:1;
		unsigned TurnLaserONflag:1;
		unsigned RefreshStatusflag:1;
		unsigned uart_hi_speed:1;
		unsigned PacketisBeingReceived:1;
		unsigned ChangeUARTSpeed:1;
		unsigned StopHTPAflag:1;
		unsigned InSleepWake:1;
}FLAGS;
extern FLAGS _system_flags;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     FLAGInit
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Initialize FLAG bit fields
//Note:         
//~~~~~~~~~
void FLAGInit(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     FLAGMaint
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Maintenance on FLAG bit fields
//Note:         
//~~~~~~~~~
void FLAGMaint(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     SYSTEMInit
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Initialize system variables
//Note:         
//~~~~~~~~~
void SYSTEMInit(void);

//state machine
void StateMachine(void);

void TurnOnHTPA(void);

void TurnOffHTPA(void);

void ByteStuffing(void);

void TwelveBitMode(void);

void SixteenBitMode(void);


