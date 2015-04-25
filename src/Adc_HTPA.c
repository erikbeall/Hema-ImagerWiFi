/////////////////////////////////////////////////////////////////////////////////////
//File Name: Adc_HTPA.c
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

///*****************************************************************************
//* Function: ADCInit
//* Preconditions: None.
//* Overview: This function initiates ADC for single conversion, select Analog input pins
//* Input: None.
//* Output: None.
//******************************************************************************/
void ADCinit() 
{
    VBAT_ADC_TRIS = 1; // VBAT_ADC as input
	VBAT_ADC_AN = 0;

    AD1CON1 = 0b1000000011100100;   // auto convert after end of sampling 
    AD1CSSL = 0;        // no scanning required 
    AD1CON3 = 0b0001111100000001;   // max sample time = 31Tad, Tad = 2 x Tcy = 125ns >75ns
    AD1CON2 = 0b0000000000000000;        // use MUXA, AVss and AVdd are used as Vref+/-
    AD1CON1bits.ADON = 1; // turn on the ADC
} //initADC


///*****************************************************************************
//* Function: readADC
//* Preconditions: None.
//* Overview: This function returns an int ADC read value based on "ch" character channel input.
//* Input: None.
//* Output: None.
//******************************************************************************/
unsigned int readADC( unsigned char ch)
{
unsigned int temp; //create a temp variable for limiting range of output
unsigned char i;
unsigned int ADAVG;
//unsigned int ADACCUM;

ADAVG = 0;
temp = 0;
AD1CHS  = ch;               // select analog input channel	
AD1CON1bits.SAMP = 1;       // start sampling, automatic conversion will follow  //dummy read 
while (!AD1CON1bits.DONE);   // wait to complete the conversion
AD1CON1bits.SAMP = 1;       // start sampling, automatic conversion will follow  //dummy read
while (!AD1CON1bits.DONE);   // wait to complete the conversion


for (i=0;i<64;i++)
	{
//	AD1CON1bits.SAMP = 1;       // start sampling, automatic conversion will follow  //dummy read
//	while (!AD1CON1bits.DONE);   // wait to complete the conversion	
	ADAVG = ADC1BUF0;  //Added for averaging	
	}
 return ADAVG;

} // readADC





		    


