/**********************************************************************
	> File Name: hw_ftm.h
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Sat 22 Mar 2014 03:16:59 PM CST
 *********************************************************************/

#ifndef __HW_FTM_H__
#define __HW_FTM_H__
/********************************************************************/

//宏定义FTM模块
#define FTM0    FTM0_BASE_PTR
#define FTM1    FTM1_BASE_PTR
#define FTM2    FTM2_BASE_PTR
#define FTM3    FTM3_BASE_PTR

//FTM通道枚举定义
typedef enum FtmChnEnum
{
  FTM_Ch0       =0,
  FTM_Ch1       =1,
  FTM_Ch2       =2,
  FTM_Ch3       =3,
  FTM_Ch4       =4,
  FTM_Ch5       =5,
  FTM_Ch6       =6,
  FTM_Ch7       =7,
  FTM_PhA       =8,
  FTM_PhB       =9
} FtmChnEnum_Type;

#define FTM_MODE_PWM    0x01    //PWM输出模式
#define FTM_MODE_IC     0x02    //输入捕获模式
#define FTM_MODE_QD     0x04    //正交解码模式
#define FTM_MODE_DEC    0x08    //双边缘捕获模式

#define FTM_CLK_DIV1    0       //FTM CLK分频系数1
#define FTM_CLK_DIV2    1       //FTM CLK分频系数2
#define FTM_CLK_DIV4    2       //FTM CLK分频系数4
#define FTM_CLK_DIV8    3       //FTM CLK分频系数8
#define FTM_CLK_DIV16   4       //FTM CLK分频系数16
#define FTM_CLK_DIV32   5       //FTM CLK分频系数32
#define FTM_CLK_DIV64   6       //FTM CLK分频系数64
#define FTM_CLK_DIV128  7       //FTM CLK分频系数128

#define ALIGN_LEFT  (FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK)    //PWM左对齐
#define ALIGN_RIGHT (FTM_CnSC_MSB_MASK|FTM_CnSC_ELSA_MASK)    //PWM左对齐

#define DEADTIME_CH01   FTM_COMBINE_DTEN0_MASK  //FTM_Ch0\FTM_Ch1插入死区
#define DEADTIME_CH23   FTM_COMBINE_DTEN1_MASK  //FTM_Ch0\FTM_Ch1插入死区
#define DEADTIME_CH45   FTM_COMBINE_DTEN2_MASK  //FTM_Ch0\FTM_Ch1插入死区
#define DEADTIME_CH67   FTM_COMBINE_DTEN3_MASK  //FTM_Ch0\FTM_Ch1插入死区

#define DEADTIME_DIV1   0    //PWM死区CLK分频系数1
#define DEADTIME_DIV4   1    //PWM死区CLK分频系数4
#define DEADTIME_DIV16  3    //PWM死区CLK分频系数16

#define CAPTURE_RI      FTM_CnSC_ELSA_MASK      //上升沿捕获
#define CAPTURE_FA      FTM_CnSC_ELSB_MASK      //下降沿捕获
#define CAPTURE_RIFA    (FTM_CnSC_ELSA_MASK|FTM_CnSC_ELSB_MASK)      //上升下降沿捕获

#define QD_MODE_PHAB    0                               //AB相解码模式
#define QD_MODE_CNTDIR  FTM_QDCTRL_QUADMODE_MASK        //计数和方向解码模式

//FTM模块中断回调函数类型
typedef void (*FTM_ISR_CALLBACK)(void);

