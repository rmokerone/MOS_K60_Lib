/**********************************************************************
	> File Name: main.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Thu 22 May 2014 12:19:26 PM CST
 *********************************************************************/

#include "common.h"
#include "dev_ov7725.h"

extern DMA_ISR_CALLBACK DMA_ISR[16];
extern GPIO_InitTypeDef OV_PTA_init;

extern char ready_send_flag;

int main (void)
{

    Ov7725_Init ();
    Ov7725_Delay();

    //Ov7725_Dma_Init();
    Ov7725_Gpio_Init ();
    dma_portx2buff_init (DMA_CH0, (void *)(&GPIOB_PDIR), (void *)img,PORTA_DMAREQ ,DMA_BYTE1, PHOTO_SIZE / 8, DADDR_KEEPON);
    //装载DMA中断
    DMA_ISR[DMA_CH0] = Ov7725_eagle_dma;
    //关闭DMA通道
    DMA_DIS(DMA_CH0);
    //关GPIO中断
    disable_irq(INT_PORTA - 16);
    //清DMA中断置为标志
    DMA_IRQ_CLEAN(DMA_CH0);
    //使能DMA中断
    DMA_IRQ_EN (DMA_CH0);
    //开启GPIO中断
    LPLD_GPIO_EnableIrq(OV_PTA_init);
    while (1)
    {
        //获取图像
        //Ov7725_eagle_get_img ();
        //解压图像
        //Ov7725_img_extract (Pix_Data, img, PHOTO_SIZE/8);
        if(ready_send_flag)
        {
            //发送图片到上位机
            //get_midline(Pix_Data, 60, 80);
            Ov7725_sendimg (Pix_Data, PHOTO_SIZE);
            ready_send_flag = 0;
        }
    }
    return 0;
}
