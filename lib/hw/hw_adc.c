/**********************************************************************
	> File Name: hw_adc.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Thu 20 Mar 2014 01:20:53 AM CST
 *********************************************************************/

#include "common.h"
#include "hw_adc.h"

//用户自定义中断服务函数数组
ADC_ISR_CALLBACK ADC_ISR[2];

static uint8 LPLD_ADC_Cal(ADC_MemMapPtr);

/*
 * LPLD_ADC_Init
 * ADC通用初始化函数，选择ADCx、采集模式、精度等参数
 * 
 * 参数:
 *    adc_init_structure--ADC初始化结构体，
 *                        具体定义见ADC_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_ADC_Init(ADC_InitTypeDef adc_init_structure)
{
  uint8 i;
  ADC_MemMapPtr adcx = adc_init_structure.ADC_Adcx;
  uint8 diff = adc_init_structure.ADC_DiffMode;
  uint8 mode = adc_init_structure.ADC_BitMode;
  uint8 time = adc_init_structure.ADC_SampleTimeCfg;
  uint8 ltime = adc_init_structure.ADC_LongSampleTimeSel;
  uint8 avg = adc_init_structure.ADC_HwAvgSel;
  uint8 muxab = adc_init_structure.ADC_MuxSel;
  uint8 pga = adc_init_structure.ADC_PgaGain;
  uint8 hwtrg = adc_init_structure.ADC_HwTrgCfg;
  ADC_ISR_CALLBACK isr_func = adc_init_structure.ADC_Isr;
 
  //配置ADC时钟 //这里貌似只配置了两个还要另行配置其他的几个
  if(adcx == ADC0)
  {
    i=0;
    SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;   // 开启ADC0时钟
  }
  else if(adcx == ADC1)
  {
    i=1;
    SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK;   // 开启ADC1时钟
  }
  else 
  {
    return 0;
  }

  if(adc_init_structure.ADC_CalEnable == TRUE)
    LPLD_ADC_Cal(adcx);  //进行ADC校准
  
  //设置ADCCFG1寄存器
  adcx->CFG1  &=   (~ADC_CFG1_ADLPC_MASK);         // 重新为正常使用进行配置
  adcx->CFG1  |=    ADC_CFG1_ADIV(ADIV_1)          // ADC输入时钟分频为 1
                  | (time)                           // 设置长短时间采样模式
                  | ADC_CFG1_ADICLK(ADICLK_BUS_2)  // ADC输入时钟源为 BusClk
                  | ADC_CFG1_MODE(mode);           //设置ADC转换精度

  //设置ADCCFG2寄存器
  adcx->CFG2 &=  (~ADC_CFG2_ADACKEN_MASK);   //禁用异步时钟输出
  adcx->CFG2 |=    (muxab)                      // ADC复用选择
                 | (ADC_CFG2_ADHSC_MASK)        // 高速转换
                 | ADC_CFG2_ADLSTS(ltime);    // 长采样时间时钟周期选择
                                              // 总采样周期见K60技术文档 page:840
  //用于设定阈值
  adcx->CV1  = 0x1234u ; 
  adcx->CV2  = 0x5678u ;
 
  adcx->SC2  &= ((~ADC_SC2_ACFE_MASK)     //关闭比较功能 
                 & (~ADC_SC2_DMAEN_MASK)); //关闭DMA
  adcx->SC2  |=  (hwtrg & ADC_SC2_ADTRG_MASK) //设置触发方式
                 |  ADC_SC2_ACREN_MASK          //使能范围比较
                 |  ADC_SC2_ACFGT_MASK          //使能大于比较功能
                 |  ADC_SC2_REFSEL(REFSEL_EXT); //选择外部参考源VREFH和VREFL
  
  if(adc_init_structure.ADC_DmaEnable == TRUE) 
    adcx->SC2  |= ADC_SC2_DMAEN_MASK;           //使能DMA

  adcx->SC3  &=  (~ADC_SC3_CAL_MASK)          //关闭校准
                 & (~ADC_SC3_ADCO_MASK);         //选择一次转换
  adcx->SC3  |=  avg;                        //硬件平均
  
  adcx->PGA  = pga<<ADC_PGA_PGAG_SHIFT; 
  
  //校准完毕后再重新初始化ADC寄存器
  //adcx->SC1[0] = ADC_SC1_ADCH(AD31);    //复位SC1
  adcx->SC1[hwtrg & 0x01] = diff;         //设置单端、差分输入
  
  if(isr_func!= NULL)
  {
    ADC_ISR[i] = isr_func;
  }
  
  return 1;
}

/*
 * LPLD_ADC_Deinit
 * ADC反初始化函数，禁用ADC模块
 * 
 * 参数:
 *    adc_init_structure--ADC初始化结构体，
 *                        具体定义见ADC_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_ADC_Deinit(ADC_InitTypeDef adc_init_structure)
{
  ADC_MemMapPtr adcx = adc_init_structure.ADC_Adcx;
  uint8 hwtrg = adc_init_structure.ADC_HwTrgCfg;
  
  adcx->SC1[hwtrg & 0x01] = ADC_SC1_ADCH(AD31);    //复位SC1
  
  //配置ADC时钟
  if(adcx == ADC0)
  {
    SIM_SCGC6 &= ~(SIM_SCGC6_ADC0_MASK);   // 关闭ADC0时钟
  }
  else if(adcx == ADC1)
  {
    SIM_SCGC3 &= ~(SIM_SCGC3_ADC1_MASK);   // 关闭ADC1时钟
  }
  else 
  {
    return 0;
  }
  
  return 1;
}

/*
 * LPLD_ADC_Get
 * 软件触发模式获取AD转换结果
 * 
 * 参数:
 *    adcx--ADCx模块号
 *      |__ADC0          --ADC0
 *      |__ADC1          --ADC1
 *    chn--ADC采集输入通道,详见技术手册K60P144M100SF2RM第118页
 *      ADC0
 *      |__DAD0          --差分(ADC0_DP0和ADC0_DM0)/单端(ADC0_DP0)
 *      |__DAD1          --差分(ADC0_DP1和ADC0_DM1)/单端(ADC0_DP1)
 *      |__DAD2          --差分(PGA0_DP和PGA0_DM)/单端(PGA0_DP)
 *      |__DAD3          --差分(ADC0_DP3和ADC0_DM3)/单端(ADC0_DP3)
 *      |__AD4           --单端A(保留)/单端B(ADC0_SE4b--PTC2)
 *      |__AD5           --单端A(保留)/单端B(ADC0_SE5b--PTD1)
 *      |__AD6           --单端A(保留)/单端B(ADC0_SE6b--PTD5)
 *      |__AD7           --单端A(保留)/单端B(ADC0_SE7b--PTD6)
 *      |__AD8           --单端(ADC0_SE8--PTB0)
 *      |__AD9           --单端(ADC0_SE9--PTB1)
 *      |__AD10          --单端(ADC0_SE10--PTA7)
 *      |__AD11          --单端(ADC0_SE11--PTA8)
 *      |__AD12          --单端(ADC0_SE12--PTB2)
 *      |__AD13          --单端(ADC0_SE13--PTB3)
 *      |__AD14          --单端(ADC0_SE14--PTC0)
 *      |__AD15          --单端(ADC0_SE15--PTC1)
 *      |__AD16          --单端(ADC0_SE16)
 *      |__AD17          --单端(ADC0_SE17-PTE24)
 *      |__AD18          --单端(ADC0_SE18--PTE25)
 *      |__AD19          --单端(ADC0_DM0)
 *      |__AD20          --单端(ADC0_DM1)
 *      |__AD23          --单端(DAC0)
 *      |__AD26          --温度传感器
 *      |__AD27          --Bandgap
 *      |__AD29          --差分(-VREFH)/单端(VREFH)
 *      |__AD30          --单端(VREFL)
 *      ADC1
 *      |__DAD0          --差分(ADC1_DP0和ADC1_DM0)/单端(ADC1_DP0)
 *      |__DAD1          --差分(ADC1_DP1和ADC1_DM1)/单端(ADC1_DP1)
 *      |__DAD2          --差分(PGA1_DP和PGA1_DM)/单端(PGA1_DP)
 *      |__DAD3          --差分(ADC1_DP3和ADC1_DM3)/单端(ADC1_DP3)
 *      |__AD4           --单端A(ADC1_SE4a--PTE0)/单端B(ADC1_SE4b--PTC8)
 *      |__AD5           --单端A(ADC1_SE5a--PTE1)/单端B(ADC1_SE5b--PTC9)
 *      |__AD6           --单端A(ADC1_SE6a--PTE2)/单端B(ADC1_SE6b--PTC10)
 *      |__AD7           --单端A(ADC1_SE7a--PTE3)/单端B(ADC1_SE7b--PTC11)
 *      |__AD8           --单端(ADC1_SE8--PTB0)
 *      |__AD9           --单端(ADC1_SE9--PTB1)
 *      |__AD10          --单端(ADC1_SE10--PTB4)
 *      |__AD11          --单端(ADC1_SE11--PTB5)
 *      |__AD12          --单端(ADC1_SE12--PTB6)
 *      |__AD13          --单端(ADC1_SE13--PTB7)
 *      |__AD14          --单端(ADC1_SE14--PTB10)
 *      |__AD15          --单端(ADC1_SE15--PTB11)
 *      |__AD16          --单端(ADC1_SE16)
 *      |__AD17          --单端(ADC1_SE17--PTA17)
 *      |__AD18          --单端(VREF)
 *      |__AD19          --单端(ADC1_DM0)
 *      |__AD20          --单端(ADC1_DM1)
 *      |__AD23          --单端(DAC1)
 *      |__AD26          --温度传感器
 *      |__AD27          --Bandgap
 *      |__AD29          --差分(-VREFH)/单端(VREFH)
 *      |__AD30          --单端(VREFL)
 *
 * 输出:
 *    AD通道转换值(右对齐)，若为差分转换结果，则为二进制补码格式(需强制转换为int16)
 *
 */
