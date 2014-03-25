/**
 * @file HW_LPTMR.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief LPTMR�ײ�ģ����غ���
 *
 * ���Ľ���:�������޸�
 *
 * ��Ȩ����:�����������µ��Ӽ������޹�˾
 * http://www.lpld.cn
 * mail:support@lpld.cn
 *
 * @par
 * ����������������[LPLD]������ά������������ʹ���߿���Դ���롣
 * �����߿���������ʹ�û��Դ���롣�����μ�����ע��Ӧ���Ա�����
 * ���ø��Ļ�ɾ��ԭ��Ȩ���������������ο����߿��Լ�ע���ΰ�Ȩ�����ߡ�
 * ��Ӧ�����ش�Э��Ļ����ϣ�����Դ���롢���ó��۴��뱾��
 * �������²���������ʹ�ñ��������������κ��¹ʡ��������λ���ز���Ӱ�졣
 * ����������������͡�˵��������ľ���ԭ�����ܡ�ʵ�ַ�����
 * ������������[LPLD]��Ȩ�������߲��ý�������������ҵ��Ʒ��
 */

#include "common.h"
#include "hw_lptmr.h"

LPTMR_ISR_CALLBACK LPTMR_ISR[1];

/*
 * LPLD_LPTMR_Init
 * LPTMR��ʼ������
 * 
 * ����:
 *    lptmr_init_structure--LPTMR��ʼ���ṹ�壬
 *                         ���嶨���LPTMR_InitTypeDef 
 * ���:
 *    0--���ô���
 *    1--���óɹ�
 */
uint8 LPLD_LPTMR_Init(LPTMR_InitTypeDef lptmr_init_structure)
{
  uint8  mode = lptmr_init_structure.LPTMR_Mode; 
  uint16 period_ms = lptmr_init_structure.LPTMR_PeriodMs;
  uint8  pluseacc_input = lptmr_init_structure.LPTMR_PluseAccInput;
  LPTMR_ISR_CALLBACK isr_func = lptmr_init_structure.LPTMR_Isr;

//  ASSERT(mode <= LPTMR_MODE_PLACC );
 // ASSERT(pluseacc_input <= LPTMR_ALT2 );
    if (mode > LPTMR_MODE_PLACC)
        return 0;
    if (pluseacc_input > LPTMR_ALT2)
        return 0;
  //ʹ��OSCERCLK
  OSC0_CR |= OSC_CR_ERCLKEN_MASK;
  //��ʼ��LPTMRʱ��
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;  
   
  //��ռĴ���
  LPTMR0_CSR = 0x00;                   
  LPTMR0_PSR = 0x00;
  LPTMR0_CMR = 0x00;
   
  if(mode == LPTMR_MODE_TIMER)          // ���óɵ͹��Ķ�ʱ������
  {    
    /* ����LPTMR */
    // LPO 1Khz
    //���ñȽϵ�ֵ
    //ʹ���ڲ�ʱ�� ��Ƶϵ��PrescaleΪ 2^(X+1)
    //��ʱ����һ����������=ʱ��Դ(Clk_Src)/ Ԥ��Ƶֵ(Prescale)
    //��ʱ���Ķ�ʱֵ=���õıȽ�ֵ(compare_value)/��ʱ����һ����������
    LPTMR0_CMR = LPTMR_CMR_COMPARE(period_ms);                 //���ñȽ�ֵ
    //LPTMR0_PSR = LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK;       //����LPOʱ����·ʹ��
    LPTMR0_PSR = (0 | LPTMR_PSR_PCS(3)
                    | LPTMR_PSR_PRESCALE(4)
                 );
        
  }
  else if(mode == LPTMR_MODE_PLACC)             //���ó����岶��ģʽ
  {
    
    if(pluseacc_input == LPTMR_ALT1)            //PTA19����
    {
      PORTA->PCR[19] = PORT_PCR_MUX(0x6);       //����PTA19Ϊ�����ۼ�
    }
    else if(pluseacc_input == LPTMR_ALT2)       //PTC5����
    {
      PORTC->PCR[5] = PORT_PCR_MUX(0x4);        //����PTC5Ϊ�����ۼ�  
    }

    //�����õ��ڲ�ʱ���޹�
    //�ⲿ������ź���Ϊ����ʱ��
     LPTMR0_PSR = (0 | LPTMR_PSR_PCS(3)
                    | LPTMR_PSR_PRESCALE(4)
                 );
  //LPTMR0_PSR = LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; 
    LPTMR0_CSR = LPTMR_CSR_TPS(pluseacc_input);
    LPTMR0_CSR |= LPTMR_CSR_TMS_MASK; //����ģʽ
    LPTMR0_CSR |= LPTMR_CSR_TFC_MASK; //���ɼ���ģʽ
  }
  
  if(lptmr_init_structure.LPTMR_IntEnable == TRUE && isr_func != NULL)
  {
    LPTMR_ISR[0] = isr_func;
    LPTMR0_CSR |= LPTMR_CSR_TIE_MASK;
  }

  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK; //����LPTMR
  return 1;
}

