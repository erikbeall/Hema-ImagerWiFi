/////////////////////////////////////////////////////////////////////////////////////
//File Name: Outputs_HTPA.c
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
 
 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     OutputInit
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Setup LED initializations for TRIS and LAT, any other outputs too.
//Note:         
//~~~~~~~~~
void OutputInit(void)
{

	LED1tris = 0; //LED sinking (low = off, high = on)
	LED1lat = 1;//LED off
	
	LED2tris = 0; //LED on sinking
	LED2lat = 1;//LED off
	
	HTPA_CONT_tris = 0;
	HTPA_CONT_lat = 0;
	
	_5V_ENtris = 0;
	_5V_ENlat = 0;
	
	// active-low to put in low power mode
	// Not anymore with RN171 - active low pulse to reset the module (has a 100k pull-up), so write a 1
	RN171_RESET_TRIS = 0;
	RN171_RESET_LAT = 1;

	// sleep/shutdown output (sleep as soon as this is high once "set sys trigger 0x20" in cmd mode)
	// then use "set sys trigger 2" and use RN171's CTS pin to wake it back up
	RN171_GP8_SHTDWN_TRIS = 0;
	RN171_GP8_SHTDWN_LAT = 0;
	RN171_GP8_SHTDWN_AN = 1;
	
	// setup STATUS line - use this to monitor connection status and stop streaming/go to sleep when lost
	RN171_STATUS_GP6_TCP_TRIS = 1;
	RN171_STATUS_GP6_TCP_AN = 1;
	RN171_STATUS_GP4_WIFICX_TRIS = 1;
	RN171_STATUS_GP4_WIFICX_AN = 1;
	
	LaserOuttris = 0;
	LaserOutlat = 0;
}



