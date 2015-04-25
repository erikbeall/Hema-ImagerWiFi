/////////////////////////////////////////////////////////////////////////////////////
//File Name: Buttons_HTPA.c
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

#include "main_HTPA.h"


//*****************************************************************************
// Debounce button counters
//
//****************************************************************************/
unsigned char _b1_cnt;
unsigned char _b2_cnt;

//*****************************************************************************
// Structure: BUTTONS _button_press
//
// Overview: the structure provides a n access to button bit indicators.
//
//****************************************************************************/
BUTTON _button_press;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     ButtonTest
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Put this in a while loop to test buttons
//Note:         
//~~~~~~~~~
void ButtonTest(void)
{


}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     DBButtonTest
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Put this in a while loop to test debounced buttons
//Note:         
//~~~~~~~~~
void DBButtonTest(void)
{

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     BtnInit
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Setup debounce
//Note:         
//~~~~~~~~~
void BtnInit(void)
{
	SW1tris = 1; //Input
	//SW1AN = 1;// digital
	_button_press.SW1DBB = 0;  
	_b1_cnt = 128; //initialize debounce counts
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     BtnProcessEvents
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Must be called periodically to process buttons
//Note:         
//~~~~~~~~~
void BtnProcessEvents(void)
{

	if (!SW1port) {
		_b1_cnt++;
		if (_b1_cnt > (128 + BUTTON_MAX_DEBOUCE)){
			_b1_cnt = (128 + BUTTON_MAX_DEBOUCE);
			_button_press.SW1DBB = 1;
		}		
	}
	else {
		_b1_cnt--;
		if (_b1_cnt < (128 - BUTTON_MAX_DEBOUCE)){
			_b1_cnt = (128 - BUTTON_MAX_DEBOUCE);
			_button_press.SW1DBB = 0;
		}
	} //End of individual button debounce

}// End of button process events