uint16 LPLD_ADC_Get(ADC_MemMapPtr adcx, AdcChnEnum_Type chn)
{
  adcx->SC1[0] &= ~(ADC_SC1_AIEN_MASK);
  adcx->SC1[0] &= ~(ADC_SC1_ADCH_MASK);
  adcx->SC1[0] |= ADC_SC1_ADCH(chn);
  while((adcx->SC1[0]&ADC_SC1_COCO_MASK) == 0); //等待转换完成  
  return adcx->R[0];
}


/*
 * LPLD_ADC_EnableConversion
 * 使能ADCx转换通道，用于软件中断、硬件触发等模式
 * 
 * 参数:
 *    adcx--ADCx模块号
 *      |__ADC0          --ADC0
 *      |__ADC1          --ADC1
 *    chn--ADC采集输入通道,详见技术手册K60P144M100SF2RM第118页
 *      取值同LPLD_ADC_Get形参chn
 *    ab--SC1寄存器A、B通道选择
 *      |__0          --A转换通道
 *      |__1          --B转换通道
 *    irq--是否使能转换完成中断
 *      |__TRUE       --使能转换完成中断
 *      |__FALSE      --禁用转换完成中断
 *
 * 输出:
 *    无
 *
 */
void LPLD_ADC_EnableConversion(ADC_MemMapPtr adcx, AdcChnEnum_Type chn, uint8 ab, boolean irq)
{
  if(irq == TRUE)
  {
    adcx->SC1[ab] |= (ADC_SC1_AIEN_MASK);
  }
  adcx->SC1[ab] &= ~(ADC_SC1_ADCH_MASK);
  adcx->SC1[ab] |= ADC_SC1_ADCH(chn);
}


