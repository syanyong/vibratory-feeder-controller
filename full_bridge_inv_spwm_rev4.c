/**************************************************************************************************
* Name: dsPIC30F4011 for control half-bridge inverter using SPWM
* Version: rev3
* Description: 
* I/O:	RB0 = LED STATUS;
*		RB1	= ADC1;
*		RB2 = ADC2;
*		RD0 = SPWM Output;
*		RD1 = Reserved;
*		RD2 = (Debugging) Sin Wave Frequency
*		RD3 = (Debugging) Sampling time
*		UART = 19200
* First written : Sarucha Yanyong
**************************************************************************************************/
#include <p30F4011.h>
#include <dsp.h>
#include <adc10.h>
#include "spwm.h"
#include <stdio.h>

/* Setup Configuration For ET-BASE dsPIC30F4011 */
_FOSC(CSW_FSCM_OFF & XT_PLL16);								// Disable Clock Switching,Enable Fail-Salf Clock // Clock Source = Primary XT + (PLL x 8)
_FWDT(WDT_OFF);												// Disable Watchdog 
_FBORPOR(PBOR_OFF & PWRT_64 & MCLR_EN);					    // Disable Brown-Out ,Power ON = 64mS,Enable MCLR
_FGS(CODE_PROT_OFF);										// Code Protect OFF
/* End Configuration For ET-BASE dsPIC30F4011 */

/* Global Define *********************************************************************************/
#define RELAY_PIN			_LATE4 /*E0-E3 For SPWM*/
#define LED 				_LATB0
#define ADC1_PIN			_LATB1
#define ADC2_PIN			_LATB2
#define ADC3_PIN			_LATB3
#define BT_START			!PORTDbits.RD0
#define EXT_TRIGGER			_LATD1
#define MOVEING_SIZE		10

