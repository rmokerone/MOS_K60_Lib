/**********************************************************************
	> File Name: hw_pit.h
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Sun 23 Mar 2014 11:39:47 AM CST
 *********************************************************************/

#ifndef __HW_PIT_H__
#define __HW_PIT_H__

#define PIT     PIT_BASE_PTR
//PIT模块中断回调函数类型
typedef void (*PIT_ISR_CALLBACK)(void);

//PIT模块号定义
typedef enum PITx
{
  PIT0=0,
  PIT1=1,
  PIT2=2,
  PIT3=3
}PITx;

//PIT模块初始化结构体，用于配置PIT各项参数
typedef struct
{  
  /*
    描述：
      选择PITx
    取值：
      PIT0、PIT1、PIT2、PIT3
    初始化：
      必须初始化
  */
  PITx PIT_Pitx;
  
  /*
    描述：
      PITx模块的周期，单位us
    取值：
      1~0xFFFFFFFF
    初始化：
      不必须初始化，PIT_PeriodUs、PIT_PeriodMs、PIT_PeriodS至少一个初始化
  */
  uint32 PIT_PeriodUs;
  
  /*
    描述：
      PITx模块的周期，单位ms
    取值：
      1~0xFFFFFFFF
    初始化：
      不必须初始化，PIT_PeriodUs、PIT_PeriodMs、PIT_PeriodS至少一个初始化
  */
  uint32 PIT_PeriodMs;
  
  /*
    描述：
      PITx模块的周期，单位s
    取值：
      1~0xFFFFFFFF
    初始化：
      不必须初始化，PIT_PeriodUs、PIT_PeriodMs、PIT_PeriodS至少一个初始化
  */
  uint32 PIT_PeriodS;

  /*
    描述：
      PITx中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  PIT_ISR_CALLBACK PIT_Isr; 
} PIT_InitTypeDef;

//PIT通用初始化函数，选择PITx、配置中断周期、中断函数
uint8 LPLD_PIT_Init(PIT_InitTypeDef);
//PIT反初始化函数，关闭选择的PITx
void LPLD_PIT_Deinit(PIT_InitTypeDef);
//使能PITx中断
void LPLD_PIT_EnableIrq(PIT_InitTypeDef);
//禁用PITx中断
void LPLD_PIT_DisableIrq(PIT_InitTypeDef);

//pit开始记时
void LPLD_PIT_TimeStart (PITx PIT_pitx);
//获取PIT计时时间，（超时关闭定时器）
uint32 LPLD_PIT_TimeGetUs (PITx PIT_pitx);
//关闭PIT计时
void LPLD_PIT_TimeClose(PITx PIT_pitx);

#endif
