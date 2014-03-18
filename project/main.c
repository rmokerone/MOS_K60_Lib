/*
 * File: main.c
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
#include "common.h"
#include "main.h"

void init_gpio(void);
void delay (void);

GPIO_InitTypeDef gpio_init_struct;

/**
 * \brief  LED1 Toggle with a delay, button SW1 toggles LED2 (ISR)
 *
 * \param  void
 *
 * \return void
 */
int main(void)
{
    init_gpio();
    while (1)
    {
        MOS_GPIO_Toggle(PTB,LED2);
        delay();
    }
    
}

/**
 * \brief  Silly delay
 *
 * \param  void
 *
 * \return void
 */
void delay(void)
{
  volatile unsigned int i,j;

  for(i=0; i<10000; i++)
  {
	for(j=0; j<300; j++)
      __asm__("nop");
  }
}

void init_gpio(void)
{
    gpio_init_struct.GPIO_PTx = PTB;
    gpio_init_struct.GPIO_Pins = LED2;
    gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
    gpio_init_struct.GPIO_Output = OUTPUT_H;
    MOS_GPIO_Init (gpio_init_struct);
}
