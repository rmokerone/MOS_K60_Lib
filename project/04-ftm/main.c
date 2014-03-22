/**********************************************************************
	> File Name: main.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Sat 22 Mar 2014 03:50:32 PM CST
 *********************************************************************/

#include "common.h"

FTM_InitTypeDef ftm_init_struct;

void pwm_init (void);

int main (void)
{
    pwm_init();
    LPLD_FTM_PWM_ChangeDuty (FTM0, FTM_Ch0, 1500);
    while (1);
    {
        //LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0, 1000);
        delay();
        LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0, 1000);
        delay();
    }
    return 0;
}

void pwm_init (void)
{
    ftm_init_struct.FTM_Ftmx = FTM0;
    ftm_init_struct.FTM_Mode = FTM_MODE_PWM;
    ftm_init_struct.FTM_PwmFreq = 5000;

    LPLD_FTM_Init (ftm_init_struct);

    LPLD_FTM_PWM_Enable (FTM0,
                        FTM_Ch0,
                        0,
                        PTC1,
                        ALIGN_LEFT);
}
