/**********************************************************************
	> File Name: hw_uart.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Wed 19 Mar 2014 12:12:57 AM CST
 *********************************************************************/

#include "common.h"
#include "hw_uart.h"


/*
 * LPLD_UART_Init
 * 初始化UART通道、波特率、发送接收引脚
 * 
 * 参数:
 *    uart_init_structure--UART初始化结构体，
 *                        具体定义见UART_InitTypeDef
 *
 * 输出:
 *    无
 *
 */
void LPLD_UART_Init(UART_InitTypeDef uart_init_structure)
{
  register uint16 sbr, brfa;
  uint32 sysclk;
  uint8 temp, x;
  UART_MemMapPtr uartx = uart_init_structure.UART_Uartx;
  uint32 baud = uart_init_structure.UART_BaudRate;
  PortPinsEnum_Type tx_pin = uart_init_structure.UART_TxPin;
  PortPinsEnum_Type rx_pin = uart_init_structure.UART_RxPin;
  UART_ISR_CALLBACK rx_isr = uart_init_structure.UART_RxIsr;
  UART_ISR_CALLBACK tx_isr = uart_init_structure.UART_TxIsr;
  
  if(baud == NULL)
  {
    baud = 9600;
  }
  
  //使能选中的UART串口通道时钟，相应GPIO的UART复用功能   
  if(uartx == UART0_BASE_PTR)
  {
    x = 0;
    sysclk = g_core_clock;
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
    //设置Tx引脚
    if(tx_pin == PTA2)
      PORTA_PCR[2] = PORT_PCR_MUX(2); 
    else if(tx_pin == PTA14)
      PORTA_PCR[14] = PORT_PCR_MUX(3); 
    else
      PORTB_PCR[17] = PORT_PCR_MUX(3); 
    //设置Rx引脚
    if(rx_pin == PTA1)
      PORTA_PCR[1] = PORT_PCR_MUX(2); 
    else if(rx_pin == PTA15)
      PORTA_PCR[15] = PORT_PCR_MUX(3); 
    else
      PORTB_PCR[16] = PORT_PCR_MUX(3); 
  }
  else
  {
    if (uartx == UART1_BASE_PTR)
    {
      x = 1;
      sysclk = g_core_clock;
      SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
      
      if(tx_pin == PTE0)   
        PORTE_PCR[0] = PORT_PCR_MUX(3); 
      else
        PORTC_PCR[4] = PORT_PCR_MUX(3); 
      
      if(rx_pin == PTE1)
        PORTE_PCR[1] = PORT_PCR_MUX(3); 
      else
        PORTC_PCR[3] = PORT_PCR_MUX(3); 
    }
    else
    {
      sysclk = g_bus_clock;
      if (uartx == UART2_BASE_PTR)
      {
        x = 2;
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
        
        PORTD_PCR[3] = PORT_PCR_MUX(3); 
        PORTD_PCR[2] = PORT_PCR_MUX(3);
      }
      else
      {
        if(uartx == UART3_BASE_PTR)
        {
          x = 3;
          SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
          
          if(tx_pin == PTE4)
            PORTE_PCR[4] = PORT_PCR_MUX(3); 
          else if(tx_pin == PTB11)
            PORTB_PCR[11] = PORT_PCR_MUX(3); 
          else 
            PORTC_PCR[17] = PORT_PCR_MUX(3); 
          
          if(rx_pin == PTE5)
            PORTE_PCR[5] = PORT_PCR_MUX(3); 
          else if(rx_pin == PTB10)
            PORTB_PCR[10] = PORT_PCR_MUX(3); 
          else
            PORTC_PCR[16] = PORT_PCR_MUX(3);
        }
        else
        {
          if(uartx == UART4_BASE_PTR)
          {
            x = 4;
            SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;
            
            if(tx_pin == PTE24)
              PORTE_PCR[24] = PORT_PCR_MUX(3); 
            else
              PORTC_PCR[15] = PORT_PCR_MUX(3); 
            
            if(rx_pin == PTE25)
              PORTE_PCR[25] = PORT_PCR_MUX(3); 
            else
              PORTC_PCR[14] = PORT_PCR_MUX(3);
          }
          else
          {
            x = 5;
            uartx = UART5_BASE_PTR;
            SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;
            
            if(tx_pin == PTD9)
              PORTD_PCR[9] = PORT_PCR_MUX(3); 
            else
              PORTE_PCR[8] = PORT_PCR_MUX(3); 
            
            if(rx_pin == PTD8)
              PORTD_PCR[8] = PORT_PCR_MUX(3); 
            else
              PORTE_PCR[9] = PORT_PCR_MUX(3); 
          }
        }
      }
    }
  }
  
  //在配置好其他寄存器前，先关闭发送器和接收器
  uartx->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
  
  //配置UART为 8位, 无奇偶校验 */
  uartx->C1 = 0;	
  
  //计算波特率
  sbr = (uint16)((sysclk)/(baud * 16));
  
  //保存UARTx_BDH寄存器中除了SBR的值
  temp = uartx_BDH & ~(UART_BDH_SBR(0x1F));
  
  uartx->BDH = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
  uartx->BDL = (uint8)(sbr & UART_BDL_SBR_MASK);
  
  //配置波特率的微调分数
  brfa = (((sysclk*32)/(baud * 16)) - (sbr * 32));
  
  //保存UARTx_C4寄存器中除了BRFA的值
  temp = uartx_C4 & ~(UART_C4_BRFA(0x1F));
  
  uartx->C4 = temp |  UART_C4_BRFA(brfa);    
  
  //配置接收中断
  if(uart_init_structure.UART_RxIntEnable == TRUE && rx_isr != NULL)
  {
    uartx->C2 |= UART_C2_RIE_MASK; 
    UART_R_ISR[x] = rx_isr;
  } 
  else
  {
    uartx->C2 &= ~(UART_C2_RIE_MASK); 
  }
  //配置发送中断
  if(uart_init_structure.UART_TxIntEnable == TRUE && tx_isr != NULL)
  {
    uartx->C2 |= UART_C2_TIE_MASK; 
    UART_T_ISR[x] = tx_isr;
  } 
  else
  {
    uartx->C2 &= ~(UART_C2_TIE_MASK); 
  }
  
  //使能发送器和接收器
  uartx->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );    
}
