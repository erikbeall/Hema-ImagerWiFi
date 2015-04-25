/////////////////////////////////////////////////////////////////////////////////////
//File Name: Outputs_HTPA.h
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

//Pin Definitions 

// changed for HTPA_v8 for kickstarter 2
/* ADC block - see SPI_HTPA.h
RC3=ADC_DIN (PORTCbits.RC3, TRISCbits.TRISC3)
RC5=ADC_DOUT
RA4=ADC_CS
RB4=ADC_SCLK 
 HTPA block
RC4=HTPA_SCLK
RA8=HTPA_1MHZ
RB5=HTPA_CONT
RB6=HTPA_DATA_IN_OUT
RB7=HTPA_VALID*/

/* RN171 block
RA7=RESET
RB0=GPIO4 (set AN)
RB1=GPIO6 (set AN)
RB2=GPIO8_CS (set AN)
 - for following 4, see UART_HTPA.h
RB12=RTS_RN171_CTS (set AN)
RB13=CTS_RN171_RTS (set AN)
RB14=RN171_RX (set AN)
RB15=RN171_TX  (set AN) */

/* humidity, laser, button, ADC and LEDs
RB8=SCL_SHT21P - undef so far (unconnected)
RB9=SDA_SHT21P - undef
RC9=Button
RB3=VBAT_ADC - see Adc_HTPA.h
RC6=LASER
RC7=LED1.0
RC8=LED1.1 
RA9=5V Enable */
// note RB0-3, RB12-15 are AN bits (keep AN5 for VBAT_ADC), AD1PCFGbits.PCFGxx = 1 for digital, 0 for analog
#define LED1tris TRISCbits.TRISC7 //red
#define LED1port PORTCbits.RC7
#define LED1lat  LATCbits.LATC7

#define LED2tris TRISCbits.TRISC8 //green
#define LED2port PORTCbits.RC8
#define LED2lat  LATCbits.LATC8

#define _5V_ENtris TRISAbits.TRISA9
#define _5V_ENport PORTAbits.RA9
#define _5V_ENlat  LATAbits.LATA9

#define LaserOuttris TRISCbits.TRISC6
#define LaserOutport PORTCbits.RC6
#define LaserOutlat  LATCbits.LATC6

//inputs
/*GPIO4/6 alt functions with "set sys iofunc 0x70"
RB0=GPIO4 - goes high when first client associates, low when no client is on network
RB1=GPIO6 - high when TCP connection is open, low when TCP conn is closed
RB2=GPIO8_CS (sleep as soon as this is high once "set sys trigger 0x20" in cmd mode)
 then use "set sys trigger 2" and use RN171's CTS pin to wake it back up
*/
#define RN171_STATUS_GP6_TCP_TRIS TRISBbits.TRISB1
#define RN171_STATUS_GP6_TCP_PORT PORTBbits.RB1
#define RN171_STATUS_GP6_TCP_LAT  LATBbits.LATB1
#define RN171_STATUS_GP6_TCP_AN AD1PCFGbits.PCFG3

#define RN171_STATUS_GP4_WIFICX_TRIS TRISBbits.TRISB0
#define RN171_STATUS_GP4_WIFICX_PORT PORTBbits.RB0
#define RN171_STATUS_GP4_WIFICX_LAT LATBbits.LATB0
#define RN171_STATUS_GP4_WIFICX_AN AD1PCFGbits.PCFG2

#define RN171_GP8_SHTDWN_TRIS TRISBbits.TRISB2
#define RN171_GP8_SHTDWN_PORT PORTBbits.RB2
#define RN171_GP8_SHTDWN_LAT LATBbits.LATB2
#define RN171_GP8_SHTDWN_AN AD1PCFGbits.PCFG4

// pull this low to reset module, otherwise leave high
#define RN171_RESET_TRIS TRISAbits.TRISA7 
#define RN171_RESET_PORT PORTAbits.RA7
#define RN171_RESET_LAT  LATAbits.LATA7

#define WRITE_ADDR 0xA0//10100000
#define READ_ADDR 0xA1//10100001


//////////////////

#define FIRST_ADDRESS 0x7D00//0x7D00,0x3E80
#define LAST_ADDRESS 0xABFE
//-----------------------------------------------------------------------------

//No Connected Pins


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Function:     OutputInit
//PreCondition: None
//Input:        None
//Output:       None
//Overview:     Setup LED initializations for TRIS and LAT, any other outputs too.
//Note:         
//~~~~~~~~~
void OutputInit(void);


//-----------------------------------------------------------------------------
// function prototypes for I2c
//-----------------------------------------------------------------------------
//I2C init
void i2c_init(void);
//I2C start function
void i2c_start(void);
//I2C reset function
void reset_i2c_bus(void);
//I2C send function
char send_i2c_byte(int data);
//I2c write packet
void I2Cwrite(unsigned int address, unsigned char data);
//I2c Read Packet
unsigned char I2Cread(unsigned int addr);
//write initializing
//void I2CWriteInit(void);


void EraseWord(unsigned int addr);

void WriteWord(unsigned int addr, unsigned int data);

			
unsigned int ReadWord(unsigned int addr);





