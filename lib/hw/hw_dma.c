/**
 * @file HW_DMA.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief DMA�ײ�ģ����غ���
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
#include "hw_dma.h"

//�û��Զ����жϷ���������
DMA_ISR_CALLBACK DMA_ISR[16];


/*
 * LPLD_DMA_Init
 * ��ʼ��eDMAģ��
 * 
 * ����:
 *    dma_init_struct--eDMA��ʼ���ṹ�壬
 *                        ���嶨���DMA_InitTypeDef
 *
 * ���:
 *    0--���ô���
 *    1--���óɹ�
 */
uint8 LPLD_DMA_Init(DMA_InitTypeDef dma_init_struct)
{
  uint8 chx = dma_init_struct.DMA_CHx;
  uint8 req = dma_init_struct.DMA_Req;
  boolean periodic_trigg = dma_init_struct.DMA_PeriodicTriggerEnable;
  uint16 major_cnt = dma_init_struct.DMA_MajorLoopCnt;
  uint32 minor_cnt = dma_init_struct.DMA_MinorByteCnt;
  uint32 src_addr = dma_init_struct.DMA_SourceAddr;
  uint8 src_dsize = dma_init_struct.DMA_SourceDataSize;
  int16 src_addroffset = dma_init_struct.DMA_SourceAddrOffset;
  int32 src_lastadj = dma_init_struct.DMA_LastSourceAddrAdj;
  uint32 dst_addr = dma_init_struct.DMA_DestAddr;
  uint8 dst_dsize = dma_init_struct.DMA_DestDataSize;
  int16 dst_addroffset = dma_init_struct.DMA_DestAddrOffset;
  int32 dst_lastadj = dma_init_struct.DMA_LastDestAddrAdj;
  boolean auto_disable = dma_init_struct.DMA_AutoDisableReq;
  
  //�������
  //ASSERT( chx <= DMA_CH15 );       //eDMAͨ��ѡ��
  //ASSERT( req <= DMA_MUX_63 );     //����Դѡ��
  //ASSERT( major_cnt <= 0x7FFF );   //�������ж�
  //ASSERT( src_addr != NULL );      //Դ��ַ�ж�
  //ASSERT( (src_dsize <= DMA_SRC_32BIT)||(src_dsize == DMA_SRC_16BYTE) );     //Դ���ݴ����С�ж�
  //ASSERT( dst_addr != NULL );      //Ŀ�ĵ�ַ�ж�
  //ASSERT( (dst_dsize <= DMA_DST_32BIT)||(dst_dsize == DMA_DST_16BYTE) );     //Ŀ�����ݴ����С�ж�
 
  if (chx > DMA_CH15)
    return 0;
  if (req > DMA_MUX_63)
    return 0;
  if (major_cnt > 0x7FFF)
    return 0;
  if (src_addr == 0)
    return 0;
  if ((src_dsize > DMA_SRC_32BIT) || (src_dsize != DMA_SRC_16BYTE))
    return 0;
  if (dst_addr == 0)
    return 0;
  if ((dst_dsize > DMA_DST_32BIT) || (dst_dsize != DMA_SRC_16BYTE))
    return 0;

  SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;     //��DMAģ��ʱ��
  SIM_SCGC6 |= SIM_SCGC6_DMAMUX0_MASK;  //��DMAͨ����·������ʱ��   
  
  //�ر�ͨ��xӲ��DMA���� 
  DMA0->ERQ &= ~(1<<chx);
  
  //ѡ�� ͨ��x ���������DMAԴ������
  DMAMUX->CHCFG[chx] = DMAMUX_CHCFG_SOURCE(req);
  //�Ƿ�ʹ�����ڴ�������
  if(periodic_trigg == TRUE)
  {
    DMAMUX->CHCFG[chx] |= DMAMUX_CHCFG_TRIG_MASK;
  }
  else
  {
    DMAMUX->CHCFG[chx] &= ~(DMAMUX_CHCFG_TRIG_MASK);
  }
  
  
  //����Դ��ַ   
  DMA0->TCD[chx].SADDR = DMA_SADDR_SADDR(src_addr);
  //��ִ�������Դ��ַ�Ĳ���֮����Դ��ַ�Ļ���������/����ƫ�Ƶ�ַ
  DMA0->TCD[chx].SOFF = DMA_SOFF_SOFF(src_addroffset);
  //����Դ��ַ�Ĵ����С
  DMA0->TCD[chx].ATTR = 0 | DMA_ATTR_SSIZE(src_dsize);
  //���ļ���������major iteration count���ﵽ�����µ���Դ��ַ
  DMA0->TCD[chx].SLAST = DMA_SLAST_SLAST(src_lastadj);
  
  //����Ŀ�ĵ�ַ 
  DMA0->TCD[chx].DADDR = DMA_DADDR_DADDR(dst_addr);
  //��ִ�������Ŀ�ĵ�ַ�Ĳ���֮����Ŀ�ĵ�ַ�Ļ���������/����ƫ�Ƶ�ַ
  DMA0->TCD[chx].DOFF = DMA_DOFF_DOFF(dst_addroffset);
  //����Ŀ�ĵ�ַ�Ĵ�����
  DMA0->TCD[chx].ATTR |= DMA_ATTR_DSIZE(dst_dsize);
  //���ļ���������major iteration count���ﵽ�����µ���Ŀ�ĵ�ַ
  DMA0->TCD[chx].DLAST_SGA = DMA_DLAST_SGA_DLASTSGA(dst_lastadj);
  
  //Ĭ��Ϊ����ͨ�����ӹ��ܣ�����������Ӵ˹���
  if( 1 == 1)
  {
    //===============���������������ȣ�ѭ������====================================
    //������ѭ�������� current major loop count
    DMA0->TCD[chx].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(major_cnt);
    //��ʼѭ��������������ѭ��������Ϊ���ʱ�򣬽�װ����ʼѭ����������ֵ
    DMA0->TCD[chx].BITER_ELINKNO = DMA_CITER_ELINKNO_CITER(major_cnt);
  }
  
  //Ĭ��Ϊ���ô�ѭ����ַƫ�ƹ��ܣ�����������Ӵ˹���
  if( 1 == 1)
  {
    //��ѭ��һ�δ����ֽڵĸ���
    DMA0->TCD[chx].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(minor_cnt);
  }
  
  //���TCD���ƼĴ���     
  DMA0->TCD[chx].CSR = 0;
  
  //����eDMA�ж�
  if((dma_init_struct.DMA_Isr != NULL) && 
     (dma_init_struct.DMA_MajorCompleteIntEnable == TRUE))
  {

       DMA0->TCD[chx].CSR |= DMA_CSR_INTMAJOR_MASK; //ʹ��DMA ��ѭ�������������� �ж�
       DMA_ISR[chx] = dma_init_struct.DMA_Isr;
  }
  if((dma_init_struct.DMA_Isr != NULL) && 
     (dma_init_struct.DMA_MajorHalfCompleteIntEnable == TRUE))
  {

       DMA0->TCD[chx].CSR |= DMA_CSR_INTHALF_MASK; //ʹ��DMA ��ѭ������������һ�� �ж�
       DMA_ISR[chx] = dma_init_struct.DMA_Isr;
  }
    
  if(auto_disable == TRUE)
  {
     DMA0->TCD[chx].CSR |= DMA_CSR_DREQ_MASK; //��ѭ����������������Զ��ر�DMA 
  }
  else
  {
     DMA0->TCD[chx].CSR &= ~(DMA_CSR_DREQ_MASK); //��ѭ��������������󣬲��ر�DMA
  } 
 
  //DMAͨ��ʹ��
  DMAMUX->CHCFG[chx] |= DMAMUX_CHCFG_ENBL_MASK;
    
  return 1;
}