//////////////////i2c
////function initiates I2C1 module to baud rate BRG
void i2c_init(void)
{
   int temp;
   I2C1BRG = 37;// 400kHz for 16Mhz Fcy
   I2C1CONbits.I2CEN = 0;	// Disable I2C Mode
   I2C1CONbits.DISSLW = 1;	// Disable slew rate control
   IFS1bits.MI2C1IF = 0;	 // Clear Interrupt
   I2C1CONbits.I2CEN = 1;	// Enable I2C Mode
   temp = I2C1RCV;	 // read buffer to clear buffer full
   reset_i2c_bus();	 // set bus to idle
}
//function iniates a start condition on bus
void i2c_start(void)
{
   int x = 0;
   I2C1CONbits.ACKDT = 0;	//Reset any previous Ack
   Delay10us(1);
   I2C1CONbits.SEN = 1;	//Initiate Start condition
   Nop();

   //the hardware will automatically clear Start Bit
   //wait for automatic clear before proceding
   while (I2C1CONbits.SEN)
   {
      Delay10us(1);
      x++;
      if (x > 20)
      break;
   }
   Delay10us(1);
}
//Resets the I2C bus to Idle
void reset_i2c_bus(void)
{
   int x = 0;
   //initiate stop bit
   I2C1CONbits.PEN = 1;
   //wait for hardware clear of stop bit
   while (I2C1CONbits.PEN)
   {
      Delay10us(1);
      x ++;
      if (x > 20) break;
   }
   I2C1CONbits.RCEN = 0;
   IFS1bits.MI2C1IF = 0; // Clear Interrupt
   I2C1STATbits.IWCOL = 0;
   I2C1STATbits.BCL = 0;
   Delay10us(1);
}
//basic I2C byte send
char send_i2c_byte(int data)
{
   int i;
   while (I2C1STATbits.TBF) { }
   IFS1bits.MI2C1IF = 0; // Clear Interrupt
   I2C1TRN = data; // load the outgoing data byte
   // wait for transmission
   for (i=0; i<500; i++)
   {
      if (!I2C1STATbits.TRSTAT) break;
      Delay10us(1);
      }
      if (i == 500) {
      return(1);
   }
   // Check for NO_ACK from slave, abort if not found
   if (I2C1STATbits.ACKSTAT == 1)
   {
      reset_i2c_bus();
      return(1);
   }
   Delay10us(1);
   return(0);
}
void I2Cwrite(unsigned int address, unsigned char data)
{
	unsigned char TempData;
    i2c_start();
	send_i2c_byte(WRITE_ADDR);	// Indicates address
	TempData = (address >> 8)& 0x00FF;
	send_i2c_byte(TempData);	// high byte
	TempData = address& 0x00FF;
	send_i2c_byte(TempData);	// low byte
	send_i2c_byte(data);
	reset_i2c_bus();

}
unsigned char I2Cread(unsigned int address)
{
	unsigned char TempData;
	i2c_start();
	send_i2c_byte(WRITE_ADDR);	// Indicates address	
	TempData = (address >> 8)& 0x00FF;
	send_i2c_byte(TempData);	// high byte
	TempData = address & 0x00FF;
	send_i2c_byte(TempData);	// low byte
	reset_i2c_bus();
	i2c_start();
	
	send_i2c_byte(READ_ADDR);	// Indicates address
	Delay10us(1);	
   int i = 0;
   char data = 0;

   //set I2C module to receive
   I2C1CONbits.RCEN = 1;

   //if no response, break
   while (!I2C1STATbits.RBF)
   {
      i ++;
      if (i > 2000) break;
   }

   //get data from I2CRCV register
   data = I2C1RCV;
//	//set ACK to high
//   I2C1CONbits.ACKEN = 1;
//
//   //wait before exiting
//   DelayuSec(10);
	reset_i2c_bus();
   //return data
   return data;	
}
void EraseWord(unsigned int addr)
{
	
	
	
// C example using MPLAB C30
//unsigned long progAddr = addr; // Address of row to write
unsigned int offset;
//Set up pointer to the first memory location to be written
TBLPAG = 0x00; // Initialize PM Page Boundary SFR
offset = addr; // Initialize lower word of address
__builtin_tblwtl(offset, 0x0000); // Set base address of erase block
// with dummy latch write
NVMCON = 0x4042; // Initialize NVMCON
asm("DISI #5"); // Block all interrupts with priority <7
// for next 5 instructions
__builtin_write_NVM(); // check function to perform unlock
// sequence and set WR	
	
}
void WriteWord(unsigned int addr, unsigned int data)
{
	
	
// C example using MPLAB C30
unsigned int offset;
//unsigned long progAddr = addr; // Address of word to program
unsigned int progDataL; // Data to program lower word
unsigned char progDataH ; // Data to program upper byte
progDataL = data;
progDataH = 0xFF;
//Set up NVMCON for word programming
NVMCON = 0x4003; // Initialize NVMCON
//Set up pointer to the first memory location to be written
TBLPAG = 0x00; // Initialize PM Page Boundary SFR
offset = addr; // Initialize lower word of address
//Perform TBLWT instructions to write latches
__builtin_tblwtl(offset, progDataL); // Write to address low word
__builtin_tblwth(offset, progDataH); // Write to upper byte
asm("DISI #5"); // Block interrupts with priority <7
// for next 5 instructions
__builtin_write_NVM(); // C30 function to perfo	
}
unsigned int ReadWord(unsigned int addr)
{
// C example using MPLAB C30
unsigned int offset;
//unsigned long progAddr = addr; // Address of word to program
unsigned int datal;
unsigned int datah;
//Set up pointer to the first memory location to be written
//TBLPAG = progAddr>>16; // Initialize PM Page Boundary SFR
TBLPAG = 0x00; // Initialize PM Page Boundary SFR
//offset = progAddr & 0xFFFF; // Initialize lower word of address
offset = addr; // Initialize lower word of address
//Perform TBLWT instructions to write latches
datal = __builtin_tblrdl(offset); // Write to address low word
datah = __builtin_tblrdh(offset); // Write to upper byte
return datal;
}	

	

//-----------------------------------------------------------------------------
