/*
 * File: system_k60.c
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
 #include <stdint.h>
 #include "common.h"

/*-------------------------------------------------------------
 * 定义时钟相关值
 * -----------------------------------------------------------*/
#define CPU_XTAL_CLK_HZ                 50000000u       //外部有源晶振频率，单位Hz
#define CPU_XTAL32k_CLK_HZ              32768u          //外部32k时钟晶振频率，单位Hz    
#define CPU_INT_SLOW_CLK_HZ             32768u          //慢速内部振荡器的值，单位Hz
#define CPU_INT_FAST_CLK_HZ             4000000u        //快速内部振荡器的值，单位Hz
#define DEFAULT_SYSTEM_CLOCK            100000000u      //默认系统主频，单位Hz
 

/**
 * @brief 系统主频（单位Hz）
 */
uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/**
 * \brief  Clock initialization ( MHz)
 *
 * \param  void
 *
 * \return void
 */
void SystemInit(void)
{
  //使能所有IO口时钟
  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK
              | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK
              | SIM_SCGC5_PORTE_MASK);
  //初始化各部分时钟：系统内核主频、总线时钟、FlexBus时钟、Flash时钟
  LPLD_PLL_Setup(CORE_CLK_MHZ);
  
  //更新内核主频
  SystemCoreClockUpdate();
  
  //获取各部分时钟
  g_core_clock = SystemCoreClock;
  g_bus_clock = g_core_clock / ((uint32_t)((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT)+ 1u);
  g_flexbus_clock =  g_core_clock / ((uint32_t)((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV3_MASK) >> SIM_CLKDIV1_OUTDIV3_SHIFT)+ 1u);
  g_flash_clock =  g_core_clock / ((uint32_t)((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT)+ 1u);
 
}

/**
 * 更新SystemCoreClock
 *
 * @param  none
 * @return none
 *
 * @brief  更新全局变量SystemCoreClock的值，以便获取最新的系统内核频率。
 *         
 */
void SystemCoreClockUpdate (void) 
{
    uint32_t temp;
    temp =  CPU_XTAL_CLK_HZ *((uint32_t)(MCG_C6 & MCG_C6_VDIV_MASK) + 24u );
    temp = (uint32_t)(temp/((uint32_t)(MCG_C5 & MCG_C5_PRDIV_MASK) +1u ));
    SystemCoreClock = temp;
}

