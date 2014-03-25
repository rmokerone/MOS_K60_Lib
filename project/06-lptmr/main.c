/**
 * --------------����"��������K60�ײ��V3"�Ĺ��̣�LPLD_PulseAcc��-----------------
 * @file LPLD_PulseAcc.c
 * @version 0.1
 * @date 2013-9-29
 * @brief ����LPTMRģ�飬ʵ�������ۼӹ��ܡ�
 *
 * ��Ȩ����:�����������µ��Ӽ������޹�˾
 * http://www.lpld.cn
 * mail:support@lpld.cn
 * Ӳ��ƽ̨:  LPLD K60 Card / LPLD K60 Nano
 *
 * �����̻���"��������K60�ײ��V3"������
 * ���п�Դ�������"lib"�ļ����£��û����ظ��ĸ�Ŀ¼�´��룬
 * �����û������豣����"project"�ļ����£��Թ����������ļ�������
 * �ײ��ʹ�÷���������ĵ��� 
 *
 */
#include "common.h"

/********************************************
 * ˵��: 
 * ����K60��Ƭ����FTMģ�����100Khz���岨
 * �����岨���뵽LPTMR �����ۼ�ͨ��
 * LPTMR���������۲�ͨ�����ڴ�ӡ��������ۼӽ��
 * Ӳ������˵��:
 * K60��Ƭ������       
 * FTM    -PTC1  -----> PTC5   LPTMR ALT2 
 * ʹ�ô��ڵ������ֲ���������Ϊ115200
 * ʹ�ô��ڵ������ֲ鿴���н����
 ********************************************/

#define PIT_TIMER_PERIOD 10000 //10ms
                               
FTM_InitTypeDef ftm_init_param;
PIT_InitTypeDef pit_init_param;
LPTMR_InitTypeDef lptmr_init_param;
uint16_t GetFreq;
uint16_t GetCnt;

void pit_isr0();
/********************************************************************/

int main (void)
{
  //ע�⣺�ɼ���Ƶ�ʱ������100HZ
  //��ʼ��FTM
  ftm_init_param.FTM_Ftmx = FTM0;
  ftm_init_param.FTM_PwmFreq = 100000;//100Khz
  ftm_init_param.FTM_Mode = FTM_MODE_PWM;
  //��ʼ��PIT
  pit_init_param.PIT_Pitx = PIT0;
  pit_init_param.PIT_PeriodUs = PIT_TIMER_PERIOD;//10ms 0.1khz
  pit_init_param.PIT_Isr = pit_isr0; 
  //LPTMR_MODE_PLACC�����ۼ�ģʽ
  //LPTMR_ALT2�������ΪPTC5
  //�����ж�
  //���жϺ��� 
  lptmr_init_param.LPTMR_Mode = LPTMR_MODE_PLACC;
  lptmr_init_param.LPTMR_PluseAccInput = LPTMR_ALT2;
  lptmr_init_param.LPTMR_Isr = NULL;
  //FTM,PIT,LPTMR��ʼ��
  LPLD_PIT_Init(pit_init_param);
  LPLD_PIT_EnableIrq(pit_init_param); //ʹ��PIT�ж�
  LPLD_FTM_Init(ftm_init_param);
  LPLD_LPTMR_Init(lptmr_init_param); 
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch0,5000,PTC1,ALIGN_LEFT);
 
  GetFreq=0;
  GetCnt=0;
  
  while(1)
  {   
  }
}


void pit_isr0()
{
    static uint32_t temp=0;
    
    if(GetCnt>99) //����100���ۼӣ��ۼӴ���Խ�ߣ�����Խ�ߣ���ʵʱ�Խ���
    {
       GetFreq=temp/100; //ȡƽ��
       printf("the PulseAcc:%d hz \r\n",GetFreq*100);
       temp=0;
       GetCnt=0;
    }
    else
    {
       temp+=LPLD_LPTMR_GetPulseAcc();
       GetCnt++;
    }
    
    LPLD_LPTMR_ResetCounter();       //��λLPTMR0 counter 
    LPLD_LPTMR_Init(lptmr_init_param);
}

