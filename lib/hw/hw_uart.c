/**********************************************************************
	> File Name: hw_uart.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Wed 19 Mar 2014 12:12:57 AM CST
 *********************************************************************/

#include "common.h"
#include "hw_uart.h"

//用户接收中断函数入口地址数组
UART_ISR_CALLBACK UART_R_ISR[6];
//用户发送中断函数入口地址数组
UART_ISR_CALLBACK UART_T_ISR[6];  


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
  
  if(baud == 0)
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
      PORTA_BASE_PTR->PCR[2] = PORT_PCR_MUX(2); 
    else if(tx_pin == PTA14)
      PORTA_BASE_PTR->PCR[14] = PORT_PCR_MUX(3); 
    else
      PORTB_BASE_PTR->PCR[17] = PORT_PCR_MUX(3); 
    //设置Rx引脚
    if(rx_pin == PTA1)
      PORTA_BASE_PTR->PCR[1] = PORT_PCR_MUX(2); 
    else if(rx_pin == PTA15)
      PORTA_BASE_PTR->PCR[15] = PORT_PCR_MUX(3); 
    else
      PORTB_BASE_PTR->PCR[16] = PORT_PCR_MUX(3); 
  }
  else
  {
    if (uartx == UART1_BASE_PTR)
    {
      x = 1;
      sysclk = g_core_clock;
      SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
      
      if(tx_pin == PTE0)   
        PORTE_BASE_PTR->PCR[0] = PORT_PCR_MUX(3); 
      else
        PORTC_BASE_PTR->PCR[4] = PORT_PCR_MUX(3); 
      
      if(rx_pin == PTE1)
        PORTE_BASE_PTR->PCR[1] = PORT_PCR_MUX(3); 
      else
        PORTC_BASE_PTR->PCR[3] = PORT_PCR_MUX(3); 
    }
    else
    {
      sysclk = g_bus_clock;
      if (uartx == UART2_BASE_PTR)
      {
        x = 2;
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
        
        PORTD_BASE_PTR->PCR[3] = PORT_PCR_MUX(3); 
        PORTD_BASE_PTR->PCR[2] = PORT_PCR_MUX(3);
      }
      else
      {
        if(uartx == UART3_BASE_PTR)
        {
          x = 3;
          SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
          
          if(tx_pin == PTE4)
            PORTE_BASE_PTR->PCR[4] = PORT_PCR_MUX(3); 
          else if(tx_pin == PTB11)
            PORTB_BASE_PTR->PCR[11] = PORT_PCR_MUX(3); 
          else 
            PORTC_BASE_PTR->PCR[17] = PORT_PCR_MUX(3); 
          
          if(rx_pin == PTE5)
            PORTE_BASE_PTR->PCR[5] = PORT_PCR_MUX(3); 
          else if(rx_pin == PTB10)
            PORTB_BASE_PTR->PCR[10] = PORT_PCR_MUX(3); 
          else
            PORTC_BASE_PTR->PCR[16] = PORT_PCR_MUX(3);
        }
        else
        {
          if(uartx == UART4_BASE_PTR)
          {
            x = 4;
            SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;
            
            if(tx_pin == PTE24)
              PORTE_BASE_PTR->PCR[24] = PORT_PCR_MUX(3); 
            else
              PORTC_BASE_PTR->PCR[15] = PORT_PCR_MUX(3); 
            
            if(rx_pin == PTE25)
              PORTE_BASE_PTR->PCR[25] = PORT_PCR_MUX(3); 
            else
              PORTC_BASE_PTR->PCR[14] = PORT_PCR_MUX(3);
          }
          else
          {
            x = 5;
            uartx = UART5_BASE_PTR;
            SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;
            
            if(tx_pin == PTD9)
              PORTD_BASE_PTR->PCR[9] = PORT_PCR_MUX(3); 
            else
              PORTE_BASE_PTR->PCR[8] = PORT_PCR_MUX(3); 
            
            if(rx_pin == PTD8)
              PORTD_BASE_PTR->PCR[8] = PORT_PCR_MUX(3); 
            else
              PORTE_BASE_PTR->PCR[9] = PORT_PCR_MUX(3); 
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
  temp = uartx->BDH & ~(UART_BDH_SBR(0x1F));
  
  uartx->BDH = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
  uartx->BDL = (uint8)(sbr & UART_BDL_SBR_MASK);
  
  //配置波特率的微调分数
  brfa = (((sysclk*32)/(baud * 16)) - (sbr * 32));
  
  //保存UARTx_C4寄存器中除了BRFA的值
  temp = uartx->C4 & ~(UART_C4_BRFA(0x1F));
  
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

/*
 * LPLD_UART_GetChar
 * 串口查询方式读取一个字节
 * 
 * 参数:
 *    uartx--UART模块号
 *      |__UART0          --UART0
 *      |__UART1          --UART1
 *      |__UART2          --UART2
 *      |__UART3          --UART3
 *      |__UART4          --UART4
 *      |__UART5          --UART5
 *
 * 输出:
 *    串口接收的1个字节
 *
 */
int8 LPLD_UART_GetChar(UART_MemMapPtr uartx)
{
  //等待数据接收
  while (!(uartx->S1 & UART_S1_RDRF_MASK));
  
  //返回接收的1个字节数据
  return uartx->D;
}

/********************************************************************/
/*
 * LPLD_UART_GetChar_Present
 * 检查是否接收到字符
 * 
 * 参数:
 *    uartx--UART模块号
 *      |__UART0          --UART0
 *      |__UART1          --UART1
 *      |__UART2          --UART2
 *      |__UART3          --UART3
 *      |__UART4          --UART4
 *      |__UART5          --UART5
 *
 * 输出:
 *  0       没有接收到字符
 *  1       已经接收到字符
 */
int32 LPLD_UART_GetChar_Present(UART_MemMapPtr uartx)
{
    return (uartx->S1 & UART_S1_RDRF_MASK);
}


/*
 * LPLD_UART_PutChar
 * 串口查询方式发送一个字节
 * 
 * 参数:
 *    uartx--UART模块号
 *      |__UART0          --UART0
 *      |__UART1          --UART1
 *      |__UART2          --UART2
 *      |__UART3          --UART3
 *      |__UART4          --UART4
 *      |__UART5          --UART5
 *    ch--待发送的1个字节
 *
 * 输出:
 *    无
 *
 */
void LPLD_UART_PutChar(UART_MemMapPtr uartx, int8 ch)
{
  //等待FIFO准备就绪
  while(!(uartx->S1 & UART_S1_TDRE_MASK));
  
  //将要发送的1个字节发给UART数据寄存器
  uartx->D = (uint8)ch;
}


/*
 * LPLD_UART_PutCharArr
 * 串口查询方式发送字节型数组
 * 
 * 参数:
 *    uartx--UART模块号
 *      |__UART0          --UART0
 *      |__UART1          --UART1
 *      |__UART2          --UART2
 *      |__UART3          --UART3
 *      |__UART4          --UART4
 *      |__UART5          --UART5
 *    *ch--待发送的字节数组头地址
 *    len--字节数组长度
 *
 * 输出:
 *    无
 *
 */
void LPLD_UART_PutCharArr(UART_MemMapPtr uartx, char *ch, int32 len)
{
  while(len--)
  {
    LPLD_UART_PutChar(uartx, *(ch++));
  }
}

/*
 * LPLD_UART_EnableIrq
 * 使能串口数据接收中断
 * 
 * 参数:
 *    uart_init_structure--UART初始化结构体，
 *                        具体定义见UART_InitTypeDef
 *
 * 输出:
 *    无
 *
 */
void LPLD_UART_EnableIrq(UART_InitTypeDef uart_init_structure)
{
  if(uart_init_structure.UART_Uartx == UART0_BASE_PTR)
  {
    enable_irq(INT_UART0_RX_TX - 16);
  }
  else if(uart_init_structure.UART_Uartx == UART1_BASE_PTR)
  {
    enable_irq(INT_UART1_RX_TX - 16);
  }
  else if(uart_init_structure.UART_Uartx == UART2_BASE_PTR)
  {
    enable_irq(INT_UART3_RX_TX - 16);
  }
  else if(uart_init_structure.UART_Uartx == UART3_BASE_PTR)
  {
    enable_irq(INT_UART3_RX_TX - 16);
  }
  else if(uart_init_structure.UART_Uartx == UART4_BASE_PTR)
  {
    enable_irq(INT_UART4_RX_TX - 16);
  }
  else if(uart_init_structure.UART_Uartx == UART5_BASE_PTR)
  {
    enable_irq(INT_UART5_RX_TX - 16);
  }
}

/*
 * LPLD_UART_DisableIrq
 * 禁用串口数据接收中断
 * 
 * 参数:
 *    uart_init_structure--UART初始化结构体，
 *                        具体定义见UART_InitTypeDef
 *
 * 输出:
 *    无
 *
 */
void LPLD_UART_DisableIrq(UART_InitTypeDef uart_init_structure)
{
  //根据中断请求号使能相应中断
  if(uart_init_structure.UART_Uartx == UART0_BASE_PTR)
    disable_irq(INT_UART0_RX_TX - 16);
  else if(uart_init_structure.UART_Uartx == UART1_BASE_PTR)
    disable_irq(INT_UART1_RX_TX - 16);
  else if(uart_init_structure.UART_Uartx == UART2_BASE_PTR)
    disable_irq(INT_UART2_RX_TX - 16);
  else if(uart_init_structure.UART_Uartx == UART3_BASE_PTR)
    disable_irq(INT_UART3_RX_TX - 16);
  else if(uart_init_structure.UART_Uartx == UART4_BASE_PTR)
    disable_irq(INT_UART4_RX_TX - 16);
  else if(uart_init_structure.UART_Uartx == UART5_BASE_PTR)
    disable_irq(INT_UART5_RX_TX - 16);
}

//HW层中断函数，用户无需调用
//UART0
void UART0_RX_TX_IRQHandler(void)
{
  
  //进入接收中断函数
  if((UART0_S1 & UART_S1_RDRF_MASK) && (UART0_C2 & UART_C2_RIE_MASK))
  {
    UART_R_ISR[0]();
  }
  //进入发送中断函数
  if((UART0_S1 & UART_S1_TDRE_MASK) && (UART0_C2 & UART_C2_TIE_MASK))
  {
    UART_T_ISR[0]();
  }
}

//UART1
void UART1_RX_TX_IRQHandler(void)
{
  
  //进入接收中断函数
  if((UART1_S1 & UART_S1_RDRF_MASK) && (UART1_C2 & UART_C2_RIE_MASK))
  {
    UART_R_ISR[0]();
  }
  //进入发送中断函数
  if((UART1_S1 & UART_S1_TDRE_MASK) && (UART1_C2 & UART_C2_TIE_MASK))
  {
    UART_T_ISR[0]();
  }
}

//UART2
void UART2_RX_TX_IRQHandler(void)
{
  //进入接收中断函数
  if((UART2_S1 & UART_S1_RDRF_MASK) && (UART2_C2 & UART_C2_RIE_MASK))
  {
    UART_R_ISR[0]();
  }
  //进入发送中断函数
  if((UART2_S1 & UART_S1_TDRE_MASK) && (UART2_C2 & UART_C2_TIE_MASK))
  {
    UART_T_ISR[0]();
  }
}

//uart3
void UART3_RX_TX_IRQHandler(void)
{
  //进入接收中断函数
  if((UART3_S1 & UART_S1_RDRF_MASK) && (UART3_C2 & UART_C2_RIE_MASK))
  {
    UART_R_ISR[0]();
  }
  //进入发送中断函数
  if((UART3_S1 & UART_S1_TDRE_MASK) && (UART3_C2 & UART_C2_TIE_MASK))
  {
    UART_T_ISR[0]();
  }
}

//uart4
void UART4_RX_TX_IRQHandler(void)
{
  //进入接收中断函数
  if((UART4_S1 & UART_S1_RDRF_MASK) && (UART4_C2 & UART_C2_RIE_MASK))
  {
    UART_R_ISR[0]();
  }
  //进入发送中断函数
  if((UART4_S1 & UART_S1_TDRE_MASK) && (UART4_C2 & UART_C2_TIE_MASK))
  {
    UART_T_ISR[0]();
  }
}

//uart3
void UART5_RX_TX_IRQHandler(void)
{
  //进入接收中断函数
  if((UART5_S1 & UART_S1_RDRF_MASK) && (UART5_C2 & UART_C2_RIE_MASK))
  {
    UART_R_ISR[0]();
  }
  //进入发送中断函数
  if((UART5_S1 & UART_S1_TDRE_MASK) && (UART5_C2 & UART_C2_TIE_MASK))
  {
    UART_T_ISR[0]();
  }
}

