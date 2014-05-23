/*
 * File: startup_k60.c
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

extern unsigned long _etext;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;

#if defined (__cplusplus)
extern "C" {
#endif
#include "common.h"
//主函数
extern int main(void);
//系统初始化函数
extern void SystemInit (void);

extern void __StackTop(void);

void Reset_Handler(void);
__attribute__ ((weak)) void NMI_Handler(void);
__attribute__ ((weak)) void HardFault_Handler(void);
__attribute__ ((weak)) void MemManage_Handler(void);
__attribute__ ((weak)) void BusFault_Handler(void);
__attribute__ ((weak)) void UsageFault_Handler(void);
__attribute__ ((weak)) void SVC_Handler(void);
__attribute__ ((weak)) void DebugMonitor_Handler(void);
__attribute__ ((weak)) void PendSV_Handler(void);
__attribute__ ((weak)) void SysTick_Handler(void);
__attribute__ ((weak)) void Default_Handler(void);

void DMA0_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA3_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA4_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA5_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA6_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA7_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA8_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA9_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA10_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA11_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA12_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA13_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA14_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA15_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DMA_Error_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void MCM_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void FTFL_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void Read_Collision_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void LVD_LVW_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void LLW_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void Watchdog_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void RNG_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void I2C0_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void I2C1_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void SPI0_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void SPI1_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void SPI2_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CAN0_ORed_Message_buffer_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CAN0_Bus_Off_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CAN0_Error_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CAN0_Tx_Warning_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CAN0_Rx_Warning_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CAN0_Wake_Up_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_ORed_Message_buffer_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_Bus_Off_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_Error_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_Tx_Warning_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_Rx_Warning_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_Wake_Up_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void UART0_RX_TX_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void UART0_ERR_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void UART1_RX_TX_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void UART1_ERR_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void UART2_RX_TX_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void UART2_ERR_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void UART3_RX_TX_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void UART3_ERR_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void UART4_RX_TX_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void UART4_ERR_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void UART5_RX_TX_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void UART5_ERR_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void ADC0_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void ADC1_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CMP0_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CMP1_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CMP2_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void FTM0_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void FTM1_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void FTM2_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void CMT_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void RTC_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void PIT0_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void PIT1_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void PIT2_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void PIT3_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void PDB0_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void USB0_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void USB_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void ENET_1588_Timer_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void ENET_Transmit_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void ENET_Receive_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void ENET_Error_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void I2S0_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void SDHC_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DAC0_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void DAC1_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void TSI0_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void MCG_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void LPTimer_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void PORTA_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void PORTB_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void PORTC_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void PORTD_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void PORTE_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));
void Software_IRQHandler(void) __attribute__ ((weak, alias ("Default_Handler")));

#if defined (__cplusplus)
}
#endif


__attribute__ ((section(".vectors")))
void (* const g_pfnVectors[])(void) = {
  &__StackTop,                             // The initial stack pointer
  Reset_Handler,                           // The reset handler
  NMI_Handler,                             // The NMI handler
  HardFault_Handler,                       // The hard fault handler
  MemManage_Handler,                       // The MPU fault handler
  BusFault_Handler,                        // The bus fault handler
  UsageFault_Handler,                      // The usage fault handler
  0,                                       // Reserved
  0,                                       // Reserved
  0,                                       // Reserved
  0,                                       // Reserved
  SVC_Handler,                             // SVCall handler
  DebugMonitor_Handler,                    // Debug monitor handler
  0,                                       // Reserved
  PendSV_Handler,                          // The PendSV handler
  SysTick_Handler,                         // The SysTick handler
  DMA0_IRQHandler,                         // DMA Channel 0 Transfer Complete
  DMA1_IRQHandler,                         // DMA Channel 1 Transfer Complete
  DMA2_IRQHandler,                         // DMA Channel 2 Transfer Complete
  DMA3_IRQHandler,                         // DMA Channel 3 Transfer Complete
  DMA4_IRQHandler,                         // DMA Channel 4 Transfer Complete
  DMA5_IRQHandler,                         // DMA Channel 5 Transfer Complete
  DMA6_IRQHandler,                         // DMA Channel 6 Transfer Complete
  DMA7_IRQHandler,                         // DMA Channel 7 Transfer Complete
  DMA8_IRQHandler,                         // DMA Channel 8 Transfer Complete
  DMA9_IRQHandler,                         // DMA Channel 9 Transfer Complete
  DMA10_IRQHandler,                        // DMA Channel 10 Transfer Complete
  DMA11_IRQHandler,                        // DMA Channel 11 Transfer Complete
  DMA12_IRQHandler,                        // DMA Channel 12 Transfer Complete
  DMA13_IRQHandler,                        // DMA Channel 13 Transfer Complete
  DMA14_IRQHandler,                        // DMA Channel 14 Transfer Complete
  DMA15_IRQHandler,                        // DMA Channel 15 Transfer Complete
  DMA_Error_IRQHandler,                    // DMA Error Interrupt
  MCM_IRQHandler,                          // Normal Interrupt
  FTFL_IRQHandler,                         // FTFL Interrupt
  Read_Collision_IRQHandler,               // Read Collision Interrupt
  LVD_LVW_IRQHandler,                      // Low Voltage Detect, Low Voltage Warning
  LLW_IRQHandler,                          // Low Leakage Wakeup
  Watchdog_IRQHandler,                     // WDOG Interrupt
  RNG_IRQHandler,                          // RNGB Interrupt
  I2C0_IRQHandler,                         // I2C0 interrupt
  I2C1_IRQHandler,                         // I2C1 interrupt
  SPI0_IRQHandler,                         // SPI0 Interrupt
  SPI1_IRQHandler,                         // SPI1 Interrupt
  SPI2_IRQHandler,                         // SPI2 Interrupt
  CAN0_ORed_Message_buffer_IRQHandler,     // CAN0 OR'd Message Buffers Interrupt
  CAN0_Bus_Off_IRQHandler,                 // CAN0 Bus Off Interrupt
  CAN0_Error_IRQHandler,                   // CAN0 Error Interrupt
  CAN0_Tx_Warning_IRQHandler,              // CAN0 Tx Warning Interrupt
  CAN0_Rx_Warning_IRQHandler,              // CAN0 Rx Warning Interrupt
  CAN0_Wake_Up_IRQHandler,                 // CAN0 Wake Up Interrupt
  Default_Handler,                         // Reserved interrupt 51
  Default_Handler,                         // Reserved interrupt 52
  CAN1_ORed_Message_buffer_IRQHandler,     // CAN1 OR'd Message Buffers Interrupt
  CAN1_Bus_Off_IRQHandler,                 // CAN1 Bus Off Interrupt
  CAN1_Error_IRQHandler,                   // CAN1 Error Interrupt
  CAN1_Tx_Warning_IRQHandler,              // CAN1 Tx Warning Interrupt
  CAN1_Rx_Warning_IRQHandler,              // CAN1 Rx Warning Interrupt
  CAN1_Wake_Up_IRQHandler,                 // CAN1 Wake Up Interrupt
  Default_Handler,                         // Reserved interrupt 59
  Default_Handler,                         // Reserved interrupt 60
  UART0_RX_TX_IRQHandler,                  // UART0 Receive/Transmit interrupt
  UART0_ERR_IRQHandler,                    // UART0 Error interrupt
  UART1_RX_TX_IRQHandler,                  // UART1 Receive/Transmit interrupt
  UART1_ERR_IRQHandler,                    // UART1 Error interrupt
  UART2_RX_TX_IRQHandler,                  // UART2 Receive/Transmit interrupt
  UART2_ERR_IRQHandler,                    // UART2 Error interrupt
  UART3_RX_TX_IRQHandler,                  // UART3 Receive/Transmit interrupt
  UART3_ERR_IRQHandler,                    // UART3 Error interrupt
  UART4_RX_TX_IRQHandler,                  // UART4 Receive/Transmit interrupt
  UART4_ERR_IRQHandler,                    // UART4 Error interrupt
  UART5_RX_TX_IRQHandler,                  // UART5 Receive/Transmit interrupt
  UART5_ERR_IRQHandler,                    // UART5 Error interrupt
  ADC0_IRQHandler,                         // ADC0 interrupt
  ADC1_IRQHandler,                         // ADC1 interrupt
  CMP0_IRQHandler,                         // CMP0 interrupt
  CMP1_IRQHandler,                         // CMP1 interrupt
  CMP2_IRQHandler,                         // CMP2 interrupt
  FTM0_IRQHandler,                         // FTM0 fault, overflow and channels interrupt
  FTM1_IRQHandler,                         // FTM1 fault, overflow and channels interrupt
  FTM2_IRQHandler,                         // FTM2 fault, overflow and channels interrupt
  CMT_IRQHandler,                          // CMT interrupt
  RTC_IRQHandler,                          // RTC interrupt
  Default_Handler,                         // Reserved interrupt 83
  PIT0_IRQHandler,                         // PIT timer channel 0 interrupt
  PIT1_IRQHandler,                         // PIT timer channel 1 interrupt
  PIT2_IRQHandler,                         // PIT timer channel 2 interrupt
  PIT3_IRQHandler,                         // PIT timer channel 3 interrupt
  PDB0_IRQHandler,                         // PDB0 Interrupt
  USB0_IRQHandler,                         // USB0 interrupt
  USB_IRQHandler,                          // USBInterrupt
  ENET_1588_Timer_IRQHandler,              // Ethernet MAC IEEE 1588 Timer Interrupt
  ENET_Transmit_IRQHandler,                // Ethernet MAC Transmit Interrupt
  ENET_Receive_IRQHandler,                 // Ethernet MAC Receive Interrupt
  ENET_Error_IRQHandler,                   // Ethernet MAC Error and miscelaneous Interrupt
  I2S0_IRQHandler,                         // I2S0 Interrupt
  SDHC_IRQHandler,                         // SDHC Interrupt
  DAC0_IRQHandler,                         // DAC0 interrupt
  DAC1_IRQHandler,                         // DAC1 interrupt
  TSI0_IRQHandler,                         // TSI0 Interrupt
  MCG_IRQHandler,                          // MCG Interrupt
  LPTimer_IRQHandler,                      // LPTimer interrupt
  Default_Handler,                         // Reserved interrupt 102
  PORTA_IRQHandler,                        // Port A interrupt
  PORTB_IRQHandler,                        // Port B interrupt
  PORTC_IRQHandler,                        // Port C interrupt
  PORTD_IRQHandler,                        // Port D interrupt
  PORTE_IRQHandler,                        // Port E interrupt
  Default_Handler,                         // Reserved interrupt 108
  Default_Handler,                         // Reserved interrupt 109
  Software_IRQHandler,                     // Software Interrupt
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  Default_Handler,                         // Reserved
  (void *)0xFFFFFFFF,                      // FTFL
  (void *)0xFFFFFFFF,                      // FTFL
  (void *)0xFFFFFFFF,                      // FTFL
  (void *)0xFFFFFFFE                       // FTFL
};

/**
 * \brief  Default Handler
 *
 * \param  void
 *
 * \return void
 */
void Default_Handler(void)
{
#define VECTORNUM       (*(volatile char *) (0xE000ED04))
  //while (1);
  char vtr = VECTORNUM;
  printf ("\n ******************\n");
  printf ("Default Handler Error\n");
  printf ("On vector %d **** Interrupt\n", vtr);
  while (1);
}

/**
 * \brief  Reset ISR Handler
 *
 * \param  void
 *
 * \return void
 */
void Reset_Handler(void)
{
  unsigned char *source;
  unsigned char *destination;

  /* watchdog disable */
  *((volatile unsigned short *)0x4005200E) = 0xC520;
  *((volatile unsigned short *)0x4005200E) = 0xD928;
  *((volatile unsigned short *)0x40052000) = 0x01D2;

  /* copy data values from ROM to RAM */
  source = (unsigned char *)&_etext;
  destination = (unsigned char *)&_sdata;
  while (destination < (unsigned char*)&_edata) {
    *(destination++) = *(source++);
  }

  /* clear bss section */
  source = (unsigned char *)&_sbss;
  destination = (unsigned char *)&_ebss;
  while (source < destination ) {
    *source++ = 0;
  }
 //进入系统初始化函数
  SystemInit();
 //进入用户函数
  main();

  while(1);
}
