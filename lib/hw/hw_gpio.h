/**********************************************************************
	> File Name: hw_gpio.h
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Tue 18 Mar 2014 11:23:44 AM CST
 *********************************************************************/

#ifndef __HW_GPIO_H__
#define __HW_GPIO_H__

#include "common.h"

//宏定义初始化函数用到的参数
//GPIO_InitTypeDef->GPIO_Dir
#define DIR_INPUT   0 //输入
#define DIR_OUTPUT  1 //输出
//GPIO_InitTypeDef->GPIO_OUTPUT
#define OUTPUT_L    0 //输出低电平
#define OUTPUT_H    0 //输出高电平

//宏定义端口地址
#define PTA     PTA_BASE_PTR
#define PTB     PTB_BASE_PTR
#define PTC     PTC_BASE_PTR
#define PTD     PTD_BASE_PTR
#define PTE     PTE_BASE_PTR
#define PTF     PTF_BASE_PTR

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

}GPIO_InitTypeDef;

//声明函数
//GPIO通用初始化函数
int8 MOS_GPIO_Init (GPIO_InitTypeDef);
//设置GPIO端口0~31的输出
void MOS_GPIO_Output (GPIO_MemMapPtr, uint32);
//设置GPIO端口0~31的电平翻转
void MOS_GPIO_Toggle (GPIO_MemMapPtr, uint32);
//取得GPIO端口0～31位的数据
uint32 MOS_GPIO_Input (GPIO_MemMapPtr);

#endif
