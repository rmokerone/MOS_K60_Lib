/**********************************************************************
	> File Name: hw_uart.h
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Tue 18 Mar 2014 10:58:49 PM CST
 *********************************************************************/

#ifndef __HW_UART_H_
#define __HW_UART_H_

#include <stdio.h>

#define UART0 UART0_BASE_PTR
#define UART1 UART1_BASE_PTR
#define UART2 UART2_BASE_PTR
#define UART3 UART3_BASE_PTR
#define UART4 UART4_BASE_PTR
#define UART5 UART5_BASE_PTR

//UART模块中断回调函数类型
typedef void (*UART_ISR_CALLBACK)(void);

//UART模块初始化结构体，用于配置UART各项参数
typedef struct
{
  /*
    描述：
      选择UARTx
    取值：
      UART0~UART5
    初始化：
      不必须初始化，默认值UART5
  */
    UART_MemMapPtr UART_Uartx;        
      
  /*
    描述：
      设置UARTx波特率
    推荐取值：
      4800、9600、19200、38400、115200
    初始化：
      不必须初始化，默认值9600
  */
  uint32 UART_BaudRate;  
  
  /*
    描述：
      发送引脚，每个UARTx对应不同的TxPin
    取值：
      UART0-PTA2、PTA14、*PTB17
      UART1-PTE0、*PTC4
      UART2-*PTD3
      UART3-PTE4、PTB11、*PTC17
      UART4-PTE24、*PTC15
      UART5-PTD9、*PTE8
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type UART_TxPin;   
  
  /*
    描述：
      接收引脚，每个UARTx对应不同的RxPin
    取值：
      UART0-PTA1、PTA15、*PTB16
      UART1-PTE1、*PTC3
      UART2-*PTD2
      UART3-PTE5、PTB10、*PTC16
      UART4-PTE25、*PTC14
      UART5-PTD8、*PTE9
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type UART_RxPin; 
  
  /*
    描述：
      使能接收中断
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认值FALSE
  */
  boolean UART_RxIntEnable;
  
  /*
    描述：
      使能发送中断
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认值FALSE
  */
  boolean UART_TxIntEnable;
  
  /*
    描述：
      接收中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  UART_ISR_CALLBACK UART_RxIsr; 
  
  /*
    描述：
      发送中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  UART_ISR_CALLBACK UART_TxIsr; 
} UART_InitTypeDef;

//初始化uart通道、波特率、发送接收引脚
void LPLD_UART_Init(UART_InitTypeDef);
//uart获取一个字符
int8 LPLD_UART_GetChar(UART_MemMapPtr);
//检查是否接收到字符
int32 LPLD_UART_GetChar_Present(UART_MemMapPtr);
//uart发送一个字符
void LPLD_UART_PutChar(UART_MemMapPtr, int8);
//uart发送字符数组
void LPLD_UART_PutCharArr(UART_MemMapPtr, char *, int32);
//uart中断使能
void LPLD_UART_EnableIrq(UART_InitTypeDef);
//uart中断禁用
void LPLD_UART_DisableIrq(UART_InitTypeDef);

#endif
