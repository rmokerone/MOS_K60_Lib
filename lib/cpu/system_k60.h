/*
 * File: system_k60.h
 *
 * Copyright (c) 2013, 0xc0170
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 *
 */
#ifndef SYSTEM_K60_H
#define SYSTEM_K60_H

#include <stdint.h>
#include "common.h"

#if defined (__cplusplus)
extern "C" {
#endif

/**
 * @brief 系统主频 （单位Hz)
 */
extern uint32_t SystemCoreClock;

/**
 * 初始化系统
 * 
 * @parm none
 * @return none
 *
 * @brief 初始化MCU硬件系统，包括开放IO时钟，初始化主频,
 * SystemInit函数由MCU启动文件启动.
 */
void SystemInit(void);

/**
 * 更新SystemCoreClock
 *
 * @param  none
 * @return none
 *
 * @brief  更新全局变量SystemCoreClock的值，以便获取最新的系统内核频率。
 *         
 */
void SystemCoreClockUpdate (void); 

/*
 * 使能中断irq
 */
void enable_irq (int irq);

/*
 * 关闭中断irq
 */
void disable_irq(int irq);

#if defined (__cplusplus)
}
#endif

#endif
