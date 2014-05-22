/**********************************************************************
	> File Name: dev_ov7725.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Wed 21 May 2014 03:50:45 PM CST
 *********************************************************************/

#include "common.h"
#include "dev_ov7725.h"

GPIO_InitTypeDef    OV_PTA_init;
GPIO_InitTypeDef    OV_PTB_init;
//GPIO_InitTypeDef    OV_PTE_init;
GPIO_InitTypeDef    OV_PTD_init;

DMA_InitTypeDef     OV_dma_init;

/*OV7725初始化配置表*/
reg_s ov7725_eagle_reg[] =
{
    //寄存器，寄存器值次
    {OV7725_COM4         , 0xC1},
    {OV7725_CLKRC        , 0x00},
    {OV7725_COM2         , 0x03},
    {OV7725_COM3         , 0xD0},
    {OV7725_COM7         , 0x40},
    {OV7725_HSTART       , 0x3F},
    {OV7725_HSIZE        , 0x50},
    {OV7725_VSTRT        , 0x03},
    {OV7725_VSIZE        , 0x78},
    {OV7725_HREF         , 0x00},
    {OV7725_SCAL0        , 0x0A},
    {OV7725_AWB_Ctrl0    , 0xE0},
    {OV7725_DSPAuto      , 0xff},
    {OV7725_DSP_Ctrl2    , 0x0C},
    {OV7725_DSP_Ctrl3    , 0x00},
    {OV7725_DSP_Ctrl4    , 0x00},

#if (H==80)
    {OV7725_HOutSize     , 0x14},
#elif (H==160)
    {OV7725_HOutSize     , 0x28},
#elif (H==240)
    {OV7725_HOutSize     , 0x3c},
#elif (H==320)
    {OV7725_HOutSize     , 0x50},
#else

#endif

#if (V==60)
    {OV7725_VOutSize     , 0x1E},
#elif (V==120)
    {OV7725_VOutSize     , 0x3c},
#elif (V==180)
    {OV7725_VOutSize     , 0x5a},
#elif (V==240)
    {OV7725_VOutSize     , 0x78},
#else

#endif

    {OV7725_EXHCH        , 0x00},
    {OV7725_GAM1         , 0x0c},
    {OV7725_GAM2         , 0x16},
    {OV7725_GAM3         , 0x2a},
    {OV7725_GAM4         , 0x4e},
    {OV7725_GAM5         , 0x61},
    {OV7725_GAM6         , 0x6f},
    {OV7725_GAM7         , 0x7b},
    {OV7725_GAM8         , 0x86},
    {OV7725_GAM9         , 0x8e},
    {OV7725_GAM10        , 0x97},
    {OV7725_GAM11        , 0xa4},
    {OV7725_GAM12        , 0xaf},
    {OV7725_GAM13        , 0xc5},
    {OV7725_GAM14        , 0xd7},
    {OV7725_GAM15        , 0xe8},
    {OV7725_SLOP         , 0x20},
    {OV7725_LC_RADI      , 0x00},
    {OV7725_LC_COEF      , 0x13},
    {OV7725_LC_XC        , 0x08},
    {OV7725_LC_COEFB     , 0x14},
    {OV7725_LC_COEFR     , 0x17},
    {OV7725_LC_CTR       , 0x05},
    {OV7725_BDBase       , 0x99},
    {OV7725_BDMStep      , 0x03},
    {OV7725_SDE          , 0x04},
    {OV7725_BRIGHT       , 0x00},
    {OV7725_CNST         , 0xFF},
    {OV7725_SIGN         , 0x06},
    {OV7725_UVADJ0       , 0x11},
    {OV7725_UVADJ1       , 0x02},

};

/*结构体数组成员数目*/
uint8 ov7725_eagle_cfgnum = ARR_SIZE (ov7725_eagle_reg);

//图像状态
volatile IMG_STATUS_e ov7725_eagle_img_flag = IMG_FINISH;

