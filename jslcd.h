/**************************************************************************************************
* Name: 
* Version: 
* Description: LCD Pin is edited for Vibration feeding controller Project. (Mass Product Prototype)
* I/O:	
* Update: 2014/07/23 15:01
* First written : 
**************************************************************************************************/
#include <p30f4011.h>

/* Global Define *********************************************************************************/
/*Character LCD Interface Pins*/ 
#define TRIS_DATA_PIN_4     TRISEbits.TRISE5
#define TRIS_DATA_PIN_5     TRISEbits.TRISE4	
#define TRIS_DATA_PIN_6     TRISEbits.TRISE3		
#define TRIS_DATA_PIN_7     TRISEbits.TRISE2			
#define TRIS_RS             TRISFbits.TRISF1   			
#define TRIS_E              TRISFbits.TRISF0 			

#define DATA_PIN_4          LATEbits.LATE5				
#define DATA_PIN_5          LATEbits.LATE4				
#define DATA_PIN_6          LATEbits.LATE3				
#define DATA_PIN_7          LATEbits.LATE2				
#define RS_PIN              LATFbits.LATF0     		
#define E_PIN               LATFbits.LATF1    

/* Display ON/OFF Control */
#define DON                 0x0F  						// Display on      
#define DOFF                0x0B  						// Display off     
#define CURSOR_ON           0x0F  						// Cursor on       
#define CURSOR_OFF          0x0D  						// Cursor off      
#define BLINK_ON            0x0F  						// Cursor Blink    
#define BLINK_OFF           0x0E  						// Cursor No Blink 

/* Cursor or Display Shift */
#define SHIFT_CUR_LEFT      0x13  						// Cursor shifts to the left   
#define SHIFT_CUR_RIGHT     0x17  						// Cursor shifts to the right  
#define SHIFT_DISP_LEFT     0x1B  						// Display shifts to the left  
#define SHIFT_DISP_RIGHT    0x1F  						// Display shifts to the right  	

#define LCD_LINE1           SetDDRamAddr(0x00)  
#define LCD_LINE2           SetDDRamAddr(0x40)		

void lcd_delay(unsigned int ms)
{
	unsigned int x,a;		// Keep for counter loop  
	for(x=0;x<ms;x++)
	{
		for(a=0;a<816;a++);	// Loop for delay 1 millisec per unit
	}
}

/* Function Prototypes here!! ********************************************************************/
// void Initial_4bitLCD(void);								// Initial LCD Interface
// void SetCGRamAddr(unsigned char);						
// void SetDDRamAddr(unsigned char);						// Set Cursor Address
// void WriteCmdLCD(unsigned char);						// Write Command
// void WriteDataLCD(unsigned char);						// Write Data
// void PutsLCD(unsigned char*);							// Print Message
// void Delay_tW_LCD(void);								// Enable Pulse Delay	
// void Busy_LCD(void);									// Wait LCD Busy
// void Delay(unsigned long int);	





/**********************
* Delay Time Function 
*     1-4294967296     
***********************/
void Delay(unsigned long int count1)
{
  while(count1 > 0) {count1--;}							// Loop Decrease Counter	
}
void Delay_tW_LCD(void)                  // Enable Pulse Delay
{
  int i;
  for(i=0;i<18;i++);       
}

void Busy_LCD(void)                    // 40uS Delay
{
  unsigned int i;
  for(i=0;i<1800;i++);      
}

void SetCGRamAddr(unsigned char address)
{
  RS_PIN = 0;  											// Select Instruction  
  DATA_PIN_7 = 0;										// DB7 Must be 0 For Setting CGRam ADDR
  DATA_PIN_6 = 1;										// DB6 Must be 1 For Setting CGRam ADDR  
  DATA_PIN_5 = ((address & 0x20)>>5);
  DATA_PIN_4 = ((address & 0x10)>>4);    
  E_PIN = 1;											// Strobe High Nibble
  Delay_tW_LCD();										// Enable Pulse Delay
  E_PIN = 0;
  Busy_LCD();

  RS_PIN = 0;  											// Select Instruction
  DATA_PIN_7 = ((address & 0x08)>>3);
  DATA_PIN_6 = ((address & 0x04)>>2);
  DATA_PIN_5 = ((address & 0x02)>>1);
  DATA_PIN_4 = ((address & 0x01)>>0);
  E_PIN = 1;											// Strobe Low Nibble
  Delay_tW_LCD();										// Enable Pulse Delay
  E_PIN = 0;   
  Busy_LCD();  
}