//FTM模块初始化结构体，用于配置FTM各项参数
typedef struct
{
  /*
    描述：
      选择FTMx
    取值：
      FTM0、FTM1、FTM2
    初始化：
      必须初始化
  */
  FTM_MemMapPtr FTM_Ftmx;
  
  /*
    描述：
      配置FTMx的工作模式
    取值：
      FTM_MODE_PWM-PWM输出模式
      FTM_MODE_IC-输入捕获模式
      FTM_MODE_QD-正交解码模式
    初始化：
      必须初始化
  */
  uint8 FTM_Mode;
  
  /*
    描述：
      配置PWM模式下的PWM输出频率，单位Hz
    取值：
      1~0xFFFFFFFF
    初始化：
      FTM_MODE_PWM模式下必须初始化
  */
  uint32 FTM_PwmFreq;
  
  /*
    描述：
      配置PWM模式下的相应通道对的死区功能
    取值：  
      DEADTIME_CH01-通道0和通道1插入死区
      DEADTIME_CH23-通道2和通道3插入死区
      DEADTIME_CH45-通道4和通道5插入死区
      DEADTIME_CH67-通道6和通道7插入死区
    初始化：
      不须初始化，默认不插入死区
  */
  uint32 FTM_PwmDeadtimeCfg;
  
  /*
    描述：
      PWM模式下死区功能对总线时钟的分频系数
    取值：  
      DEADTIME_DIV1-总线1分频
      DEADTIME_DIV4-总线4分频
      DEADTIME_DIV16-总线16分频
    初始化：
      不须初始化
  */
  uint8 FTM_PwmDeadtimeDiv;

  /*
    描述：
      PWM模式下死区功能插入的个数
    取值：  
      0~63，0为不插入死区
    初始化：
      不须初始化
  */
  uint8 FTM_PwmDeadtimeVal;
  
  /*
    描述：
      正交解码的解码模式
    取值：  
      QD_MODE_PHAB-AB相解码模式
      QD_MODE_CNTDIR-计数和方向解码模式
    初始化：
      不须初始化，默认AB相解码模式
  */
  uint8 FTM_QdMode;
  
  /*
    描述：
      FTMx时钟相对总线时钟的分频系数
    取值：  
      FTM_CLK_DIV1-FTM CLK分频系数1
      FTM_CLK_DIV2-FTM CLK分频系数2
      FTM_CLK_DIV4-FTM CLK分频系数4
      FTM_CLK_DIV8-FTM CLK分频系数8
      FTM_CLK_DIV16-FTM CLK分频系数16
      FTM_CLK_DIV32-FTM CLK分频系数32
      FTM_CLK_DIV64-FTM CLK分频系数64
      FTM_CLK_DIV128-FTM CLK分频系数128
    初始化：
      FTM_MODE_IC模式下须初始化
  */
  uint8 FTM_ClkDiv;
  
  /*
    描述：
      使能计数器溢出中断
    取值：  
      TRUE、FALSE
    初始化：
      不须初始化
  */
  boolean FTM_ToiEnable;
  
  /*
    描述：
      FTMx中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  FTM_ISR_CALLBACK FTM_Isr; 
  
} FTM_InitTypeDef;

//FTM模块通用初始化，可初始化为PWM、输入捕获、正交解码、双边捕获模式
uint8 LPLD_FTM_Init(FTM_InitTypeDef);
//FTM模块通用反初始化
uint8 LPLD_FTM_Deinit(FTM_InitTypeDef);
//FTM模块PWM模式输出使能，配置输出通道、占空比、指定对应的引脚、对齐方式
uint8 LPLD_FTM_PWM_Enable(FTM_MemMapPtr, FtmChnEnum_Type, uint32, PortPinsEnum_Type, uint8);
//变更FTM模块PWM模式指定通道的输出占空比
uint8 LPLD_FTM_PWM_ChangeDuty(FTM_MemMapPtr, FtmChnEnum_Type, uint32);
//FTM模块输入捕获模式输出使能，配置输入通道、指定对应的引脚、捕获边缘
uint8 LPLD_FTM_IC_Enable(FTM_MemMapPtr, FtmChnEnum_Type, PortPinsEnum_Type, uint8);
//禁用FTM模块指定通道的输出、输入
uint8 LPLD_FTM_DisableChn(FTM_MemMapPtr, FtmChnEnum_Type);
//FTM模块正交解码模式输入使能，配置输入通道PHA和PHB
uint8 LPLD_FTM_QD_Enable(FTM_MemMapPtr, PortPinsEnum_Type, PortPinsEnum_Type);
//FTM模块正交解码模式输入禁用
uint8 LPLD_FTM_QD_Disable(FTM_MemMapPtr);

//判断FTMx是否产生计数溢出中断标志
boolean LPLD_FTM_IsTOF(FTM_MemMapPtr);
//清除FTMx计数器溢出中断标志
void LPLD_FTM_ClearTOF(FTM_MemMapPtr);
//判断通道n是否产生中断标志
boolean LPLD_FTM_IsCHnF(FTM_MemMapPtr, FtmChnEnum_Type);
//清除通道n中断标志
void LPLD_FTM_ClearCHnF(FTM_MemMapPtr, FtmChnEnum_Type);
//获取FTMx通道n捕获的FTMx计数值
uint16 LPLD_FTM_GetChVal(FTM_MemMapPtr, FtmChnEnum_Type);
//获取FTMx时钟分频系数
uint8 LPLD_FTM_GetClkDiv(FTM_MemMapPtr);
//获取FTMx计数器值
uint16 LPLD_FTM_GetCounter(FTM_MemMapPtr);
//清空FTMx计数器
void LPLD_FTM_ClearCounter(FTM_MemMapPtr);

//使能FTMx中断
uint8 LPLD_FTM_EnableIrq(FTM_InitTypeDef);
//禁用FTMx中断
uint8 LPLD_FTM_DisableIrq(FTM_InitTypeDef);

#endif
