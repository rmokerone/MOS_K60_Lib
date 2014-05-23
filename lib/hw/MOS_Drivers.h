/**********************************************************************
	> File Name: MOS_Drivers.h
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Tue 18 Mar 2014 05:21:20 PM CST
 *********************************************************************/

#ifndef __MOS_DRIVERS_H__
#define __MOS_DRIVERS_H__

//底层库版本定义
#define MOS_Version "0.1"

//端口引脚枚举定义
typedef enum PortPinsEnum
{
  //PortA
  PTA0=0,    PTA1=1,    PTA2=2,    PTA3=3,    
  PTA4=4,    PTA5=5,    PTA6=6,    PTA7=7,    
  PTA8=8,    PTA9=9,    PTA10=10,  PTA11=11,
  PTA12=12,  PTA13=13,  PTA14=14,  PTA15=15,
  PTA16=16,  PTA17=17,  PTA18=18,  PTA19=19,
  /********* MK60DZ10无PTA20~23 ************/
  PTA24=24,  PTA25=25,  PTA26=26,  PTA27=27,
  PTA28=28,  PTA29=29,
  /********* MK60DZ10无PTA30~31 ************/
  
  //PortB
  PTB0=32,   PTB1=33,   PTB2=34,   PTB3=35,
  PTB4=36,   PTB5=37,   PTB6=38,   PTB7=39,
  PTB8=40,   PTB9=41,   PTB10=42,  PTB11=43,
  /********* MK60DZ10无PTB12~15 ************/
  PTB16=48,  PTB17=49,  PTB18=50,  PTB19=51,
  PTB20=52,  PTB21=53,  PTB22=54,  PTB23=55,
  /********* MK60DZ10无PTB24~27 ************/
  /********* MK60DZ10无PTB28~31 ************/
  
  //PortC
  PTC0=60,   PTC1=61,   PTC2=62,   PTC3=63,
  PTC4=64,   PTC5=65,   PTC6=66,   PTC7=67,
  PTC8=68,   PTC9=69,   PTC10=70,  PTC11=71,
  PTC12=72,  PTC13=73,  PTC14=74,  PTC15=75,
  PTC16=76,  PTC17=77,  PTC18=78,  PTC19=79,
  /********* MK60DZ10无PTC20~23 ************/
  /********* MK60DZ10无PTC24~27 ************/
  /********* MK60DZ10无PTC28~31 ************/
  
  //PortD
  PTD0=92,   PTD1=93,   PTD2=94,   PTD3=95,
  PTD4=96,   PTD5=97,   PTD6=98,   PTD7=99,
  PTD8=100,  PTD9=101,  PTD10=102, PTD11=103,
  PTD12=104, PTD13=105, PTD14=106, PTD15=107,
  /********* MK60DZ10无PTD16~19 ************/
  /********* MK60DZ10无PTD20~23 ************/
  /********* MK60DZ10无PTD24~27 ************/
  /********* MK60DZ10无PTD28~31 ************/
  
  //PortE
  PTE0=124,  PTE1=125,  PTE2=126,  PTE3=127,
  PTE4=128,  PTE5=129,  PTE6=130,  PTE7=131,
  PTE8=132,  PTE9=133,  PTE10=134, PTE11=135,
  PTE12=136,
  /********* MK60DZ10无PTE12~15 ************/
  /********* MK60DZ10无PTE16~19 ************/
  /********* MK60DZ10无PTE20~23 ************/
  PTE24=148, PTE25=149, PTE26=150, PTE27=151,
  PTE28=152
  /********* MK60DZ10无PTE29~31 ************/
} PortPinsEnum_Type;

//include base hardware drivers
#include "hw_gpio.h"
#include "hw_mcg.h"
#include "hw_uart.h"
#include "hw_adc.h"
#include "hw_ftm.h"
#include "hw_pit.h"
#include "hw_lptmr.h"
#include "hw_spi.h"
#include "hw_dma.h"
#include "hw_dac.h"
#include "hw_pdb.h"

//add OV7725 driver 
#include "dev_sccb.h"
//#include "dev_ov7725.h"

#endif
