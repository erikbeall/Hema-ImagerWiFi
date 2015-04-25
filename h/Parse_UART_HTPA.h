/////////////////////////////////////////////////////////////////////////////////////
//File Name: Parse_UART_HTPA.h
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
//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define SLEEPTIME 180

#define STATUS_REQUEST 0x00
#define STREAM_REQUEST 0x01
#define CAL_CONSTANTS 0x02//request
#define CAL_CONSTANTS_UPDATE 0x03
#define HALT_DATA_STANDBY 0x04
#define HALT_DATA_SLEEPMODE 0x05
#define CONFIGURE_BLUETOOTH 0x06
#define LASER_ON 0x0A
#define BATTERY 0x0C
//
#define _12BIT 0x00
#define _16BIT 0x01
#define STOP_STREAMING 0x02
#define SEND_SINGLE_12BIT 0x03
#define SEND_SINGLE_16BIT 0x04
#define SEND_FRAMES_ONE_DELAY 0x05
//


//-----------------------------------------------------------------------------
// Structs
//-----------------------------------------------------------------------------
    
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

extern unsigned int MemoryLength;

//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void ParsingRXMsg(void);
//
void ConfigBlueTooth(void);//
//
void StatusMessage(void);//
//
void StreamMessage(void);//
//
void PTATMessage(void);//
//
void PixelSenMessage(void);//
//
void PixelSlopeMessage(void);//
//
void PixelOffsetMessage(void);//
//
void ArrayTempMessage(void);//
//
void InterpretFactorMessage(void);//
//
void AbsoluteTempMessage(void);//
//
void ThermalArrayMessage(void);//
//
void PasswordMessage(void);//
//
//-----------------------------------------------------------------------------
//#endif
//-----------------------------------------------------------------------------
void ChangeClassofDevice(void);

void SetNameofDevice(void);

void ChangeBaudRateTempTo230K(void);

void CommandMode(void);

void ExitCommandMode(void);

void Reboot(void);