/* Global Variable *******************************************************************************/
/*For SPWM Function*/
saw_spwm spwm_kk = {0,0,0,0,0,0,0};
const unsigned int sin_table[2000] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,251,252,253,254,255,256,257,258,259,260,261,262,262,263,264,265,266,267,268,269,270,271,272,273,273,274,275,276,277,278,279,280,281,282,282,283,284,285,286,287,288,289,290,291,291,292,293,294,295,296,297,298,299,300,300,301,302,303,304,305,306,307,307,308,309,310,311,312,313,314,315,315,316,317,318,319,320,321,321,322,323,324,325,326,327,328,328,329,330,331,332,333,334,334,335,336,337,338,339,340,340,341,342,343,344,345,346,346,347,348,349,350,351,351,352,353,354,355,356,356,357,358,359,360,361,361,362,363,364,365,366,366,367,368,369,370,371,371,372,373,374,375,375,376,377,378,379,379,380,381,382,383,384,384,385,386,387,388,388,389,390,391,392,392,393,394,395,395,396,397,398,399,399,400,401,402,403,403,404,405,406,406,407,408,409,410,410,411,412,413,413,414,415,416,416,417,418,419,419,420,421,422,423,423,424,425,426,426,427,428,429,429,430,431,432,432,433,434,434,435,436,437,437,438,439,440,440,441,442,443,443,444,445,445,446,447,448,448,449,450,450,451,452,453,453,454,455,455,456,457,458,458,459,460,460,461,462,462,463,464,465,465,466,467,467,468,469,469,470,471,471,472,473,473,474,475,475,476,477,477,478,479,479,480,481,481,482,483,483,484,485,485,486,487,487,488,489,489,490,491,491,492,493,493,494,494,495,496,496,497,498,498,499,500,500,501,501,502,503,503,504,505,505,506,506,507,508,508,509,509,510,511,511,512,512,513,514,514,515,515,516,517,517,518,518,519,520,520,521,521,522,522,523,524,524,525,525,526,527,527,528,528,529,529,530,530,531,532,532,533,533,534,534,535,536,536,537,537,538,538,539,539,540,540,541,541,542,543,543,544,544,545,545,546,546,547,547,548,548,549,549,550,550,551,551,552,552,553,553,554,554,555,555,556,556,557,557,558,558,559,559,560,560,561,561,562,562,563,563,564,564,565,565,566,566,567,567,568,568,568,569,569,570,570,571,571,572,572,573,573,573,574,574,575,575,576,576,577,577,577,578,578,579,579,580,580,580,581,581,582,582,583,583,583,584,584,585,585,585,586,586,587,587,587,588,588,589,589,589,590,590,591,591,591,592,592,592,593,593,594,594,594,595,595,595,596,596,597,597,597,598,598,598,599,599,599,600,600,600,601,601,602,602,602,603,603,603,604,604,604,605,605,605,606,606,606,606,607,607,607,608,608,608,609,609,609,610,610,610,611,611,611,611,612,612,612,613,613,613,613,614,614,614,615,615,615,615,616,616,616,617,617,617,617,618,618,618,618,619,619,619,619,620,620,620,620,621,621,621,621,622,622,622,622,623,623,623,623,623,624,624,624,624,625,625,625,625,625,626,626,626,626,627,627,627,627,627,628,628,628,628,628,628,629,629,629,629,629,630,630,630,630,630,630,631,631,631,631,631,631,632,632,632,632,632,632,633,633,633,633,633,633,633,634,634,634,634,634,634,634,635,635,635,635,635,635,635,635,636,636,636,636,636,636,636,636,636,637,637,637,637,637,637,637,637,637,637,638,638,638,638,638,638,638,638,638,638,638,638,638,639,639,639,639,639,639,639,639,639,639,639,639,639,639,639,639,639,639,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,639,639,639,639,639,639,639,639,639,639,639,639,639,639,639,639,639,639,638,638,638,638,638,638,638,638,638,638,638,638,638,637,637,637,637,637,637,637,637,637,637,636,636,636,636,636,636,636,636,636,635,635,635,635,635,635,635,635,634,634,634,634,634,634,634,633,633,633,633,633,633,633,632,632,632,632,632,632,631,631,631,631,631,631,630,630,630,630,630,630,629,629,629,629,629,628,628,628,628,628,627,627,627,627,627,626,626,626,626,626,625,625,625,625,625,624,624,624,624,623,623,623,623,623,622,622,622,622,621,621,621,621,620,620,620,620,619,619,619,619,618,618,618,618,617,617,617,616,616,616,616,615,615,615,615,614,614,614,613,613,613,613,612,612,612,611,611,611,610,610,610,610,609,609,609,608,608,608,607,607,607,606,606,606,605,605,605,604,604,604,603,603,603,602,602,602,601,601,601,600,600,600,599,599,599,598,598,598,597,597,596,596,596,595,595,595,594,594,594,593,593,592,592,592,591,591,590,590,590,589,589,588,588,588,587,587,587,586,586,585,585,584,584,584,583,583,582,582,582,581,581,580,580,579,579,579,578,578,577,577,576,576,576,575,575,574,574,573,573,572,572,572,571,571,570,570,569,569,568,568,567,567,566,566,566,565,565,564,564,563,563,562,562,561,561,560,560,559,559,558,558,557,557,556,556,555,555,554,554,553,553,552,552,551,551,550,550,549,549,548,548,547,547,546,546,545,545,544,543,543,542,542,541,541,540,540,539,539,538,538,537,536,536,535,535,534,534,533,533,532,531,531,530,530,529,529,528,528,527,526,526,525,525,524,524,523,522,522,521,521,520,519,519,518,518,517,516,516,515,515,514,514,513,512,512,511,510,510,509,509,508,507,507,506,506,505,504,504,503,503,502,501,501,500,499,499,498,497,497,496,496,495,494,494,493,492,492,491,490,490,489,489,488,487,487,486,485,485,484,483,483,482,481,481,480,479,479,478,477,477,476,475,475,474,473,473,472,471,471,470,469,468,468,467,466,466,465,464,464,463,462,462,461,460,459,459,458,457,457,456,455,455,454,453,452,452,451,450,450,449,448,447,447,446,445,445,444,443,442,442,441,440,439,439,438,437,437,436,435,434,434,433,432,431,431,430,429,428,428,427,426,425,425,424,423,422,422,421,420,419,419,418,417,416,416,415,414,413,412,412,411,410,409,409,408,407,406,405,405,404,403,402,402,401,400,399,398,398,397,396,395,395,394,393,392,391,391,390,389,388,387,387,386,385,384,383,383,382,381,380,379,378,378,377,376,375,374,374,373,372,371,370,369,369,368,367,366,365,365,364,363,362,361,360,360,359,358,357,356,355,355,354,353,352,351,350,350,349,348,347,346,345,344,344,343,342,341,340,339,339,338,337,336,335,334,333,333,332,331,330,329,328,327,326,326,325,324,323,322,321,320,320,319,318,317,316,315,314,313,313,312,311,310,309,308,307,306,306,305,304,303,302,301,300,299,298,298,297,296,295,294,293,292,291,290,289,289,288,287,286,285,284,283,282,281,280,280,279,278,277,276,275,274,273,272,271,271,270,269,268,267,266,265,264,263,262,261,260,260,259,258,257,256,255,254,253,252,251,250,249,248,248,247,246,245,244,243,242,241,240,239,238,237,236,235,234,234,233,232,231,230,229,228,227,226,225,224,223,222,221,220,219,218,218,217,216,215,214,213,212,211,210,209,208,207,206,205,204,203,202,201,200,199,199,198,197,196,195,194,193,192,191,190,189,188,187,186,185,184,183,182,181,180,179,178,177,176,175,174,174,173,172,171,170,169,168,167,166,165,164,163,162,161,160,159,158,157,156,155,154,153,152,151,150,149,148,147,146,145,144,143,142,141,140,139,138,137,136,135,134,134,133,132,131,130,129,128,127,126,125,124,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
unsigned tri_table[SPWM_SAWSIZE];
unsigned char spwm_sin_amp;

