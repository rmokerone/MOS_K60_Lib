/**********************************************************************
	> File Name: hw_gpio.h
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Tue 18 Mar 2014 11:23:44 AM CST
 *********************************************************************/

#ifndef __HW_GPIO_H__
#define __HW_GPIO_H__

#include "common.h"

#define PORTA   PORTA_BASE_PTR
#define PORTB   PORTB_BASE_PTR
#define PORTC   PORTC_BASE_PTR
#define PORTD   PORTD_BASE_PTR
#define PORTE   PORTE_BASE_PTR
#define PORTF   PORTF_BASE_PTR

//宏定义初始化函数用到的参数
//GPIO_InitTypeDef->GPIO_Dir
#define DIR_INPUT   0 //输入
#define DIR_OUTPUT  1 //输出
//GPIO_InitTypeDef->GPIO_OUTPUT
#define OUTPUT_L    0 //输出低电平
#define OUTPUT_H    1 //输出高电平
//GPIO_InitTypeDef->GPIO_PinControl
#define INPUT_PULL_DOWN (0|PORT_PCR_PE_MASK)                      //输入下拉
#define INPUT_PULL_UP   (PORT_PCR_PS_MASK|PORT_PCR_PE_MASK)       //输入上拉
#define INPUT_PULL_DIS  0                                       //禁用PULL
#define OUTPUT_SR_FAST  0                       //高压摆率
#define OUTPUT_SR_SLOW  PORT_PCR_SRE_MASK       //低压摆率
#define INPUT_PF_EN     PORT_PCR_PFE_MASK       //使能低通滤波器
#define INPUT_PF_DIS    0                       //禁用低通滤波器
#define OUTPUT_OD_EN    PORT_PCR_ODE_MASK       //输出开漏使能
#define OUTPUT_OD_DIS   0                       //输出开漏禁用
#define OUTPUT_DSH      PORT_PCR_DSE_MASK       //高驱动能力输出
#define OUTPUT_DSL      0                       //低驱动能力输出
#define IRQC_DIS        PORT_PCR_IRQC(0x00)     //禁用中断\请求
#define IRQC_DMARI      PORT_PCR_IRQC(0x01)     //上升沿产生DMA请求
#define IRQC_DMAFA      PORT_PCR_IRQC(0x02)     //下降沿产生DMA请求
#define IRQC_DMAET      PORT_PCR_IRQC(0x03)     //边沿产生DMA请求
#define IRQC_L          PORT_PCR_IRQC(0x08)     //低电平触发外部中断
#define IRQC_RI         PORT_PCR_IRQC(0x09)     //上升沿触发外部中断
#define IRQC_FA         PORT_PCR_IRQC(0x0A)     //下降沿触发外部中断
#define IRQC_ET         PORT_PCR_IRQC(0x0B)     //边沿触发外部中断
#define IRQC_H          PORT_PCR_IRQC(0x0C)     //高电平触发外部中断

//宏定义端口地址
#define PTA     PTA_BASE_PTR
#define PTB     PTB_BASE_PTR
#define PTC     PTC_BASE_PTR
#define PTD     PTD_BASE_PTR
#define PTE     PTE_BASE_PTR
#define PTF     PTF_BASE_PTR

//GPIO模块中断回调函数类型
typedef void (*GPIO_ISR_CALLBACK)(void);

//定义初始化结构体
typedef struct
{
    /*
     * 选择PORTx
     */
    GPIO_MemMapPtr GPIO_PTx;
    /*
     * 选择要初始话的引脚
     */
    uint32 GPIO_Pins;
    /*
     * 配置引脚控制
     */
    uint32 GPIO_PinControl;
    /*
     * 配置输入输出方向
     */
    uint8 GPIO_Dir;
    /*
     * 配置输出电平
     */
    uint8 GPIO_Output;
  
    /*
        描述：
         外部中断回调函数
        取值：
         函数必须为无返回值,无参数(eg. void isr(void);)
        初始化：
         不必须初始化、如未初始化则不会触发中断
    */
    GPIO_ISR_CALLBACK GPIO_Isr;
}GPIO_InitTypeDef;

//声明函数
//GPIO通用初始化函数
int8 LPLD_GPIO_Init (GPIO_InitTypeDef);
//使能GPIO外部中断
uint8 LPLD_GPIO_EnableIrq (GPIO_InitTypeDef);
//禁用GPIO口外部中断
uint8 LPLD_GPIO_DisableIrq (GPIO_InitTypeDef);
//设置GPIO端口0~31的输出
void LPLD_GPIO_Output (GPIO_MemMapPtr, uint32);
//设置GPIO端口0~31的电平翻转
void LPLD_GPIO_Toggle (GPIO_MemMapPtr, uint32);
//设置GPIO端口一位的电平翻转
void LPLD_GPIO_Toggle_b (GPIO_MemMapPtr, uint8);
//取得GPIO端口0～31位的数据
uint32 LPLD_GPIO_Input (GPIO_MemMapPtr);
//取得GPIO端口一位的数据
uint8 LPLD_GPIO_Input_b (GPIO_MemMapPtr, uint8);
//端口输出控制
void LPLD_GPIO_Output_b (GPIO_MemMapPtr, uint8, uint8);

#endif
