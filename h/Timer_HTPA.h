/////////////////////////////////////////////////////////////////////////////////////
//File Name: Timer_HTPA.h
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
extern unsigned int ms_flag ;//millisecond flag
extern unsigned ms1;
extern unsigned int ms50;  //used to store the 50ms software time count
extern unsigned int ms100;  //used to store the 100ms software time count
extern unsigned int ms250;  //used to store the 250ms software time count
extern unsigned int ms1000; //used to store the 1000ms software time count
extern unsigned long int ms3600000; //used to store the 3600000ms = 1 hour software time count

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimerInit
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Initializes Timer0 for use.
//~~~~~~~~~
void TimerInit(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimerIsOverflowEvent
//PreCondition: None
//Input:        None
//Output:       Status
//Overview:     Checks for an overflow event, returns TRUE if an overflow occurred.
//Note:         This function should be checked at least twice per overflow period.
//~~~~~~~~~
unsigned char TimerIsOverflowEvent(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimeBase50ms
//PreCondition: None
//Input:        None
//Output:       Status
//Overview:     Creates a 50 ms time base when called on a 1ms period.
//Note:         Nest this function within TimerIsOverflowEvent IF statement to get new time bases.
//~~~~~~~~~
unsigned char TimeBase50ms(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimeBase100ms
//PreCondition: None
//Input:        None
//Output:       Status
//Overview:     Creates a 100 ms time base when called on a 1ms period.
//Note:         Nest this function within TimerIsOverflowEvent IF statement to get new time bases.
//~~~~~~~~~
unsigned char TimeBase100ms(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimeBase250ms
//PreCondition: None
//Input:        None
//Output:       Status
//Overview:     Creates a 250 ms time base when called on a 1ms period.
//Note:         Nest this function within TimerIsOverflowEvent IF statement to get new time bases.
//~~~~~~~~~
unsigned char TimeBase250ms(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimeBase1s
//PreCondition: None
//Input:        None
//Output:       Status
//Overview:     Creates a 1s time base when called on a 1ms period.
//Note:         Nest this function within TimerIsOverflowEvent IF statement to get new time bases.
//~~~~~~~~~
unsigned char TimeBase1s(void);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     TimeBase1h
//PreCondition: None
//Input:        None
//Output:       Status
//Overview:     Creates a 1hour time base when called on a 1ms period.
//Note:         Nest this function within TimerIsOverflowEvent IF statement to get new time bases.
//~~~~~~~~~
unsigned char TimeBase1h(void);