/*For Main Function*/
unsigned char initial_flag;
unsigned char first_spwm_sin_amp;
char serial_buffer[20]="";
unsigned int mcnt, mcnt2;
unsigned int result[20], i;

/*ADC Value*/
unsigned int adc_value[2] = {0,0};

// Moving Average
unsigned int q0data[MOVEING_SIZE];
unsigned int q1data[MOVEING_SIZE];
unsigned int adc_mavg[2] = {0,0};

/* Function Prototypes here!! ********************************************************************/
void delay_ms(unsigned int);												/*For Main Software*/
unsigned int MovingAverage(unsigned int * , unsigned int );
void UART1Init(void);														/*UART Module*/
void UART1Send(unsigned char);
void UART1SendText(char[]);
void UART1SendStrNumLine(char[], unsigned int);
void AdcInitV2(void);														/*Adc Module*/
unsigned int ADC2SpwmPeriod(unsigned int);
void Timer1Interrupt(unsigned char);										/*Timer Module*/
void ExtiInterrupt(void);													/*External Interrupt*/
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void);		/*ISR*/
/*void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void);*/
/*void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void);*/

void GetAdc(unsigned int * pt_adc){
	unsigned char i=0;
	ADCON1bits.SAMP = 1;                    		/*Start Sampling*/
	ConvertADC10();                 				/*Convert to Digital 10 bits*/     
	for(i=0;i<=1;i++){
		*(pt_adc+i) = ReadADC10(i+1);
	}	
}
/**************************************************************************************************
* Function Name: Main
* Output Variable:
* Input Variable:
* Note:
* Version:
* First written : Sarucha Yanyong
**************************************************************************************************/
int main(void){
	/* 	
	*	Initial I/O
	*  	TRISX: Output is 0b0, Input is 0b1 for earch bit.
	*/
	TRISD = 0x0003;
	LATD = 0x0000;
	TRISE = 0x0000;
	LATE = 0x000F;
	TRISB = 0x000E;									/*Reserving RB1 for LED Output*/
	LATB = 0x0000;	

	/*
	*	Initial Module
	*/
	UART1Init();
	AdcInitV2();
	UART1SendStrNumLine("START",1234);

	/*
	*	Initial Queue Module
	*/
	for(mcnt=0;mcnt<=(MOVEING_SIZE-1);mcnt++){
		q0data[mcnt] = 0;
		q1data[mcnt] = 0;
	}
	while (1) 
	{
		/*
		*
		*	Start engine
		*
		*/

		/*Waiting for BT_START*/
		mcnt = 0;
		while(!BT_START){								/*If BT_START is True, Exiting the while loop*/
			if(mcnt++ <= 5){
				LED = 1;
			}else if((mcnt >5)&&(mcnt<=50)){
				LED = 0;
			}else if(mcnt > 50){
				mcnt = 0;
			}
			GetAdc(adc_value);
			for(mcnt2=0;mcnt2<=1;mcnt2++){
				sprintf(serial_buffer,"ADC%d=%d, ", mcnt2+1, adc_value[mcnt2]);
				UART1SendText(serial_buffer);
			}	
			UART1SendText(";\n\r");
			delay_ms(10);
		}
		/*
		*	Start SPWM Softly....
		*/
		
		UART1SendText("Soft-start;\n\r");
		/*1. Initial*/
		for(mcnt=0;mcnt<=MOVEING_SIZE;mcnt++){			/*Adding AdcValue for initial queue.*/
			GetAdc(adc_value);
			adc_mavg[0] = MovingAverage(q0data,adc_value[0]);
			adc_mavg[1] = MovingAverage(q0data,adc_value[1]);
		}
		initial_flag = 1;								/*Set Initial Flag*/
		LED = 0b1;										/*For show initial state*/
		spwm_sin_amp = 10;								/*Start Up Sin Amplitude*/
		PR1 = 160;										/*Default PR1*/

		/*2. Preparing for Soft-start process*/
		first_spwm_sin_amp = adc_mavg[0]/11;			/*Adding adc(an1) to be the target of increasing amplitude*/
		PR1 = ADC2SpwmPeriod(adc_mavg[1]);				/*Adding adc(an2) to adjust PR1*/

		/*3. Creating SawTooth Signal before start*/
		SawToothGen(tri_table, 640, 32);				/*Refer to spwm.h*/

		/*4. Start Timer*/
		Timer1Interrupt(1);								/*Timer Start!!!!!!*/
		delay_ms(100);

		/*5. Set Relay*/
		RELAY_PIN = 1;
		
		/*6. Soft-start*/
		for(mcnt = spwm_sin_amp;mcnt <= first_spwm_sin_amp;mcnt+=2){
			spwm_sin_amp = mcnt;
			UART1SendStrNumLine("COUNT",mcnt);
			UART1SendText(";\n\r");
			delay_ms(50);
		}
		initial_flag = 0;								/*Clear Initial lag*/

		/*
		*	Operating Loop
		*/
		while(1){										

			/*Get Adc Value*/
			ADCON1bits.SAMP = 1;                    	/*Start Sampling*/
			ConvertADC10();                 			/*Convert to Digital 10 bits*/     
			for(mcnt=1;mcnt<=2;mcnt++){
				adc_value[mcnt-1] = ReadADC10(mcnt);
				sprintf(serial_buffer,"ADC%d=%d, ", mcnt, adc_value[mcnt-1]);
				UART1SendText(serial_buffer);
			}
			
			/*Demo Adjusting Sin Wave Amplitude*/
			adc_mavg[0] = MovingAverage(q0data,adc_value[0]);
			spwm_sin_amp = adc_mavg[0]/11;

			/*Demo Adjusting Frequency*/
			adc_mavg[1] = MovingAverage(q1data,adc_value[1]);

			/*Updating period*/
			PR1 = ADC2SpwmPeriod(adc_mavg[1]);			/* I don't know why I can't put var to PR1 directly*/

			/*Debugging : Send adc_mavg*/
			serial_buffer[0]='\0';
			sprintf(serial_buffer,"mavg0=%d(Adj), ", adc_mavg[0]);
			UART1SendText(serial_buffer);

			serial_buffer[0]='\0';
			sprintf(serial_buffer,"mavg1=%d;", adc_mavg[1]);
			UART1SendText(serial_buffer);

			/*Debugging : End*/
			UART1SendText(";\n\r");

			/*Sampling Times (Soft)*/
			delay_ms(10);
			
			/*
			*	Check stop button to stop engine.
			*/
			if(!BT_START){								/*If BT_START is False, Breaking the while loop*/
				initial_flag = 1;								/*Set Initial Flag*/
				LED = 1;
				RELAY_PIN = 0;									/*2. Clear Relay*/
				for(mcnt = spwm_sin_amp;mcnt >= 10;mcnt-=1){	/*1. Soft-Stop*/
					spwm_sin_amp = mcnt;
					UART1SendStrNumLine("COUNT",mcnt);
					UART1SendText(";\n\r");
					delay_ms(50);
				}
				Timer1Interrupt(0);								/*3. Stop Timer*/
				SPWM_GATE1 = 0;									/*4. Clear all drive signal*/
				SPWM_GATE2 = 0;
				initial_flag = 0;								/*Set Initial Flag*/
				break;
			}
		}
	}
}