/*
 * LPLD_ADC_GetResult
 * 获取AD转换结果
 * 
 * 参数:
 *    adcx--ADCx模块号
 *      |__ADC0          --ADC0
 *      |__ADC1          --ADC1
 *    ab--SC1寄存器A、B通道选择
 *      |__0          --A转换通道
 *      |__1          --B转换通道
 *
 * 输出:
 *    AD通道转换值(右对齐)，若为差分转换结果，则为二进制补码格式(需强制转换为int16)
 *
 */
uint16 LPLD_ADC_GetResult(ADC_MemMapPtr adcx, uint8 ab)
{
  //参数检查
  return adcx->R[ab];
}


/*
 * LPLD_ADC_GetSC1nCOCO
 * 获取SC1寄存器中COCO位置1的编号
 * 
 * 参数:
 *    adcx--ADCx模块号
 *      |__ADC0          --ADC0
 *      |__ADC1          --ADC1
 *
 * 输出:
*    0--SC1A寄存器COCO位置1
*    1--SC1B寄存器COCO位置1
 *
 */
uint8 LPLD_ADC_GetSC1nCOCO(ADC_MemMapPtr adcx)
{
  if(adcx->SC1[0] & ADC_SC1_COCO_MASK)
    return 0;
  if(adcx->SC1[1] & ADC_SC1_COCO_MASK)
    return 1;
  return -1;
}


