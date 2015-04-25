/////////////////////////////////////////////////////////////////////////////////////
//File Name: Timer_HTPA.c
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
#define DELAY_uS 1
unsigned int ms_flag ;//millisecond flag
unsigned ms1;
unsigned int ms50;                  //used to store the 50ms software time count
unsigned int ms100;                 //used to store the 100ms software time count
unsigned int ms250;                 //used to store the 250ms software time count
unsigned int ms1000;                //used to store the 250ms software time count
unsigned long int ms3600000; //used to store the 3600000ms = 1 hour software time count
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimerInit
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Initializes Timer0 for use.
//~~~~~~~~~
void TimerInit(void)
{
	PR1 = 0x3E80;  //1ms   0x3E80 = 1ms @ 8MHZ oscillator with PLL = 32 MHZ 
	IPC0bits.T1IP = 1; 
	// the default priority level after device reset is 4
	T1CON = 0b1000000000000000;
	IFS0bits.T1IF = 0;
	TMR1 = 0x00; 		// Reset TMR1 count to zero.
	IEC0bits.T1IE = 1; 	// enable TMR1 interrupt
	T1CONbits.TON = 1; 	// Turn on TMR1 interrupt


	//timer4
	TMR4 = 0x0000;
	T4CON = 0x0000; //Start Timer4 with prescaler settings at 1:64 and T3CON = 0x0020
	PR4 = 2400;
	IPC6bits.T4IP = 5; // EBB set priority to 5 for T4
	//IPC6bits.T4IP = 4; // Setup Output Compare 3 interrupt for
	IFS1bits.T4IF = 0; // Clear Output Compare 3 interrupt flag
	IEC1bits.T4IE = 1; // Enable Output Compare 3 interrupts
	T4CONbits.TON = 0; // do NOT Start Timer4

	TMR5 = 0x0000;
	T5CON = 0x0000;
	PR5 = 160; // 10 usec delay
	IPC7bits.T5IP = 1; // set lower priority than ADC read, VALID, HTPA SPI, or ADC time delay ISRs
	IFS1bits.T5IF = 0; // Clear Output Compare 5 interrupt flag
	IEC1bits.T5IE = 1; // Enable Output Compare 5 interrupts
	T5CONbits.TON = 0;

	ms_flag = 0; // Gets set by _T1Interrupt every ms.
	ms1 = 0;
	ms50 = 0; //initialize the 50 ms software timers
	ms100 = 0; //initialize the 100 ms software timers
	ms250 = 0; //initialize the 250 ms software timers
	ms1000 = 0; //initialize the 1000 ms software timers
          
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimerIsOverflowEvent
//PreCondition: None
//Input:        None
//Output:       Status
//Overview:     Checks for an overflow event, returns TRUE if an overflow occurred.
//Note:         This function should be checked at least twice per overflow period.
//~~~~~~~~~
//unsigned char TimerIsOverflowEvent(void)
//{
////	 ms1++;
////	 if( ms1>250)
////	 {
//		if (IFS0bits.T1IF)
//		{		
//			IFS0bits.T1IF = 0;
//		//	 ms1 = 0;
//			return(1);
//		}
////	}
//	return(0);
//}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimeBase100ms
//PreCondition: None
//Input:        None
//Output:       Status
//Overview:     Creates a 100 ms time base when called on a 1ms period.
//Note:         Nest this function within TimerIsOverflowEvent IF statement to get new time bases.
//~~~~~~~~~
unsigned char TimeBase100ms(void)
{
	if (ms100 < 100)
	{		
		ms100++; //increment the ms100 counter
		return(0);
	}
	ms100 = 0;   //clear the count for next time
	return(1);   //must be greater than or equal to 100, return true
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimeBase50ms
//PreCondition: None
//Input:        None
//Output:       Status
//Overview:     Creates a 50 ms time base when called on a 1ms period.
//Note:         Nest this function within TimerIsOverflowEvent IF statement to get new time bases.
//~~~~~~~~~
unsigned char TimeBase50ms(void)
{
	if (ms50 < 50)
	{		
		ms50++; //increment the ms50 counter
		return(0);
	}
	ms50 = 0;   //clear the count for next time
	return(1);   //must be greater than or equal to 50, return true
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimeBase250ms
//PreCondition: None
//Input:        None
//Output:       Status
//Overview:     Creates a 250 ms time base when called on a 1ms period.
//Note:         Nest this function within TimerIsOverflowEvent IF statement to get new time bases.
//~~~~~~~~~
unsigned char TimeBase250ms(void)
{
	if (ms250 < 250)
	{		
		ms250++; //increment the ms250 counter
		return(0);
	}
	ms250 = 0;   //clear the count for next time
	return(1);   //must be greater than or equal to 250, return true
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimeBase1s
//PreCondition: None
//Input:        None
//Output:       Status
//Overview:     Creates a 1s time base when called on a 1ms period.
//Note:         Nest this function within TimerIsOverflowEvent IF statement to get new time bases.
//~~~~~~~~~
unsigned char TimeBase1s(void)
{
	if (ms1000 < 1000)
	{		
		ms1000++; //increment the ms1000 counter
		return(0);
	}
	ms1000 = 0;   //clear the count for next time
	return(1);   //must be greater than or equal to 1000, return true
}


unsigned char TimeBase1h(void)
{
	if (ms3600000<3600000)
	{		
		ms3600000++; //increment the ms1000 counter
		return(0);
	}
	ms3600000 = 0;   //clear the count for next time
	return(1);   //must be greater than or equal to 1000, return true
}
void __attribute__((interrupt,auto_psv)) _T1Interrupt(void)
{
	ms_flag = 1;	
	IFS0bits.T1IF = 0;	
} // end T1 ISR

