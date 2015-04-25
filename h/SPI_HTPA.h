/////////////////////////////////////////////////////////////////////////////////////
//File Name: SPI_HTPA.h
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


//Global Variables

extern unsigned int FrameCounter;
extern unsigned int PixelCounter;
extern unsigned char FrameNumber;
extern unsigned int HTPA_counter;
//************************************************************************
//ADC SPI Ports 
//************************************************************************/
/* ADC block
RC3=ADC_DIN (PORTCbits.RC3, TRISCbits.TRISC3)
RC5=ADC_DOUT (make this ADC_SDI since we are master)
RA4=ADC_CS
RB4=ADC_SCLK */
#define ADC_CS_tris TRISAbits.TRISA4  //Chip Select
#define ADC_CS_port PORTAbits.RA4
#define ADC_CS_lat  LATAbits.LATA4

#define ADC_SCK_tris TRISBbits.TRISB4  //SCK
#define ADC_SCK_port PORTBbits.RB4
#define ADC_SCK_lat  LATBbits.LATB4

#define ADC_SDO_tris TRISCbits.TRISC3  //SDO
#define ADC_SDO_port PORTCbits.RC3
#define ADC_SDO_lat  LATCbits.LATC3

#define ADC_SDI_tris TRISCbits.TRISC5  //SDI
#define ADC_SDI_port PORTCbits.RC5
#define ADC_SDI_lat  LATCbits.LATC5

//************************************************************************
//HTPA SPI Ports 
//************************************************************************/
/* HTPA block
RC4=HTPA_SCLK
RA8=HTPA_1MHZ
RB5=HTPA_CONT
RB6=HTPA_DATA_IN_OUT
RB7=HTPA_VALID */
#define HTPA_CONT_tris TRISBbits.TRISB5  //HTPA control pin
#define HTPA_CONT_port PORTBbits.RB5
#define HTPA_CONT_lat  LATBbits.LATB5

#define HTPA_SCK_tris TRISCbits.TRISC4  //SCK
#define HTPA_SCK_port PORTCbits.RC4
#define HTPA_SCK_lat  LATCbits.LATC4

#define HTPA_DATA_tris TRISBbits.TRISB6  //data
#define HTPA_DATA_port PORTBbits.RB6
#define HTPA_DATA_lat  LATBbits.LATB6

/*#define HTPA_1MHZ_tris TRISAbits.TRISA8  //1mhz clock
#define HTPA_1MHZ_port PORTAbits.RA8
#define HTPA_1MHZ_lat  LATAbits.LATA8 */
// used TDO JTAG output for MCLK - rework to remove this connection, change to RP9 for upper left corner of SHT21P pads
#define HTPA_1MHZ_tris TRISBbits.TRISB9  //1mhz clock
#define HTPA_1MHZ_port PORTBbits.RB9
#define HTPA_1MHZ_lat  LATBbits.LATB9

#define HTPA_VALID_tris TRISBbits.TRISB7  //valid pin, we never need this definition, we're setting RP7 to CN1
#define HTPA_VALID_port PORTBbits.RB7
#define HTPA_VALID_lat  LATBbits.LATB7
///*************************************************
// UART ports
///*************************************************
//outputs
/*
RB12=RTS_RN171_CTS (set AN12)
RB13=CTS_RN171_RTS (set AN11)
RB14=TX_RN171_RX (set AN10)
RB15=RX_RN171_TX  (set AN9) */
#define UART_RX_TRIS TRISBbits.TRISB15
#define UART_RX_LAT LATBbits.LATB15
#define UART_RX_PORT PORTBbits.RB15
#define UART_RX_AN AD1PCFGbits.PCFG9

#define UART_TX_TRIS TRISBbits.TRISB14
#define UART_TX_LAT LATBbits.LATB14
#define UART_TX_PORT PORTBbits.RB14
#define UART_TX_AN AD1PCFGbits.PCFG10

/* CTS and RTS inverted on board
#define CTStris TRISBbits.TRISB13 //red
#define CTSport PORTBbits.RB13
#define CTSlat  LATBbits.LATB13
#define CTSAN   AD1PCFGbits.PCFG11

#define RTStris TRISBbits.TRISB12 //green
#define RTSport PORTBbits.RB12
#define RTSlat  LATBbits.LATB12
#define RTSAN   AD1PCFGbits.PCFG12 */

#define CTStris TRISBbits.TRISB12
#define CTSport PORTBbits.RB12
#define CTSlat  LATBbits.LATB12
#define CTSAN   AD1PCFGbits.PCFG12

#define RTStris TRISBbits.TRISB13
#define RTSport PORTBbits.RB13
#define RTSlat  LATBbits.LATB13
#define RTSAN   AD1PCFGbits.PCFG11

void ADCInit(void);

void HTPAInit(void);

void ReadADC(void);
//////////////////////////////

void InputCapInit(void);//for HTPA valid pin

void PWMinit(void);

void PWMGenerate( unsigned int duty, unsigned int period);

void CNInit(void);

void PinMapping(void);