/* User Function *********************************************************************************/
/**************************************************************************************************
* Function Name: Delay Milisecond
* Propose: 
* Output Variable:
* Input Variable: ms
* Reference:
* Note: (F_MCY*0.0001) 1ms = 14746 nMCY
* First written : Sarucha Yanyong
**************************************************************************************************/
void delay_ms(unsigned int ms)
{
	unsigned int i;
	while(ms--){
    	for (i=0; i < 1400; i++){ // Correct delay_ms update 2014 05 22 15-13 //1227
    		Nop();    			  // Delay 1 MCY   									
		}
	}
}
/**************************************************************************************************
* Function Name: AdcInitV2
* Output Variable:
* Input Variable:
* Note:
* Version:
* First written : Sarucha Yanyong
**************************************************************************************************/
void AdcInitV2(void){
	unsigned int Channel, PinConfig, Scanselect, Adcon3_reg, Adcon2_reg,Adcon1_reg;
	ADCON1bits.ADON = 0;                    	// Turn off ADC     
	Channel =   	ADC_CH0_POS_SAMPLEA_AN0 &	// Channel 0 positive input select AN0
                	ADC_CH0_POS_SAMPLEA_AN1 & 	// Channel 0 positive input select AN1
                	ADC_CH0_POS_SAMPLEA_AN2 & 	// Channel 0 positive input select AN2
                	ADC_CH0_POS_SAMPLEA_AN3 & 	// Channel 0 positive input select AN3           
                	ADC_CH0_NEG_SAMPLEA_NVREF ; // Channel 0 negative VREF
	SetChanADC10(Channel);                  	// Set channel configuration             
	ConfigIntADC10(ADC_INT_DISABLE);            // Disable interrupt for ADC
	PinConfig =		ENABLE_AN0_ANA &            // Enable AN0-AN3 analog port
	            	ENABLE_AN1_ANA &
	            	ENABLE_AN2_ANA &
	            	ENABLE_AN3_ANA ;                                          
	Scanselect= 	SKIP_SCAN_AN4 &             // Scan for AN0-AN3
	            	SKIP_SCAN_AN5 &
	            	SKIP_SCAN_AN6 &
	            	SKIP_SCAN_AN7;                                                              
	Adcon3_reg = 	ADC_SAMPLE_TIME_10 &        // Sample for 10 time
	             	ADC_CONV_CLK_INTERNAL_RC &  // Internal Clock
	             	ADC_CONV_CLK_5Tcy; 
	Adcon2_reg =	ADC_VREF_AVDD_AVSS &        // Vref at Vdd and Vss
	                ADC_SCAN_ON &               // Enable scan for ADC
	                ADC_ALT_BUF_OFF &           // Disable alternate buffer
	                ADC_ALT_INPUT_OFF &         // Disable alternate input
	                ADC_CONVERT_CH0&            // Select CH0 convert         
	                ADC_SAMPLES_PER_INT_16;     // 16 sample between interrupt
	Adcon1_reg =    ADC_MODULE_ON &             // Enable module ADC
	                ADC_IDLE_CONTINUE &         // ADC run on idle mode
	               	ADC_FORMAT_INTG &           // Output value integer format
	                ADC_CLK_MANUAL &            // ADC manual clock
	                ADC_SAMPLE_SIMULTANEOUS &   // ADC sampling simultaneous
	                ADC_AUTO_SAMPLING_ON;       // ADC auto sampling                        
	OpenADC10(Adcon1_reg, Adcon2_reg, Adcon3_reg, PinConfig, Scanselect);	// Turn on ADC module     
}
/**************************************************************************************************
* Function Name: 
* Propose: 
* Output Variable:
* Input Variable:
* Reference: 
* Note: 
* First written : 
**************************************************************************************************/
void Timer1Interrupt(unsigned char enable){
	/*
	*  Dead time 2 us
	*  648
	*  7.52us = 220
	*  5.44us = 160 Minimum
	*  5.04us = 148 Min
	*  3.20us = 95
	*/
	PR1 = 160;					/*58 = 250KHz, 4us 95 = 150kH, 230->Tri 13k*/ 
	T1CONbits.TCKPS = 0b00; 	// Prescale	

	IFS0bits.T1IF = 1; 			//Timer1 Interrupt Flag Status bit
	IEC0bits.T1IE = 1; 			//Timer1 Interrupt Enable bit
	IPC0bits.T1IP = 0b111; 		//Timer1 Interrupt Priority bits (111=highest)

	T1CONbits.TON = enable;
}
/**************************************************************************************************
* Function Name: 
* Propose: 
* Output Variable:
* Input Variable:
* Reference: 
* Note: 
* First written : 
**************************************************************************************************/
void Timer2Interrupt(void){
	PR2 = 58;	/*250KHz, 4us*/
	T2CONbits.TCKPS = 0b00; // Prescale	

	IFS0bits.T2IF = 1; //Timer1 Interrupt Flag Status bit
	IEC0bits.T2IE = 1; //Timer1 Interrupt Enable bit
	IPC1bits.T2IP = 0b110; // Timer1 Interrupt Priority bits (111=highest)

	T2CONbits.TON = 0;
}