/*
 * LPLD_ADC_EnableIrq
 * 使能ADCx中断
 * 
 * 参数:
 *    adc_init_structure--PIT初始化结构体，
 *                        具体定义见ADC_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 LPLD_ADC_EnableIrq(ADC_InitTypeDef adc_init_structure)
{
  ADC_MemMapPtr adcx = adc_init_structure.ADC_Adcx;
  
  if(adcx == ADC0)
    enable_irq(INT_ADC0 - 16);
  else if(adcx == ADC1)
    enable_irq (INT_ADC1 - 16);
  else
    return 0;
  return 1;
}

/*
 * LPLD_ADC_DisableIrq
 * 禁用ADCx中断
 * 
 * 参数:
 *    adc_init_structure--PIT初始化结构体，
 *                        具体定义见ADC_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 LPLD_ADC_DisableIrq(ADC_InitTypeDef adc_init_structure)
{
  ADC_MemMapPtr adcx = adc_init_structure.ADC_Adcx;
  
  if(adcx == ADC0)
      disable_irq(INT_ADC0 - 16);
  else if(adcx == ADC1)
      disable_irq(INT_ADC1 - 16);
  else
    return 0;

  return 1;
}


/*
 * LPLD_ADC_Chn_Enable
 * 使能ADCx相应转换通道的AD采集功能
 * 
 * 参数:
 *    adcx--ADCx模块号
 *      |__ADC0          --ADC0
 *      |__ADC1          --ADC1
 *    chn--ADC采集输入通道,详见技术手册K60P144M100SF2RM第118页
 *      ADC0
 *      |__DAD0          --差分(ADC0_DP0和ADC0_DM0)/单端(ADC0_DP0)
 *      |__DAD1          --差分(ADC0_DP1和ADC0_DM1)/单端(ADC0_DP1)
 *      |__DAD2          --差分(PGA0_DP和PGA0_DM)/单端(PGA0_DP)
 *      |__DAD3          --差分(ADC0_DP3和ADC0_DM3)/单端(ADC0_DP3)
 *      |__AD4           --单端A(保留)/单端B(ADC0_SE4b--PTC2)
 *      |__AD5           --单端A(保留)/单端B(ADC0_SE5b--PTD1)
 *      |__AD6           --单端A(保留)/单端B(ADC0_SE6b--PTD5)
 *      |__AD7           --单端A(保留)/单端B(ADC0_SE7b--PTD6)
 *      |__AD8           --单端(ADC0_SE8--PTB0)
 *      |__AD9           --单端(ADC0_SE9--PTB1)
 *      |__AD10          --单端(ADC0_SE10--PTA7)
 *      |__AD11          --单端(ADC0_SE11--PTA8)
 *      |__AD12          --单端(ADC0_SE12--PTB2)
 *      |__AD13          --单端(ADC0_SE13--PTB3)
 *      |__AD14          --单端(ADC0_SE14--PTC0)
 *      |__AD15          --单端(ADC0_SE15--PTC1)
 *      |__AD16          --单端(ADC0_SE16)
 *      |__AD17          --单端(ADC0_SE17-PTE24)
 *      |__AD18          --单端(ADC0_SE18--PTE25)
 *      |__AD19          --单端(ADC0_DM0)
 *      |__AD20          --单端(ADC0_DM1)
 *      |__AD23          --单端(DAC0)
 *      |__AD26          --温度传感器
 *      |__AD27          --Bandgap
 *      |__AD29          --差分(-VREFH)/单端(VREFH)
 *      |__AD30          --单端(VREFL)
 *      ADC1
 *      |__DAD0          --差分(ADC1_DP0和ADC1_DM0)/单端(ADC1_DP0)
 *      |__DAD1          --差分(ADC1_DP1和ADC1_DM1)/单端(ADC1_DP1)
 *      |__DAD2          --差分(PGA1_DP和PGA1_DM)/单端(PGA1_DP)
 *      |__DAD3          --差分(ADC1_DP3和ADC1_DM3)/单端(ADC1_DP3)
 *      |__AD4           --单端A(ADC1_SE4a--PTE0)/单端B(ADC1_SE4b--PTC8)
 *      |__AD5           --单端A(ADC1_SE5a--PTE1)/单端B(ADC1_SE5b--PTC9)
 *      |__AD6           --单端A(ADC1_SE6a--PTE2)/单端B(ADC1_SE6b--PTC10)
 *      |__AD7           --单端A(ADC1_SE7a--PTE3)/单端B(ADC1_SE7b--PTC11)
 *      |__AD8           --单端(ADC1_SE8--PTB0)
 *      |__AD9           --单端(ADC1_SE9--PTB1)
 *      |__AD10          --单端(ADC1_SE10--PTB4)
 *      |__AD11          --单端(ADC1_SE11--PTB5)
 *      |__AD12          --单端(ADC1_SE12--PTB6)
 *      |__AD13          --单端(ADC1_SE13--PTB7)
 *      |__AD14          --单端(ADC1_SE14--PTB10)
 *      |__AD15          --单端(ADC1_SE15--PTB11)
 *      |__AD16          --单端(ADC1_SE16)
 *      |__AD17          --单端(ADC1_SE17--PTA17)
 *      |__AD18          --单端(VREF)
 *      |__AD19          --单端(ADC1_DM0)
 *      |__AD20          --单端(ADC1_DM1)
 *      |__AD23          --单端(DAC1)
 *      |__AD26          --温度传感器
 *      |__AD27          --Bandgap
 *      |__AD29          --差分(-VREFH)/单端(VREFH)
 *      |__AD30          --单端(VREFL)
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 LPLD_ADC_Chn_Enable(ADC_MemMapPtr adcx, AdcChnEnum_Type chn)
{
  //判断复用引脚是a或b
  uint8 mux = (adcx->CFG2 & ADC_CFG2_MUXSEL_MASK)>>ADC_CFG2_MUXSEL_SHIFT;
    
  if(chn > AD30)
    return 0;
  
  //不同的通道对应不同的引脚，因此需要判断并配置
  if(adcx == ADC0)
  {
    switch(chn)
    {
      case DAD0:   //ADC0_DP0 -- PGA0_DP
      case DAD1:   //ADC0_DP1 -- PGA2_DP
      case DAD2:   //PGA0_DP
      case DAD3:   //ADC0_DP3 -- PGA1_DP
        break;
      case AD4:   //ADC0_SE4b -- PTC2     
        if(mux == 1)    //b
          PORTC->PCR[2] =  PORT_PCR_MUX(0);
        break;
      case AD5:   //ADC0_SE5b -- PTD1     
        if(mux == 1)    //b
          PORTD->PCR[1] =  PORT_PCR_MUX(0);
        break;
      case AD6:   //ADC0_SE6b -- PTD5
      case AD7:   //ADC0_SE7b -- PTD6     
        if(mux == 1)    //b
          PORTD->PCR[chn-1] =  PORT_PCR_MUX(0);
        break;
      case AD8:  //ADC0_SE8 -- PTB0
      case AD9:  //ADC0_SE9 -- PTB1
        PORTB->PCR[chn-8] =  PORT_PCR_MUX(0);
        break;
      case AD10:  //ADC0_SE10 -- PTA7
      case AD11:  //ADC0_SE11 -- PTA8
        PORTA->PCR[chn-3] =  PORT_PCR_MUX(0);
        break;
      case AD12:  //ADC0_SE12 -- PTB2
      case AD13:  //ADC0_SE13 -- PTB3
        PORTB->PCR[chn-10] =  PORT_PCR_MUX(0);
        break;
      case AD14:  //ADC0_SE14 -- PTC0
      case AD15:  //ADC0_SE15 -- PTC1
        PORTC->PCR[chn-14] =  PORT_PCR_MUX(0);
        break;
      case AD16:   //ADC0_SE16
        break;
      case AD17:   //ADC0_SE17 -- PTE24
      case AD18:   //ADC0_SE18 -- PTE25
        PORTE->PCR[chn+7] =  PORT_PCR_MUX(0);
        break;
      case AD19:   //ADC0_DM0 -- PGA0_DM
      case AD20:   //ADC0_DM1 -- PGA2_DM
      case AD23:   //ADC0_SE23 -- DAC0_OUT
      case AD26:   //Temperature Sensor (S.E)
      case AD27:   //Bandgap (S.E)
      case AD29:   //VREFH (S.E)
      case AD30:   //VREFL
        break;
      default:
        return 0;  
    }
  }
  else if(adcx == ADC1)
  {
    switch(chn)
    {
      case DAD0:   //ADC1_DP0 -- PGA1_DP
      case DAD1:   //ADC1_DP1 -- PGA3_DP
      case DAD2:   //PGA1_DP 
      case DAD3:   //ADC1_DP3 -- PGA0_DP
        break;
      case AD4:   //ADC1_SE4a -- PTE0     //ADC1_SE4b -- PTC8
      case AD5:   //ADC1_SE5a -- PTE1     //ADC1_SE5b -- PTC9
      case AD6:   //ADC1_SE6a -- PTE2     //ADC1_SE6b -- PTC10
      case AD7:   //ADC1_SE7a -- PTE3     //ADC1_SE7b -- PTC11
        if(mux == 0)    //a
          PORTE->PCR[chn-4] =  PORT_PCR_MUX(0);
        else            //b
          PORTC->PCR[chn+4] =  PORT_PCR_MUX(0);
        break;
      case AD8:  //ADC1_SE8 -- PTB0
      case AD9:  //ADC1_SE9 -- PTB1
        PORTB->PCR[chn-8] =  PORT_PCR_MUX(0);
        break;
      case AD10:  //ADC1_SE10 -- PTB4
      case AD11:  //ADC1_SE11 -- PTB5
      case AD12:  //ADC1_SE12 -- PTB6
      case AD13:  //ADC1_SE13 -- PTB7
        PORTB->PCR[chn-6] =  PORT_PCR_MUX(0);
        break;
      case AD14:  //ADC1_SE14 -- PTB10
      case AD15:  //ADC1_SE15 -- PTB11
        PORTB->PCR[chn-4] =  PORT_PCR_MUX(0);
        break;
      case AD16:   //ADC1_SE16
        break;
      case AD17:  //ADC1_SE17 -- PTA17
        PORTA->PCR[chn] =  PORT_PCR_MUX(0);
        break;
      case AD18:   //VREF Output
      case AD19:   //ADC1_DM0 -- PGA1_DM
      case AD20:   //ADC1_DM1 -- PGA3_DM
      case AD23:   //DAC1_OUT 
      case AD26:   //Temperature Sensor (S.E)
      case AD27:   //Bandgap (S.E)
      case AD29:   //VREFH (S.E)
      case AD30:   //VREFL
        break;
      default:
        return 0;  
    }
  }
  else
  {
    return 0;
  }
  
  return 1;
}

/*
 * ADC_Cal
 * ADC模块校准函数，内部调用
 * 
 * 参数:
 *    adcx--ADC模块号
 *      |__ADC0         -ADC0模块
 *      |__ADC1         -ADC1模块
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
static uint8 LPLD_ADC_Cal(ADC_MemMapPtr adcx)
{
  uint16 cal_var;
    
  //为自动校准配置参数
  //为提高校准精度，需配置为：
  //32次硬件平均、ADCK不超过4MHz
  //参考高=Vdda、正常功耗模式
  //可忽略的配置：
  //输入通道、转换模式连续功能、比较功能、精度、差分单端
  //设置ADCCFG1寄存器
  adcx->CFG1  &= (~ADC_CFG1_ADLPC_MASK);          // 正常功耗配置
  adcx->CFG1  |=  ADC_CFG1_ADIV(ADIV_8)          // ADC输入时钟分频为8
                  | ADC_CFG1_ADLSMP_MASK           // 设置长时间采样模式
                  | ADC_CFG1_ADICLK(ADICLK_BUS_2); // ADC输入时钟源为 BusClk/2

  adcx->CFG2  &= (~ADC_CFG2_ADACKEN_MASK);
  adcx->CFG1  |=  ADC_CFG2_ADHSC_MASK         // 高速转换
                 | ADC_CFG2_ADLSTS(LSAMTIME_20EX); // 长采样时间时钟周期选择 额外20个时钟周期，共24个ADCK周期
                                               // 总采样周期见K60技术文档 page:840
  //用于设定阈值
  adcx->CV1  = 0x1234u ; 
  adcx->CV2  = 0x5678u ;
  
  adcx->SC2 &=  (~ADC_SC2_ADTRG_MASK);        //使能软件触发作为校准
  adcx->SC2 |=  ADC_SC2_REFSEL(REFSEL_EXT);  //选择外部参考源VREFH和VREFL
    
  adcx->SC3 &= ( ~ADC_SC3_ADCO_MASK & ~ADC_SC3_AVGS_MASK );  //设置单次转换，清除平均标志
  adcx->SC3 |= ( ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(HW_32AVG) );//打开平均标志，设置到最大采样平准
  
  adcx->SC3 |= ADC_SC3_CAL_MASK ;                            //开始校准
  
  while((adcx->SC1[0] & ADC_SC1_COCO_MASK)== 0x00 );         //等待校准完成
  	
  if ((adcx->SC3& ADC_SC3_CALF_MASK) == ADC_SC3_CALF_MASK )
  {  
   return 0;    //检查到校准错误，返回错误
  }
  // Calculate plus-side calibration
  cal_var  = 0x00;
  cal_var  = adcx->CLP0;       
  cal_var += adcx->CLP1;      
  cal_var += adcx->CLP2;      
  cal_var += adcx->CLP3;      
  cal_var += adcx->CLP4;      
  cal_var += adcx->CLPS;      
  cal_var  = cal_var/2;
  cal_var |= 0x8000; // Set MSB
  
  adcx->PG = ADC_PG_PG(cal_var);

  // Calculate minus-side calibration
  cal_var = 0x00;
  cal_var =  adcx->CLM0; 
  cal_var += adcx->CLM1;
  cal_var += adcx->CLM2;
  cal_var += adcx->CLM3;
  cal_var += adcx->CLM4;
  cal_var += adcx->CLMS;
  cal_var = cal_var/2;
  cal_var |= 0x8000; // Set MSB

  adcx->MG   = ADC_MG_MG(cal_var); 
  adcx->SC3 &= ~ADC_SC3_CAL_MASK ; //清除校验标志
  
  return 1;
}

/*
 * ADC0--ADC1中断处理函数
 * 与启动文件startup_K60.s中的中断向量表关联
 * 用户无需修改，程序自动进入对应通道中断函数
 */

void ADC0_IRQHandler(void)
{
    ADC_ISR[0]();
}

void ADC1_IRQHandler(void)
{
    ADC_ISR[1]();
}