/*
 * LPLD_DMA_EnableIrq
 * ʹ��eDMA�ж�
 * 
 * ����:
 *    dma_init_struct--eDMA��ʼ���ṹ�壬
 *                        ���嶨���DMA_InitTypeDef
 *
 * ���:
 *    0--ʧ��
 *    1--�ɹ�
 */
uint8 LPLD_DMA_EnableIrq(DMA_InitTypeDef dma_init_struct)
{
  enable_irq(dma_init_struct.DMA_CHx + INT_DMA0_DMA16 - 16);  
  return 1;
}

/*
 * LPLD_DMA_DisableIrq
 * ����eDMA�ж�
 * 
 * ����:
 *    dma_init_struct--eDMA��ʼ���ṹ�壬
 *                        ���嶨���DMA_InitTypeDef
 *
 * ���:
 *    0--ʧ��
 *    1--�ɹ�
 */
uint8 LPLD_DMA_DisableIrq(DMA_InitTypeDef dma_init_struct)
{
  disable_irq(dma_init_struct.DMA_CHx + INT_DMA0_DMA16 - 16);
  return 1;
}

/*
 * LPLD_DMA_SoftwareStartService
 * DMA�������������ʼ
 * 
 * ����:
 *    dma_init_struct--eDMA��ʼ���ṹ�壬
 *                        ���嶨���DMA_InitTypeDef
 *
 * ���:
 *    ��
 */
void LPLD_DMA_SoftwareStartService(DMA_InitTypeDef dma_init_struct)
{
  DMA0->TCD[dma_init_struct.DMA_CHx].CSR |= DMA_CSR_START_MASK; 
}

/*
 * eDMA�жϴ�����
 * �������ļ�startup_K60.s�е��ж����������
 * �û������޸ģ������Զ������Ӧͨ���жϺ���
 */
void DMA0_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[0]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<0;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA1_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[1]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<1;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA2_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[2]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<2;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA3_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[3]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<3;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA4_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[4]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<4;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA5_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[5]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<5;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA6_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[6]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<6;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA7_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[7]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<7;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA8_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[8]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<8;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA9_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[9]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<9;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA10_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[10]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<10;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA11_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[11]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<11;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA12_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[12]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<12;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA13_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[13]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<13;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA14_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[14]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<14;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}

void DMA15_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif  
  
  //�����û��Զ����жϷ���
  DMA_ISR[15]();
  //����жϱ�־λ
  DMA0->INT |= 0x1u<<15;
  
#if (UCOS_II > 0u)
  OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
#endif
}
















