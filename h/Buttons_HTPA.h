/////////////////////////////////////////////////////////////////////////////////////
//File Name: Buttons_HTPA.h
//Processor: PIC24FJ64GA104
//Compiler: MPLAB C30
//Linker: MPLAB LINK30
//Company: Hema Imaging

//
//Author               Date            Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//Mehrdad Ramazanali   11-04=2013
// EBeall 10-25-2014
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/////////////////////////////////////////////////////////////////////////////////////

//Pin Definitions for Analog Inputs

#define SW1tris TRISCbits.TRISC9   //clear
#define SW1port PORTCbits.RC9
#define SW1lat  LATCbits.LATC9
//#define SW1AN   AD1PCFGbits.PCFG9
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     ButtonTest
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Put this in a while loop to test buttons
//Note:         
//~~~~~~~~~
void ButtonTest(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     DBButtonTest
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Put this in a while loop to test debounced buttons
//Note:         
//~~~~~~~~~
void DBButtonTest(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     BtnInit
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Setup debounce and initialize TRIS settings
//Note:         
//~~~~~~~~~
void BtnInit(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     BtnProcessEvents
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Must be called periodically to process buttons
//Note:         
//~~~~~~~~~
void BtnProcessEvents(void);

//*****************************************************************************
// Structure: BUTTONS _button_press
//
// Overview: the structure provides a n access to button bit indicators.
//
//****************************************************************************/
typedef struct tagButton {
	unsigned SW1DBB:1;  //24 VAC input Debounce Bit (Bit-Field member of Structure in C)
}BUTTON;
extern BUTTON _button_press;

//*****************************************************************************
// Debounce button counters
//
//****************************************************************************/
#define	BUTTON_MAX_DEBOUCE 	25
extern unsigned char _b1_cnt; 
extern unsigned char _b2_cnt;