//ov7725初始化 
uint8 Ov7725_Init (void)
{
    uint16 i = 0;
    uint8 device_Id = 0;
    uint8 reg_Buff[50] = {0};

    LPLD_SCCB_Init ();
    
    Ov7725_Delay();

    while (!LPLD_SCCB_WriteReg (OV7725_COM7, 0x80))
    {
        if (i ++ >= 500)
            return 0;
    }

    Ov7725_Delay();
    
    while (0 == LPLD_SCCB_ReadReg (OV7725_VER, &device_Id, 1));

    printf ("You get ID success, SENSOR ID is 0x%x", device_Id);
    printf ("nconfig register Number is %d\n", ov7725_eagle_cfgnum);

    //判断摄像头是否为ov7725
    if (device_Id == OV7725_ID)
    {
        for (i = 0; i < ov7725_eagle_cfgnum; i++)
        {
            if (0 == LPLD_SCCB_WriteReg(ov7725_eagle_reg[i].addr, ov7725_eagle_reg[i].val))
            {
               printf ("error: writeReg error 0x%x\n", ov7725_eagle_reg[i].addr);
                return 0;
            }
            if (LPLD_SCCB_ReadReg(ov7725_eagle_reg[i].addr, &reg_Buff[i], 1))
            {
                printf ("writeRe 0x%x success: 0x%x\n", ov7725_eagle_reg[i].addr, reg_Buff[i]);
            }
        }
    }
    else
    {
        return 0;
    }
    return 1;

}

//ov7725延时函数
void Ov7725_Delay(void)
{
    uint16 i, n;
    for (i = 0; i < 3000; i++)
    {
        for (n = 0; n < 200; n++)
        {
            asm("nop");
        }
    }

}

//DMA中断触发函数
void Ov7725_eagle_dma (void)
{
    ov7725_eagle_img_flag = IMG_FINISH;
    DMA0->INT |= 0x1u << 0;
}

//场中断开始判断函数
void Ov7725_eagle_vsync(void)
{
    //判断场中断是否开始
    if (ov7725_eagle_img_flag == IMG_START)
    {
        ov7725_eagle_img_flag = IMG_GATHER;
        LPLD_GPIO_DisableIrq(OV_PTA_init);
        //enable_irq (DMA_CH0 + INT_DMA0_DMA16 - 16);
        LPLD_DMA_EnableIrq(OV_dma_init);
    }
    else
    {
        //disable_irq (INT_PORTA);
        LPLD_GPIO_DisableIrq(OV_PTA_init);
        ov7725_eagle_img_flag = IMG_FAIL;
    }
}

//ov7725开始采集图像
void Ov7725_eagle_get_img(void)
{
    ov7725_eagle_img_flag = IMG_START;
    //write 1 clear int 
    PORTA_ISFR = ~0;
    //清DMA中断标志位
    DMA0->INT |= 0x1u << 0;
    
    LPLD_GPIO_EnableIrq (OV_PTA_init);
    //enable_irq (INT_PORTA - 16);

    while (ov7725_eagle_img_flag != IMG_FINISH)
    {
        if (ov7725_eagle_img_flag == IMG_FAIL)
        {
            printf("eagle error\n");
            ov7725_eagle_img_flag = IMG_START;
            PORTA_ISFR = ~0;
           // enable_irq(INT_PORTA - 16);
            LPLD_GPIO_EnableIrq (OV_PTA_init);
        }
    }
}

/*!
 *  @brief      二值化图像解压（空间 换 时间 解压）
 *  @param      dst             图像解压目的地址
 *  @param      src             图像解压源地址
 *  @param      srclen          二值化图像的占用空间大小
 *  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
 *  Sample usage:   sendimg(imgbuff, CAMERA_W * CAMERA_H);                    //发送到上位机
 */
void Ov7725_img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {1, 0}; //0 和 1 分别对应的颜色
    //注：野火的摄像头 1(or255)表示 白色，0表示 黑色
    uint8 tmpsrc;
    uint16 i=0;
    while(srclen)
    {
        tmpsrc=*(src+i);
        dst[(PHOTO_SIZE/8-srclen)*8 + 0] = colour[ (tmpsrc >> 7 ) & 0x01 ];
        dst[(PHOTO_SIZE/8-srclen)*8 + 1] = colour[ (tmpsrc >> 6 ) & 0x01 ];
        dst[(PHOTO_SIZE/8-srclen)*8 + 2] = colour[ (tmpsrc >> 5 ) & 0x01 ];
        dst[(PHOTO_SIZE/8-srclen)*8 + 3] = colour[ (tmpsrc >> 4 ) & 0x01 ];
        dst[(PHOTO_SIZE/8-srclen)*8 + 4] = colour[ (tmpsrc >> 3 ) & 0x01 ];
        dst[(PHOTO_SIZE/8-srclen)*8 + 5] = colour[ (tmpsrc >> 2 ) & 0x01 ];
        dst[(PHOTO_SIZE/8-srclen)*8 + 6] = colour[ (tmpsrc >> 1 ) & 0x01 ];
        dst[(PHOTO_SIZE/8-srclen)*8 + 7] = colour[ (tmpsrc >> 0 ) & 0x01 ];
        ++i;
        --srclen;
    }
}