/*
 * LPLD_LPTMR_DeInit
 * LPTMR����ʼ���������ر�lptmrx
 * 
 * ����:
 *    ��
 *
 * ���:
 *    0--���ô���
 *    1--���óɹ�
 */
uint8 LPLD_LPTMR_Deinit(void)
{
  //���CSR�Ĵ���
  LPTMR0_CSR=0x00;  
  //���PSR�Ĵ���
  LPTMR0_PSR=0x00;
  //���CMR�Ĵ���
  LPTMR0_CMR=0x00;
  //��ֹLPTimer�ж�
  disable_irq(INT_LPTimer - 16);
  
   //�ر�LPTMRʱ��
  SIM_SCGC5 &= ~(SIM_SCGC5_LPTIMER_MASK);  
  
  return 1;
}

/*
 *  LPLD_LPTMR_Reset
 *  �͹��Ķ�ʱ����λ����λ�Ժ���յ͹��Ķ�ʱ��Counter
 * 
 *  ����:
 *      ��
 *  ���:
 *      ��
 */
void LPLD_LPTMR_ResetCounter(void)
{
  LPTMR0_CSR&=(~LPTMR_CSR_TEN_MASK);       //Reset LPTMR0 Counter
}
/*
 * LPLD_LPTMR_EnableIrq
 * ʹ��LPTMR�жϹ��ܣ��ڸú�����ʹ��TIEλ��LPTMRģ���ж�
 * 
 * ����:
 *    ��
 *
 * ���:
 *    0--���ô���
 *    1--���óɹ�
 *
 */
uint8 LPLD_LPTMR_EnableIrq(void)
{
  enable_irq(INT_LPTimer - 16);
  return 1;
}

/*
 * LPLD_LPTMR_DisableIrq
 * ��ֹLPTMR�ж�
 * 
 * ����:
 *    ��
 *
 * ���:
 *    0--���ô���
 *    1--���óɹ�
 *
 */
uint8 LPLD_LPTMR_DisableIrq(void)
{
  disable_irq(INT_LPTimer - 16); 
  return 1;
}

/*
 *  LPLD_LPTMR_GetPulseAcc
 *  �õ������ۼӵ�ֵ
 * 
 *  ����:
 *      ��
 *  ���:
 *      �����ۼ�ֵ
 */
uint16 LPLD_LPTMR_GetPulseAcc(void)
{
    LPTMR0_CNR = 0;
   return (uint16)LPTMR0_CNR;
}

/*
 *  LPLD_LPTMR_DelayMs
 *  �͹��Ķ�ʱ����ʱn��ms
 * 
 * ����:
 *    period_ms--�趨��ʱ���Ķ�ʱֵ ��λms
 *    ���65535ms
 *
 *  ���:
 *      ��
 */
void LPLD_LPTMR_DelayMs(uint16 period_ms)
{
  if(!period_ms)
    return;
  
  OSC0_CR |= OSC_CR_ERCLKEN_MASK;
  /* ����LPTMRʱ�� */
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK; 
  
  /* ���ú�����ʱ���� */
  LPTMR0_CMR = period_ms; 
  
  /* ����LPTMR����Ƶ����1kHz LPOΪʱ��Դ */
  LPTMR0_PSR = 0|LPTMR_PSR_PCS(1)|LPTMR_PSR_PBYP_MASK; 
  
  /* ������ʱ�� */
  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK; 

  /* �ȴ������ﵽ�趨ֵ */
  while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK));
  
  /*�����ʱ�Ƚϱ�־ */
  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;
  
}


/*
 * LPTMR�жϴ�����
 * �������ļ�startup_K60.s�е��ж����������
 * �û������޸ģ������Զ������Ӧͨ���жϺ���
 */
void LPT_IRQHandler(void)
{
  LPTMR0_CSR|=LPTMR_CSR_TCF_MASK;  //���LPT�Ƚϱ�־
  LPTMR_ISR[0]();
}