/**************************************************************************************************
* Function Name: External Interrupt Initial
* Propose: 
* Output Variable:
* Input Variable:
* Reference: 
* Note: 
* First written : 
**************************************************************************************************/
void ExtiInterrupt(void){
	//INTCON2 = 0x001E;       /*Setup INT1, INT2, INT3 & INT4 pins to interupt */
	                        /*on falling edge and set up INT0 pin to interupt */
	                        /*on rising edge */
	// IFS0bits.INT0IF = 0;    /*Reset INT0 interrupt flag */
	// IEC0bits.INT0IE = 1;    /*Enable INT0 Interrupt Service Routine */

	// IFS1bits.INT1IF = 0;    /*Reset INT1 interrupt flag */
	// IEC1bits.INT1IE = 1;    /*Enable INT1 Interrupt Service Routine */

	// IFS1bits.INT2IF = 0;    /*Reset INT0 interrupt flag */
	// IEC1bits.INT2IE = 1;    /*Enable INT0 Interrupt Service Routine */

	// IFS3bits.INT3IF = 0;    /*Reset INT1 interrupt flag */
	// IEC3bits.INT3IE = 1;    /*Enable INT1 Interrupt Service Routine */

	// IFS3bits.INT4IF = 0;    /*Reset INT1 interrupt flag */
	// IEC3bits.INT4IE = 1;    /*Enable INT1 Interrupt Service Routine */
}
/**************************************************************************************************
* Function Name: Moving Average
* Propose: 
* Output Variable:
* Input Variable:
* Reference: 
* Note:
* First written : Sarucha Yanyong
**************************************************************************************************/
void UART1Init(void){
	U1BRG = 97;					//24; // 19200
    U1MODEbits.PDSEL = 0;   	// 8 bits no parity  
    U1MODEbits.STSEL = 0;   	// 1 stop bit  
    U1MODEbits.LPBACK = 0;  	// loopback off  
    U1MODEbits.ALTIO  = 1;  	// alternate tx and rx, useful as we can leave the debugger on while doing comms  
    U1MODEbits.UARTEN = 1;  	// uart on  
    U1MODEbits.ABAUD = 0;   	// auto baud off  
    U1STAbits.UTXBRK = 0;   	// no pause  
    U1STAbits.UTXEN   = 1;  	// Enable Transmitter

    IFS0bits.U1RXIF = 0b0;		//UxRXIF
    IEC0bits.U1RXIE = 0b0;
    IPC2bits.U1RXIP = 0b110; 	// Setting lower than TimerInterrupt.

    U1STAbits.URXISEL = 0b01; 	//URXISEL<1:0> (UxSTA<7:6>)
}
/**************************************************************************************************
* Function Name: UART1 All In One Module
* Propose: 
* Output Variable:
* Input Variable:
* Reference: 
* Note:
* First written : Sarucha Yanyong
**************************************************************************************************/
void UART1Send(unsigned char data){
     while(U1STAbits.UTXBF);
     U1TXREG = data&0x00FF;						
}
void UART1SendStrNumLine(char str[], unsigned int value){
	unsigned char conv_str[4];
	conv_str[0] = (value/1000);
	conv_str[1] = (value%1000)/100;
	conv_str[2] = (value%100)/10;
	conv_str[3] = (value%10);

	unsigned char i=0;

	while(str[i]!='\0'){
     	UART1Send(str[i]);
     	i++;
    }
	UART1Send(':');
	for(i=0;i<=3;i++){
		UART1Send(conv_str[i]+48);
	}
    UART1Send('\n');
    UART1Send('\r');
}
void UART1SendLine(char data[]){
	unsigned char i=0;
	while(data[i]!='\0'){
     	UART1Send(data[i]);
     	i++;
    }
    UART1Send('\n');
    UART1Send('\r');
}
void UART1SendText(char data[]){
	unsigned char i=0;
	while(data[i]!='\0'){
     	UART1Send(data[i]);
     	i++;
    }
}
/**************************************************************************************************
* Function Name: UART1 Spesific for C#
* Propose: 
* Output Variable:
* Input Variable:
* Reference: 
* Note:
* First written : Sarucha Yanyong
**************************************************************************************************/
void UART1SendRegData(unsigned char reg, unsigned int value){
	unsigned char conv_str[4];
	conv_str[0] = (value/1000);
	conv_str[1] = (value%1000)/100;
	conv_str[2] = (value%100)/10;
	conv_str[3] = (value%10);
	UART1Send(reg); // A
	UART1Send(':');
	UART1Send(conv_str[0]+48);
	UART1Send(conv_str[1]+48);
	UART1Send(conv_str[2]+48);
	UART1Send(conv_str[3]+48);
	//UART1Send('#'); // A
	UART1Send(0x0D); // \n
	UART1Send(0x0A); // \n
}
/**************************************************************************************************
* Function Name: Converting String to Integer
* Propose: 
* Output Variable:
* Input Variable:
* Reference: 
* Note:
* First written : Sarucha Yanyong
**************************************************************************************************/
unsigned int String2Num(unsigned char * chr){
	unsigned int num = 0 ;
	num = (*(chr)-48)*1000;
	num += (*(chr+1)-48)*100;
	num += (*(chr+2)-48)*10;
	num += (*(chr+3)-48)*1;
	return num*1;
}
/**************************************************************************************************
* Function Name: Moving Average
* Propose: 
* Output Variable:
* Input Variable:
* Reference: 
* Note:
* First written : Sarucha Yanyong
**************************************************************************************************/
unsigned int MovingAverage(unsigned int * pt_qxdata, unsigned int new_value){
	unsigned char i = 0;
	// Moving Average Algorithm
	for (i=0 ; i<(MOVEING_SIZE - 1) ; i++){	// Shift Array [0<1]
		*(pt_qxdata + i) = *(pt_qxdata + i + 1);
	}
	*(pt_qxdata + MOVEING_SIZE - 1) = new_value;			// Adding New Value to Queue
	unsigned int sum_data = 0;             				// Average all data
	for (i=0 ; i<MOVEING_SIZE ; i++){
		sum_data += *(pt_qxdata + i);
	}	
	//return sum_data/MOVEING_SIZE;
	return __builtin_divud(sum_data, MOVEING_SIZE);	/*Divided Refer to dsp function*/
}
/**************************************************************************************************
* Function Name: ADC to adjust period (PR1) for SPWM
* Propose: 
* Output Variable:
* Input Variable:
* Reference: 
* Note:
* First written : Sarucha Yanyong
**************************************************************************************************/
unsigned int ADC2SpwmPeriod(unsigned int adc){
	if(adc < 50)
		adc = 50;
	else if(adc > 1000)
		adc = 1000;
	return (0.5263*(adc-50)+160);
}
/**************************************************************************************************
* Function Name: T1 Interrupt service routine for generate SPWM signal
* Propose: 
* Output Variable: 
* Input Variable:
* Reference: Build in DSP library
* Note:
* First written : Sarucha Yanyong
**************************************************************************************************/
unsigned int tr1_loop_count = 0;
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
    _T1IF = 0;					/*Clear Timer 1 interrupt flag*/
    /*
    *	For Testing System Alive!!!
    */
    if(!initial_flag){
		if(++tr1_loop_count >= 10000){
			LED = !LED;
			tr1_loop_count = 0;
		}
	}

    /*
    *	Calling SPWM Function
    *	Refer to spwm.h
    */
    SPWM_OUT = 1;				/*Ping*/
	DemoSpwmGenHalf(&spwm_kk, sin_table, tri_table, spwm_sin_amp);
;	SPWM_OUT = 0;				/*Pong*/
}

/**************************************************************************************************
* Function Name: INT0 Interrupt service routine
* Propose: 
* Output Variable: 
* Input Variable:
* Reference: 
* Note: _INT0Interrupt() is the INT0 interrupt service routine (ISR).
		The routine must have global scope in order to be an ISR.
		The ISR name is chosen from the device linker script.
* First written : 
**************************************************************************************************/
void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(void)
{

        IFS0bits.INT0IF = 0;    //Clear the INT0 interrupt flag or else
                                //the CPU will keep vectoring back to the ISR
}
/**************************************************************************************************
* Function Name: U1RXInterrupt Service routine.
* Propose: 
* Output Variable:
* Input Variable:
* Reference: 
* Note:
* First written : Sarucha Yanyong
**************************************************************************************************/
void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void){
}
