/**********************************************************************
	> File Name: hw_pit.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Sun 23 Mar 2014 11:43:33 AM CST
 *********************************************************************/

#include "common.h"
#include "hw_pit.h"


//用户自定义中断服务函数数组
PIT_ISR_CALLBACK PIT_ISR[4];

/*
 * LPLD_PIT_Init
 * PIT通用初始化函数，选择PITx、配置中断周期、中断函数
 * 
 * 参数:
 *    pit_init_structure--PIT初始化结构体，
 *                        具体定义见PIT_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_PIT_Init(PIT_InitTypeDef pit_init_structure)
{ 
  //计算定时加载值
  uint32 ldval = pit_init_structure.PIT_PeriodUs*(g_bus_clock/1000000)
               + pit_init_structure.PIT_PeriodMs*1000*(g_bus_clock/1000000)
               + pit_init_structure.PIT_PeriodS*1000000*(g_bus_clock/1000000);
  PITx pitx = pit_init_structure.PIT_Pitx;
  PIT_ISR_CALLBACK isr_func = pit_init_structure.PIT_Isr;
  
  //参数检查
  //ASSERT( pitx <= PIT3);        //判断模块号
  //ASSERT( ldval > 0);           //判断时加载值  
  if (pitx > PIT3)
    return 0;
  if (ldval <= 0)
    return 0;
  
  //开启定时模块时钟
  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
  
  // 开启 PIT
  PIT_MCR = 0x00;
 
  if(isr_func != NULL){
    PIT_ISR[pitx] = isr_func;
    //使能中断
    PIT->CHANNEL[pitx].TCTRL = PIT_TCTRL_TIE_MASK;
    //在NVIC中使能PIT中断
    //enable_irq(68 + pitx); 
  }
  
  //period = (period_ns/bus_period_ns)-1
  PIT->CHANNEL[pitx].LDVAL = ldval-1;
  //使能中断
  //PIT->CHANNEL[pitx].TCTRL = PIT_TCTRL_TIE_MASK;
  //开始定时
  PIT->CHANNEL[pitx].TCTRL |= PIT_TCTRL_TEN_MASK;
  
  return 1;
}

/*
 * LPLD_PIT_Deinit
 * PIT反初始化函数，关闭选择的PITx
 * 
 * 参数:
 *    pit_init_structure--PIT初始化结构体，
 *                        具体定义见PIT_InitTypeDef
 *
 * 输出:
 *    无
 */
void LPLD_PIT_Deinit(PIT_InitTypeDef pit_init_structure)
{ 
  PITx pitx = pit_init_structure.PIT_Pitx;
  
  //参数检查
  //ASSERT( pitx <= PIT3);        //判断模块号              

  disable_irq(68 + pitx); 

  //禁用中断\停止定时
  PIT->CHANNEL[pitx].TCTRL = 0;
}

/*
 * LPLD_PIT_EnableIrq
 * 使能PITx中断
 * 
 * 参数:
 *    pit_init_structure--PIT初始化结构体，
 *                        具体定义见PIT_InitTypeDef
 *
 * 输出:
 *    无
 *
 */
void LPLD_PIT_EnableIrq(PIT_InitTypeDef pit_init_structure)
{
  PITx pitx = pit_init_structure.PIT_Pitx;
  
  //参数检查
  //ASSERT( pitx <= PIT3);                //判断PITx

  enable_irq(68 + pitx); 
}

/*
 * LPLD_PIT_DisableIrq
 * 禁用PITx中断
 * 
 * 参数:
 *    pit_init_structure--PIT初始化结构体，
 *                        具体定义见PIT_InitTypeDef
 *
 * 输出:
 *    无
 *
 */
void LPLD_PIT_DisableIrq(PIT_InitTypeDef pit_init_structure)
{
  PITx pitx = pit_init_structure.PIT_Pitx;
  
  //参数检查
  //ASSERT( pitx <= PIT3);                //判断PITx
  
  disable_irq(68 + pitx);
}

//pit开始记时
void LPLD_PIT_TimeStart(PITx PIT_pitx)
{
    //开启PIT模块
    SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
    //使能PIT定时器时钟，调试模式下继续使用
    PIT_MCR &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK);
    //禁用PIT， 以便设置加载值生效
    PIT_TCTRL (PIT_pitx) &= ~(PIT_TCTRL_TEN_MASK);
    //设置溢出中断时间
    PIT_LDVAL (PIT_pitx) = ~0;
    //清中断标志位
    //PIT_Flag_Clear (PIT_pitx);
    PIT_TFLG (PIT_pitx) |= PIT_TFLG_TIF_MASK;
    //禁止PITn定时器 (用来清空计数值)
    PIT_TCTRL (PIT_pitx) &= ~PIT_TCTRL_TEN_MASK;
    //使能PIT定时器
    PIT_TCTRL (PIT_pitx) = (0
                           |PIT_TCTRL_TEN_MASK
                           );

}
//获取PIT计时时间，（超时关闭定时器）
uint32 LPLD_PIT_TimeGetUs (PITx PIT_pitx)
{
    uint32 val;
    val = (~0) - PIT_CVAL(PIT_pitx);

    if (PIT_TFLG(PIT_pitx) & PIT_TFLG_TIF_MASK)
    {
        //更换下一句的表达方式
        //PIT_Flag_Clear(PIT_pitx);
        PIT_TFLG (PIT_pitx) |= PIT_TFLG_TIF_MASK;

        PIT_TCTRL(PIT_pitx) &= ~PIT_TCTRL_TEN_MASK;
        return ~0;
    }
    if (val == (~0))
    {
        val --;
    }
    return (val/(g_bus_clock/1000000));
}

//关闭PIT计时
void LPLD_PIT_TimeClose(PITx PIT_pitx)
{
    PIT_TFLG (PIT_pitx) |= PIT_TFLG_TIF_MASK;

    //PIT_Flag_Clear(PIT_pitx);
    PIT_TCTRL(PIT_pitx) &= ~PIT_TCTRL_TEN_MASK;
}

/*
 * PIT0--PIT3中断处理函数
 * 与启动文件startup_K60.s中的中断向量表关联
 * 用户无需修改，程序自动进入对应通道中断函数
 */

void PIT0_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  PIT_ISR[0]();  
  //清除中断标志位
  PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}
void PIT1_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  PIT_ISR[1]();  
  //清除中断标志位
  PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}
void PIT2_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  PIT_ISR[2]();  
  //清除中断标志位
  PIT->CHANNEL[2].TFLG |= PIT_TFLG_TIF_MASK;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}
void PIT3_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  PIT_ISR[3]();  
  //清除中断标志位
  PIT->CHANNEL[3].TFLG |= PIT_TFLG_TIF_MASK;
}