/*!
 *  @brief      发送图像到eSmartCameraCar上位机显示
 *  @param      imgaddr         图像地址
 *  @param      imgsize         图像占用空间大小
 *  @since      v5.0
 *  @note       不同的上位机，不同的命令，这里使用 eSmartCameraCar软件，
                如果使用其他上位机，则需要修改代码。
 *  Sample usage:   sendimg(imgbuff, CAMERA_W * CAMERA_H);                    //发送到上位机
 */
void Ov7725_sendimg(uint8 *imgaddr, uint32 imgsize)
{
    uint8 cmd[4] = {0, 255, 1, 0 };    //yy_摄像头串口调试 使用的命令
    
    LPLD_UART_PutCharArr(TERM_PORT, (char *)cmd, sizeof(cmd));
    LPLD_UART_PutCharArr(TERM_PORT, (char *)imgaddr, imgsize);

    //uart_putbuff(FIRE_PORT, cmd, sizeof(cmd));    //先发送命令

    //uart_putbuff(FIRE_PORT, imgaddr, imgsize); //再发送图像
}

//OV7725GPIO口初始化
void Ov7725_Gpio_Init(void)
{
    //OV数据口初始化 B0～B7
    OV_PTB_init.GPIO_PTx = PTB;
    OV_PTB_init.GPIO_Dir = DIR_INPUT;
    OV_PTB_init.GPIO_Pins = GPIO_Pin0_7;
    //禁止中断和DMA请求 禁用PULL 输入源下拉
    OV_PTB_init.GPIO_PinControl = IRQC_DIS |
                                  INPUT_PULL_DIS|
                                  INPUT_PULL_DOWN;
    LPLD_GPIO_Init (OV_PTB_init);

    //OV场信号接口初始化 A29 
    OV_PTA_init.GPIO_PTx = PTA;
    OV_PTA_init.GPIO_Dir = DIR_INPUT;
    OV_PTA_init.GPIO_Pins = GPIO_Pin29;
    OV_PTA_init.GPIO_PinControl = IRQC_RI |
                                    INPUT_PULL_DOWN;
    OV_PTA_init.GPIO_Isr = Ov7725_isr;
    LPLD_GPIO_Init (OV_PTA_init);

    //OV PCLK信号接口初始化: PTA27-PCLK 
    OV_PTD_init.GPIO_PTx = PTA;
    OV_PTD_init.GPIO_Pins = GPIO_Pin27;
    OV_PTD_init.GPIO_Dir = DIR_INPUT;
    OV_PTD_init.GPIO_PinControl = IRQC_DMARI | INPUT_PULL_DIS;
    LPLD_GPIO_Init (OV_PTD_init);

}
 
//OV7725 DMA初始化 
void Ov7725_Dma_Init (void)
{
    OV_dma_init.DMA_CHx = DMA_CH0;
    OV_dma_init.DMA_Req = PORTA_DMAREQ;
    OV_dma_init.DMA_MajorLoopCnt = PHOTO_SIZE / 8;
    OV_dma_init.DMA_MinorByteCnt = 1;
    //
    OV_dma_init.DMA_SourceAddr = (uint32)(&GPIOB_PDIR);
    OV_dma_init.DMA_DestAddr = (uint32)img;
    OV_dma_init.DMA_DestAddrOffset = 1;
    OV_dma_init.DMA_Isr = Ov7725_eagle_dma;
    LPLD_DMA_Init (OV_dma_init);

    LPLD_DMA_DisableReq(DMA_CH0);
    //disable_irq (INT_PORTA - 16);
    LPLD_GPIO_EnableIrq (OV_PTA_init);
    DMA0->INT |= 0x1u <<0;
    //允许DMA通道传输完成中断
    //enable_irq (DMA_CH0);
    LPLD_DMA_EnableIrq (OV_dma_init);
}

//ov7725中断
void Ov7725_isr (void)
{
    uint8 n = 0;
    while (!PORTA_ISFR);
    uint32 flag = (PORTA->ISFR);
    (PORTA -> ISFR) = ~0;

    n = 29;
    if (flag & (1<<n))
    {
        Ov7725_eagle_vsync();
    }
}