void SetDDRamAddr(unsigned char address)
{  
  RS_PIN = 0;   										// Select Instruction 
  DATA_PIN_7 = 1;										// DB7 Must be 1 For Setting DDRam ADDR 
  DATA_PIN_6 = ((address & 0x40)>>6); 
  DATA_PIN_5 = ((address & 0x20)>>5);
  DATA_PIN_4 = ((address & 0x10)>>4);
  E_PIN = 1;											// Strobe High Nibble
  Delay_tW_LCD();										// Enable Pulse Delay
  E_PIN = 0;
  Busy_LCD();
 
  RS_PIN = 0;   										// Select Instruction 
  DATA_PIN_7 = ((address & 0x08)>>3);
  DATA_PIN_6 = ((address & 0x04)>>2);
  DATA_PIN_5 = ((address & 0x02)>>1);
  DATA_PIN_4 = ((address & 0x01)>>0);
  E_PIN = 1;    										// Strobe Low Nibble
  Delay_tW_LCD();										// Enable Pulse Delay
  E_PIN =0;
  Busy_LCD();
} 

void WriteCmdLCD(unsigned char cmd)
{
  RS_PIN = 0;   										// Select Instruction 
  DATA_PIN_7 = ((cmd & 0x80)>>7);
  DATA_PIN_6 = ((cmd & 0x40)>>6);
  DATA_PIN_5 = ((cmd & 0x20)>>5);
  DATA_PIN_4 = ((cmd & 0x10)>>4);
  E_PIN = 1;											// Strobe High Nibble
  Delay_tW_LCD();										// Enable Pulse Delay
  E_PIN = 0;
  Busy_LCD();
  
  RS_PIN = 0;   										// Select Instruction 
  DATA_PIN_7 = ((cmd & 0x08)>>3);
  DATA_PIN_6 = ((cmd & 0x04)>>2);
  DATA_PIN_5 = ((cmd & 0x02)>>1);
  DATA_PIN_4 = (cmd & 0x01);
  E_PIN = 1;											// Strobe Low Nibble
  Delay_tW_LCD();										// Enable Pulse Delay
  E_PIN = 0;
  Busy_LCD();
  Delay(50000);											// 1.64mS Delay
}  

void WriteDataLCD(unsigned char data)
{
  RS_PIN = 1;  											// Select Data                    
  DATA_PIN_7 = ((data & 0x80)>>7);
  DATA_PIN_6 = ((data & 0x40)>>6);
  DATA_PIN_5 = ((data & 0x20)>>5);
  DATA_PIN_4 = ((data & 0x10)>>4);
  E_PIN = 1;											// Strobe High Nibble
  Delay_tW_LCD();										// Enable Pulse Delay
  E_PIN = 0;
  Busy_LCD();

  RS_PIN = 1;  											// Select Data   
  DATA_PIN_7 = ((data & 0x08)>>3);
  DATA_PIN_6 = ((data & 0x04)>>2);
  DATA_PIN_5 = ((data & 0x02)>>1);
  DATA_PIN_4 = (data & 0x01);
  E_PIN = 1;											// Strobe Low Nibble
  Delay_tW_LCD();										// Enable Pulse Delay
  E_PIN = 0;
  Busy_LCD();
}  
void Initial_4bitLCD(void)
{
  TRIS_DATA_PIN_4 = 0;                  // Set Port Direction = Output
  TRIS_DATA_PIN_5 = 0;
  TRIS_DATA_PIN_6 = 0;
  TRIS_DATA_PIN_7 = 0;
  TRIS_RS = 0;
  TRIS_E = 0;

  RS_PIN = 0;                     // Instruction Select
  E_PIN = 0;                      // Disable Strobe
  Delay(500000);                     // Power-ON Delay (minimum of 15ms) 
  
  WriteCmdLCD(0x33);                  // Initial 4 Bit Interface Format  
  WriteCmdLCD(0x32);                  
  WriteCmdLCD(0x28);                  // Function Set (DL=0 4-Bit,N=1 2 Line,F=0 5X7)             

  WriteCmdLCD(0x0C);                  // Display on/off Control (Entry Display,Cursor off,Cursor not Blink)
  WriteCmdLCD(0x06);                    // Entry Mode Set (I/D=1 Increment,S=0 Cursor Shift)    
  WriteCmdLCD(0x01);                    // Clear Display  (Clear Display,Set DD RAM Address=0) 
}
void PutsLCD(unsigned char* buffer)
{
  while(*buffer != '\0')								// Loop Until End of Message
  {    
    WriteDataLCD(*buffer); 								// Write Character to LCD
    buffer++;
  } 
}
void LcdPrint(char* inp_str){
  char str[8];
  sprintf(str,"%8s",inp_str);
  PutsLCD((unsigned char *)str);
}
void LcdPrintNum(char* inp_str, unsigned int inp_num){
  char str[8];
  sprintf(str,inp_str, inp_num);
  PutsLCD((unsigned char *)str);
}
void clear_lcd(void)
{
	Initial_4bitLCD();
	SetDDRamAddr(0x08);									// Start Cursor Line-1    
    PutsLCD((unsigned char *)"        ");
	SetDDRamAddr(0x48);									// Start Cursor Line-1    
    PutsLCD((unsigned char *)"        ");
}

