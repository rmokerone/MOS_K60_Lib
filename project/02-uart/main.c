/**********************************************************************
	> File Name: main.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Wed 19 Mar 2014 10:03:50 PM CST
 *********************************************************************/

#include "common.h"

void uart_init(void);
void delay(void);

UART_InitTypeDef uart0_init_struct;

int main (void)
{
    char recv;

    uart_init();
    LPLD_UART_PutCharArr (UART0, "Input something:\r\n",18);
    while (1)
    {
        recv = LPLD_UART_GetChar (UART0);
        LPLD_UART_PutChar (UART0, recv);
        //LPLD_UART_PutCharArr (UART0, "Input something:\r\n",18);
    }
    return 0;
}

void uart_init(void)
{
    uart0_init_struct.UART_Uartx = UART0;
    uart0_init_struct.UART_BaudRate = 9600;
    uart0_init_struct.UART_RxPin = PTB16;
    uart0_init_struct.UART_TxPin = PTB17;

    LPLD_UART_Init (uart0_init_struct);
}

void delay (void)
{
    int x,i;
    for (x = 0xfff; x > 0; x--)
        for (i = 0xffff; i > 0; i--);
    
}
