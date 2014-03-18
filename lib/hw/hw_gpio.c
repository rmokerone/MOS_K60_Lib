/**********************************************************************
	> File Name: hw_gpio.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Tue 18 Mar 2014 11:58:07 AM CST
 *********************************************************************/

#include "hw_gpio.h"

/*
 * \brief GPIO通用初始化函数
 * \param gpio_init_structure
 * \return 0--配置失败
 *         1--配置成功
 */
int8 MOS_GPIO_Init(GPIO_InitTypeDef gpio_init_structure)
{
    uint8 i;

    PORT_MemMapPtr portx;
    uint32 pcr = PORT_PCR_MUX(1)
                |gpio_init_structure.GPIO_PinControl;
    GPIO_MemMapPtr ptx = gpio_init_structure.GPIO_PTx;
    uint32 pins = gpio_init_structure.GPIO_Pins;
    uint8 dir = gpio_init_structure.GPIO_Dir;
    uint8 output = gpio_init_structure.GPIO_Output;
    

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
    else if (ptx == PTD)
        portx = PORTF_BASE_PTR;
    else
        return 0;
    
    if (dir == DIR_OUTPUT)
    {
        ptx->PDDR = pins;
        if (output == OUTPUT_H)
            ptx->PSOR = pins;
        else
            ptx->PCOR = pins;
    }
    else
    {
        ptx->PDDR &= ~(pins);
    }
    for (i =0; i < 32; i++)
    {
        if (pins&(1ul<<i))
            portx->PCR[i] = pcr;
    }

    return 1;
}

/*
 * /brief 设置GPIO端口0~31的输出
 * /param ptx   --
 *        data32 --
 * /return 无
 */
void MOS_GPIO_Output(GPIO_MemMapPtr ptx, uint32 data32)
{
    ptx->PDOR = data32;
}

/*
 * /brief 设置GPIO端口0~31的电平翻转
 * /param ptx --
 *        data32 --
 * /return 无
 */
void MOS_GPIO_Toggle(GPIO_MemMapPtr ptx, uint32 data32)
{
    ptx->PTOR = data32;
}

/*
 * /brief 读取GPIO端口0~31的数据
 * /param ptx --
 * /return 取得的32为数据
 */
uint32 MOS_GPIO_Input(GPIO_MemMapPtr ptx)
{
    uint32 tmp;
    tmp = ptx->PDIR;
    return (tmp);
}
