/**************************************************************************************************
* Name: SPWM Generator
* File Type: Header
* Version: rev3
* Note: 
* First written: Sarucha Yanyong
**************************************************************************************************/
/* Include ***************************************************************************************/
#include <p30F4011.h>

/* Definition ************************************************************************************/
#define SPWM_GATE1 			_LATE0		/*Output Define*/
//#define SPWM_GATE2 			_LATE0		/*Output Define*/
//#define SPWM_TRIOUT			_LATE2
#define SPWM_OUT 			_LATE1
#define SPWM_TRIMAX		640			/*Triangle Max Value*/
#define SPWM_TRIINC 	32			/*Triangle Increasing step*/
#define SPWM_SINMAXPTR	1999		/*Max data of sine wave in lookup table [0-3999]*/
#define SPWM_P1PTR		1999		/*Half-Period (Phase1) is [0-3999]*/
#define SPWM_P2PTR		2000
#define SPWM_P3PTR		4999
#define SPWM_MAXPTR 	4999		/*Max period*/
#define SPWM_DT			5			/*Dead time*/
#define SPWM_TRISIZE	40
#define SPWM_SAWSIZE	21			/*For SpwmGenHalf*/
/* Variable **************************************************************************************/
typedef struct{
	unsigned int 	trisig;
	unsigned char 	trislop;
	unsigned int 	sinsig;
	unsigned int 	sin_sig_temp;
	unsigned int 	sinptr;
	unsigned char 	flag;
	unsigned int 	period;
}stspwm;							/*Opterating structure for SpwmGen Function*/
typedef struct{
	unsigned int 	trisig;
	unsigned char 	triptr;
	unsigned int 	sinsig;
	unsigned int 	sin_sig_temp;
	unsigned int 	sinptr;
	unsigned char 	flag;
	unsigned int 	period;
}saw_spwm;							/*Opterating structure for DemoSpwmGenHalf Function*/

/* Function Prototypes ***************************************************************************/
// void DemoTriGen(unsigned int*);
void SawToothGen(unsigned int*, const unsigned int, const unsigned char);
// void DemoSpwmGen(stspwm*, const unsigned int *);
// void DemoSpwmGen2(stspwm*, const unsigned int *, unsigned int *);
void DemoSpwmGenHalf(saw_spwm*, const int *, unsigned int *, unsigned int);

