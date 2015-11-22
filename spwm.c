/**************************************************************************************************
* Name: SPWM Generator
* File Type: Source
* Version: rev3
* Note: 
* First written: Sarucha Yanyong
**************************************************************************************************/
/* Include ***************************************************************************************/
#include "spwm.h"

/**************************************************************************************************
* Function Name:
* Output Variable:
* Input Variable:
* Reference:
* Note:
* First written : 
**************************************************************************************************/
unsigned int DemoFunc (unsigned int imp){
	return (imp*4);
}
/**************************************************************************************************
* Function Name: Triangle Generator
* Output Variable:
* Input Variable:
* Reference:
* Note:             Generate from [0 1 2 3 4 [5] 4 3 2 1] the next loop will start at [0]
* First written : Sarucha Yanyong
**************************************************************************************************/
void DemoTriGen(unsigned int*pt){
    unsigned char i;
    unsigned int trisig = 0;
    unsigned char trislop = 0;
    for(i=0;i < SPWM_TRISIZE;i++){
        *(pt+i) = trisig;
        if(trisig >= SPWM_TRIMAX){                              /*Changing Triangle Direction*/
            trislop = 1;                                        /*Go Down*/
        }else if(trisig <= 0){
            trislop = 0;
        }
        if(trislop == 0){                                       /*Generating Triangle Wave Value*/
            trisig += SPWM_TRIINC;                  /*For adjusting Freq*/
        }else{
            trisig -= SPWM_TRIINC;
        }
    }
}
/**************************************************************************************************
* Function Name: Sawtooth Generator
* Propose: 
* Output Variable:  
* Input Variable:   pt is output array signal.
*                   max is maximum SawTooth signal.
*                   step is step size for increasing signal.
* Reference:
* Note:             pt[size], Array Size = (max/step+1), +1 mean keep 0 at 1st array.
*                   pt[size]=[0:step:max];
*                   Sample = [0 2 4 6 8 10], max=10, step=2 and array size=(10/2+1)=6
* First written : Sarucha Yanyong
**************************************************************************************************/
void SawToothGen(unsigned int*pt, const unsigned int max, const unsigned char step){
    unsigned char i;
    unsigned int saw_sig = 0;
    for(i=0;saw_sig <= max;i++){
        *(pt+i) = saw_sig;
        saw_sig += step;         
    }
}
/**************************************************************************************************
* Function Name: Generate SPWM signal Rev 3 Half Bridge
* Propose: 
* Output Variable: SPWM_GATE1 is used for PWM output signal.
* Input Variable:
* Reference: Build in DSP library
* Note: You have to put it into Interrupt.
* Version: 2014-07-17 22-23
* First written : Sarucha Yanyong
**************************************************************************************************/
void DemoSpwmGenHalf(saw_spwm * sp, const int * sin_lookup, unsigned int * tri_lookup, unsigned int s_amp){
    /*Generating Triangle*/
    /*Triangle is Ok! Checking on 2014-07-16 21-25*/
    sp->trisig = *(tri_lookup + sp->triptr);                      
    if(++sp->triptr > (SPWM_SAWSIZE-1)){                      
        sp->triptr = 0; 
    }
    /*Getting Sine from table*/
    /* 
        sin wave generator is independent from other signal because sine wave pointer is counting itself.
    */
    sp->sinsig = (( *(sin_lookup + sp->sinptr) )*((int)sp->sin_sig_temp))/100 + 320;              /*Adjusting Sine Amplitude*/
    if(++sp->sinptr > SPWM_SINMAXPTR){                          /*SIN WAVE COUNTER*/
        sp->sinptr = 0;                                         /*Reset pointer for create Infinity Sine Wave*/
        sp->sin_sig_temp = s_amp; 
    }

    /*Driving Signal Phase 1 : P1ST-P1ED*/
    if(sp->sinsig >= sp->trisig){                          /*TriAngle and Sinusoid Comparision*/
        SPWM_GATE1 = 1;
    }else{
        SPWM_GATE1 = 0;
    }

    /*Period Pointer*/          
    if(++sp->period > 1999){   /*COUNTER*/
        sp->period = 0;             /*Reset period pointer*/
   }
}
