/**********************************************************************
	> File Name: main.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Sun 23 Mar 2014 11:56:43 AM CST
 *********************************************************************/

#include "common.h"

void pit_init (void);
void pit0_isr (void);
void pit1_isr (void);

//变量定义
PIT_InitTypeDef pit0_init_struct;
PIT_InitTypeDef pit1_init_struct;

int main (void)
{
    pit_init();
    while (1)
    {

    }
    return 0;
}

void pit_init(void)
{
    pit0_init_struct.PIT_Pitx = PIT0;
    pit0_init_struct.PIT_PeriodS = 2;
    pit0_init_struct.PIT_Isr = pit0_isr;

    LPLD_PIT_Init (pit0_init_struct);

    pit1_init_struct.PIT_Pitx = PIT1;
    pit1_init_struct.PIT_PeriodMs = 500;
    pit1_init_struct.PIT_Isr = pit1_isr;

    LPLD_PIT_Init (pit1_init_struct);

    LPLD_PIT_EnableIrq (pit0_init_struct);
    LPLD_PIT_EnableIrq (pit1_init_struct);

}

void pit0_isr(void)
{
    printf ("hello i'm pit0\n");
}

void pit1_isr(void)
{
    printf ("hello i'm pit1\n");
}
