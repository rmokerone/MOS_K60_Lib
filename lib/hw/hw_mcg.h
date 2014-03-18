/**********************************************************************
	> File Name: hw_mcg.h
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Tue 18 Mar 2014 08:45:08 PM CST
 *********************************************************************/

#ifndef __HW_MCG_H__
#define __HW_MCG_H__

#include <common.h>

typedef enum PllOptionEnum
{
  PLL_48=48u,    //如果使用USB模块，必须选择48的倍数主频
  PLL_50=50u,
  PLL_96=96u,    //如果使用USB模块，必须选择48的倍数主频
  PLL_100=100u,  //100MHz是MK60DZ10的额定最高主频，再往上飞思卡尔可不质保
  PLL_120=120u,
  PLL_150=150u,
  PLL_180=180u,
  PLL_200=200u   //如果超到200，发热很正常，烧了别找我>_>
  //高于200Mhz的内核频率不建议使用，MK60DZ10到不了煎蛋的水平^_^
} PllOptionEnum_Type;

//初始化内核时钟及其他系统时钟
extern uint8 LPLD_PLL_Setup(PllOptionEnum_Type);
//设置系统时钟分频
void LPLD_Set_SYS_DIV(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4);
   
extern uint32 g_core_clock;
extern uint32 g_bus_clock;
extern uint32 g_flash_clock;
extern uint32 g_flexbus_clock;   

#endif
