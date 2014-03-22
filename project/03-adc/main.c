/**********************************************************************
	> File Name: main.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Thu 20 Mar 2014 01:09:34 AM CST
 *********************************************************************/

#include "common.h"

void adc_init (void);
void delay();
int16 convert (int16);
//获取陀螺仪的ADC取样数据
int16 adc_get_ave (void);
//采样结果转换成角速度
int16 adc_result_to_velocity (int16 ar);

#define STATIC_ADC_RESULT 46 

ADC_InitTypeDef adc_init_struct;

int main (void)
{
    uint16 result;
    adc_init();
    while (1)
    {
        result = adc_get_ave();
        printf ("= %ddeg/s\n", adc_result_to_velocity(result - STATIC_ADC_RESULT));
    }
    return 0;
}

void delay(void)
{
    int i,j;
    for (i = 0xff; i > 0; i --)
        for (j = 0xfff; j > 0; j --);
}

void adc_init (void)
{
    adc_init_struct.ADC_Adcx = ADC0;
    adc_init_struct.ADC_DiffMode = ADC_SE;
    adc_init_struct.ADC_BitMode = SE_12BIT;
    adc_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;
    adc_init_struct.ADC_HwAvgSel = HW_4AVG;
    adc_init_struct.ADC_CalEnable = TRUE;

    LPLD_ADC_Init (adc_init_struct);
    LPLD_ADC_Chn_Enable (ADC0, AD8);

}

int16 convert (int16 result)
{
    int16 data;
    data = (result - 1) * (float)((float)330 /4094);
    return data;
}

int16 adc_get_ave(void)
{
    //取样的次数
    const char cnt = 10;
    int32 temp;
    uint i;
    for (i = cnt; i > 0; i--)
    {
        temp += LPLD_ADC_Get(ADC0, AD8);
    }
    temp /= cnt;
    return (int16)temp;
}

int16 adc_result_to_velocity (int16 ar)
{
    float vr;
    vr = (float)ar * 3300.0 / 4096.0;
    vr = vr / (0.67 * 9);
    return (int16)vr;
}
