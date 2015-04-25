/////////////////////////////////////////////////////////////////////////////////////
//File Name: Adc_HTPA.h
//Processor: PIC24FJ64GA104
//Compiler: MPLAB C30
//Linker: MPLAB LINK30
//Company: Hema Imaging

//
//Author               Date            Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//Mehrdad Ramazanali   11-04=2013
// Erik Beall          10-25-2014 (changed pin locations)
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/////////////////////////////////////////////////////////////////////////////////////
//Pin Definitions for Analog Inputs
#define VBAT_ADC_PORT PORTBbits.RB3
#define VBAT_ADC_TRIS TRISBbits.TRISB3
#define VBAT_ADC_LAT LATBbits.LATB3
#define VBAT_ADC_AN AD1PCFGbits.PCFG5


///*****************************************************************************
//* Function: ADCInit
//* Preconditions: None.
//* Overview: This function initiates ADC for single conversion, select Analog input pins
//* Input: None.
//* Output: None.
//******************************************************************************/
void ADCinit();

///*****************************************************************************
//* Function: readADC
//* Preconditions: None.
//* Overview: This function returns an int ADC read value based on "ch" character channel input.
//* Input: None.
//* Output: None.
//******************************************************************************/
unsigned int readADC( unsigned char ch);








