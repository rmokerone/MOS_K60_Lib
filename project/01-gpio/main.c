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

#define BUTTON (1<<7)

void init_gpio(void);
void delay (void);
void toggle (void);
void enable_irq (int irq);

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
    //MOS_GPIO_Toggle (PTB,LED2);
    //MOS_GPIO_Output (PTB,LED2);
    while (1)
    {
        //toggle();
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
    gpio_init_struct.GPIO_Output = OUTPUT_L;
    MOS_GPIO_Init (gpio_init_struct);

    gpio_init_struct.GPIO_PTx = PTD;
    gpio_init_struct.GPIO_Pins = BUTTON;
    gpio_init_struct.GPIO_Dir = DIR_INPUT;
    gpio_init_struct.GPIO_PinControl = IRQC_FA | INPUT_PULL_UP;
    gpio_init_struct.GPIO_Isr = toggle;
    MOS_GPIO_Init (gpio_init_struct);
    MOS_GPIO_EnableIrq (gpio_init_struct);
}

void toggle (void)
{
     MOS_GPIO_Toggle(PTB,LED2);
}
