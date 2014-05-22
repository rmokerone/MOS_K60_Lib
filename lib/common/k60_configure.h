/**********************************************************************
	> File Name: k60_configure.h
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Tue 18 Mar 2014 09:53:12 PM CST
 *********************************************************************/

#ifndef __K60_CONFIGURE_H__
#define __K60_CONFIGURE_H__

/*
 * 系统主频定义(CORE_CLK_MHZ)
 * 可选范围:
 * PLL_48        //48MHz 如果使用USB模块，必须选择48的倍数主频
 * PLL_50        //50MHz
 * PLL_96        //96MHz 如果使用USB模块，必须选择48的倍数主频
 * PLL_100       //100MHz 是MK60DZ10的额定最高主频，再往上飞思卡尔可不质保
 * PLL_120       //120MHz
 * PLL_150       //150MHz
 * PLL_180       //180MHz
 * PLL_200       //200MHz 如果超到200，发热很正常，烧了别找我>_>
 *
 * LPLD提示：MK60DZ10不推荐使用100MHz以上频率，可能引起不可修复的故障
 *           此列表为LPLD固件库列出的常用频率，可以满足各种功能、功耗需求
 */
#define CORE_CLK_MHZ        PLL_150       
   
/*
 * 总线频率定义(BUS_CLK_MHZ)
 * 可选范围:
 *   不超过系统主频，但建议不超过100，超过100后会出现跑飞、发热现象
 *
 * LPLD提示：MK60DZ10额定最高总线频率为50MHz
 *           此处设置的为期望值，即最终输出频率不会高于此频率
*/
#define BUS_CLK_MHZ         50u    
   
/*
 * Flex总线频率定义(FLEXBUS_CLK_MHZ)
 * 可选范围:
 *   不超过50
 *
 * LPLD提示：MK60DZ10额定最高Flex总线频率为50MHz
 *           此处设置的为期望值，即最终输出频率不会高于此频率
*/
#define FLEXBUS_CLK_MHZ     50u 
   
/*
 * Flash频率定义(FLASH_CLK_MHZ)
 * 可选范围:
 *   不超过25
 *
 * LPLD提示：MK60DZ10额定最高Flash频率为25MHz
 *           此处设置的为期望值，即最终输出频率不会高于此频率
*/
#define FLASH_CLK_MHZ       25u    

/*
 * 定义printf函数要使用的输出端口 
 */
#define TERM_PORT          UART4
#define TERM_BAUD          9600
#define TERM_TX            PTC15
#define TERM_RX            PTC14
#define TERM_RX_IRQ        FALSE
#define TERM_RX_FUN        NULL

#endif
