/**********************************************************************
	> File Name: hw_gpio.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Tue 18 Mar 2014 11:58:07 AM CST
 *********************************************************************/

#include "hw_gpio.h"

//用户自定义中断服务函数组
GPIO_ISR_CALLBACK GPIO_ISR[5];

/*
 * \brief GPIO通用初始化函数
 * \param gpio_init_structure
* \return 0--配置失败
 *         1--配置成功
 */
int8 LPLD_GPIO_Init(GPIO_InitTypeDef gpio_init_structure)
{
    uint8 i;

    PORT_MemMapPtr portx;
    uint32 pcr = PORT_PCR_MUX(1)
                |gpio_init_structure.GPIO_PinControl;
    GPIO_MemMapPtr ptx = gpio_init_structure.GPIO_PTx;
    uint32 pins = gpio_init_structure.GPIO_Pins;
    uint8 dir = gpio_init_structure.GPIO_Dir;
    uint8 output = gpio_init_structure.GPIO_Output;
    //GPIO_ISR_CALLBACK isr_func = gpio_init_structure.GPIO_Isr;
    

    if  (ptx == PTA)
        portx = PORTA_BASE_PTR;
    else if (ptx == PTB)
        portx = PORTB_BASE_PTR;
    else if (ptx == PTC)
        portx = PORTC_BASE_PTR;
    else if (ptx == PTD)
        portx = PORTD_BASE_PTR;
    else if (ptx == PTE)
        portx = PORTE_BASE_PTR;
    else if (ptx == PTF)
        portx = PORTF_BASE_PTR;
    else
        return 0;
    
    if (dir == DIR_OUTPUT)
    {
        ptx->PDDR |= pins;
        if (output == OUTPUT_H)
            ptx->PSOR |= pins;
        else
            ptx->PCOR |= pins;
    }
    else
    {
        ptx->PDDR &= ~(pins);
    }
    //配置所选引脚的控制寄存器
    for (i =0; i < 32; i++)
    {
        if (pins&(1ul<<i))
            portx->PCR[i] = pcr;
    }

    return 1;

}

/*
 * LPLD_GPIO_EnableIrq
 * 使能GPIO外部中断
 * 
 * 参数:
 *    gpio_init_structure--GPIO初始化结构体， 
 *                       具体定义见GPIO_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 LPLD_GPIO_EnableIrq(GPIO_InitTypeDef gpio_init_structure)
{
    GPIO_MemMapPtr ptx = gpio_init_structure.GPIO_PTx;
    GPIO_ISR_CALLBACK isr_func = gpio_init_structure.GPIO_Isr;
    
    if (isr_func == NULL)
        return 0;
    if (ptx == PTA)
    {
        GPIO_ISR[0] = isr_func;
        enable_irq(87);
    }
    else if (ptx == PTB)
    {
        enable_irq(88);
        GPIO_ISR[1] = isr_func;
    }
    else if (ptx == PTC)
    {
        enable_irq(89);
        GPIO_ISR[2] = isr_func;
    }
    else if (ptx == PTD)
    {
        enable_irq(90);
        GPIO_ISR[3] = isr_func;
    }
    else if (ptx == PTE)
    {
        enable_irq(91);
        GPIO_ISR[4] = isr_func;
    }
    else
        return 0;
    return 1;
}

/*
 * LPLD_GPIO_DisableIrq
 * 禁用GPIO外部中断
 * 
 * 参数:
 *    gpio_init_structure--GPIO初始化结构体，
 *                        具体定义见GPIO_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 LPLD_GPIO_DisableIrq(GPIO_InitTypeDef gpio_init_structure)
{
    uint8 i;
    GPIO_MemMapPtr ptx = gpio_init_structure.GPIO_PTx;
    PORT_MemMapPtr portx;
    uint32 pins = gpio_init_structure.GPIO_Pins;
 
    if  (ptx == PTA)
        portx = PORTA_BASE_PTR;
    else if (ptx == PTB)
        portx = PORTB_BASE_PTR;
    else if (ptx == PTC)
        portx = PORTC_BASE_PTR;
    else if (ptx == PTD)
        portx = PORTD_BASE_PTR;
    else if (ptx == PTE)
        portx = PORTE_BASE_PTR;
    else if (ptx == PTF)
        portx = PORTF_BASE_PTR;
    else
        return 0;
    //配置所选引脚的控制寄存器
    for (i =0; i < 32; i++)
    {
        if (pins&(1ul<<i))
            portx->PCR[i] &= ~(PORT_PCR_IRQC_MASK);
    }
    return 1;
}

/*
 * /brief 设置GPIO端口0~31的输出
 * /param ptx   --
 *        data32 --
 * /return 无
 */
void LPLD_GPIO_Output(GPIO_MemMapPtr ptx, uint32 data32)
{
    ptx->PDOR |= data32;
}

void LPLD_GPIO_Output_b (GPIO_MemMapPtr ptx, uint8 lsb_num, uint8 data1)
{
    ptx->PDOR = (ptx -> PDOR & ~(0x01L << lsb_num)) | ((uint32) data1 << lsb_num);
}

/*
 * /brief 设置GPIO端口0~31的电平翻转
 * /param ptx --
 *        data32 --
 * /return 无
 */
void LPLD_GPIO_Toggle(GPIO_MemMapPtr ptx, uint32 data32)
{
    ptx->PTOR |= data32;
}

void LPLD_GPIO_Toggle_b(GPIO_MemMapPtr ptx, uint8 lsb_num)
{
    ptx -> PTOR |= 0x01L << lsb_num;
}

/*
 * /brief 读取GPIO端口0~31的数据
 * /param ptx --
 * /return 取得的32为数据
 */
uint32 LPLD_GPIO_Input(GPIO_MemMapPtr ptx)
{
    uint32 tmp;
    tmp = ptx->PDIR;
    return (tmp);
}

uint8 LPLD_GPIO_Input_b (GPIO_MemMapPtr ptx, uint8 lsb_num)
{
    return (uint8) ((ptx -> PDIR >> lsb_num) & 0x01L);
}

/*
 * PORTA-PORTE中断处理函数
 */
void PORTA_IRQHandler (void)
{
    GPIO_ISR[0]();
    PORTA_ISFR = 0xFFFFFFFF;
}
void PORTB_IRQHandler (void)
{
    GPIO_ISR[1]();
    PORTB_ISFR = 0xFFFFFFFF;
}
void PORTC_IRQHandler (void)
{
    GPIO_ISR[2]();
    PORTC_ISFR = 0xFFFFFFFF;
}

void PORTD_IRQHandler (void)
{
    GPIO_ISR[3]();
    PORTD_ISFR = 0xFFFFFFFF;
}

void PORTE_IRQHandler (void)
{
    GPIO_ISR[4]();
    PORTE_ISFR = 0xFFFFFFFF;
}

