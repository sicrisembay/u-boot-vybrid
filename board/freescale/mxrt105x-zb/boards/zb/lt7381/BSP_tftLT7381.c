/*
 * BSP_tftLT7381.c
 *
 *  Created on: 11 Feb 2019
 *      Author: Toan
 */

//*****************************************************************************
// File dependencies.
//*****************************************************************************
/* BSP */
#include "BSP_tftLT7381.h"
#include "fsl_clock.h"
#include "fsl_lpspi.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "linux/delay.h"
#include "LT738_Lib.h"
#include "BSP_tca9535.h"

#define DIG_OUT_LCD_RESET_MUX               IOMUXC_GPIO_SD_B1_07_GPIO3_IO07
#define DIG_OUT_LCD_RESET_GPIO_BASE         GPIO3
#define DIG_OUT_LCD_RESET_IO_PIN            7U

#define LT7381_LPSPI_BASEADDR               (LPSPI1)
#define TRANSFER_BAUDRATE                   (10000000U) // 10MHz
#define LPSPI_PODF                          (4U) /* Clock divider /5 for master lpspi clock source */
#define LPSPI_MASTER_CLOCK_FREQ             (CLOCK_GetFreq(kCLOCK_SysPllClk) / (LPSPI_PODF + 1U))

static uint8_t g_masterFifoSize;

#if 0
#include "BSP_lpspiManager.h"
#include "BSP_digOut.h"

/* LCD config */
#include <cr_section_macros.h>
#endif
/* LT738 Library */
#include "LT738.h"
#include "LT738_Lib.h"
#if 0
//*****************************************************************************
// Private definitions.
//*****************************************************************************
#define BSP_LPSPI_TFT_ID    BSP_LPSPI_ID_0
#endif
#define layer1_start_addr 0
#if 0
#define SS_RESET                BSP_DOUT_SetState(DIG_OUT_LPSPI1_CS, 0U);
#define SS_SET                  BSP_DOUT_SetState(DIG_OUT_LPSPI1_CS, 1U);

//*****************************************************************************
// Private member declarations.
//*****************************************************************************
/* Buffer to send data to the LCD controller */
AT_NONCACHEABLE_SECTION_INIT(uint8_t lt7381_tx_spi_buf[2]) = {0U};
AT_NONCACHEABLE_SECTION_INIT(uint8_t lt7381_rx_spi_buf[2]) = {0U};

/* Semaphore for LPSPI transaction signal */
static SemaphoreHandle_t semHdlDone = NULL;
static StaticSemaphore_t semBuffDone;

/* Worker task to update dirty rect */
static StaticTask_t xTaskWorker;
static StackType_t xTaskStackWorker[configMINIMAL_STACK_SIZE];
static TaskHandle_t xTaskHandleWorker = NULL;

/* Initialization flag */
static bool bInit = false;

//*****************************************************************************
// Public / Internal member external declarations.
//*****************************************************************************

//*****************************************************************************
// Private function prototypes.
//*****************************************************************************
static void _Worker_Task(void *pxParam);
#endif
static void _TFT_LT7381_Init(void);
static u8 SPI_DataRead(void);
static void SPI_DataWrite(u8 data);
#if 0
static void SPI_WriteByte(u8 data);
#endif
static uint8_t SPI_StatusRead(void);
static void SPI_CmdWrite(u8 cmd);
static void LT768_IO_Spi(void);
#if 0
static void SPI_DataWrite_Pixel(u16 data);
static void SPI_DataWrite_Line(unsigned char *data, uint32_t h);
#endif
static void LCD_Window_Init(void);

//*****************************************************************************
// Public function implementations
//*****************************************************************************
static void _LT7381_InitSPI(void)
{
	lpspi_master_config_t masterConfig;

    CLOCK_EnableClock(kCLOCK_Iomuxc);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, LPSPI_PODF);
    CLOCK_SetMux(kCLOCK_LpspiMux, 2U);	/* Select PLL2 main clock as lpspi clock source */

    /* Init SPI Pins */
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_LPSPI1_SCK, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_LPSPI1_PCS0, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_LPSPI1_SDO, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_LPSPI1_SDI, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_LPSPI1_SCK, 0x10B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_LPSPI1_PCS0, 0x10B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_LPSPI1_SDO, 0x10B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_LPSPI1_SDI, 0x10B0u);

    /*Master config*/
	masterConfig.baudRate = TRANSFER_BAUDRATE;
	masterConfig.bitsPerFrame = 8;
	masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
	masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
	masterConfig.direction = kLPSPI_MsbFirst;
	masterConfig.pcsToSckDelayInNanoSec = 1000000000 / masterConfig.baudRate;
	masterConfig.lastSckToPcsDelayInNanoSec = 1000000000 / masterConfig.baudRate;
	masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;
	masterConfig.whichPcs = kLPSPI_Pcs0;
	masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;
	masterConfig.pinCfg = kLPSPI_SdiInSdoOut;
	masterConfig.dataOutConfig = kLpspiDataOutRetained;
	LPSPI_MasterInit(LT7381_LPSPI_BASEADDR, &masterConfig, LPSPI_MASTER_CLOCK_FREQ);

	g_masterFifoSize = LPSPI_GetRxFifoSize(LT7381_LPSPI_BASEADDR);

	LPSPI_Enable(LT7381_LPSPI_BASEADDR, false);
	LT7381_LPSPI_BASEADDR->CFGR1 &= (~LPSPI_CFGR1_NOSTALL_MASK);
	LPSPI_Enable(LT7381_LPSPI_BASEADDR, true);

	/*Flush FIFO , clear status , disable all the inerrupts.*/
	LPSPI_FlushFifo(LT7381_LPSPI_BASEADDR, true, true);
	LPSPI_ClearStatusFlags(LT7381_LPSPI_BASEADDR, kLPSPI_AllStatusFlag);
	LPSPI_DisableInterrupts(LT7381_LPSPI_BASEADDR, kLPSPI_AllInterruptEnable);

	LT7381_LPSPI_BASEADDR->TCR = (LT7381_LPSPI_BASEADDR->TCR &
	         ~(LPSPI_TCR_CONT_MASK | LPSPI_TCR_CONTC_MASK | LPSPI_TCR_RXMSK_MASK | LPSPI_TCR_PCS_MASK)) |
	        LPSPI_TCR_CONT(0) | LPSPI_TCR_CONTC(0) | LPSPI_TCR_RXMSK(0) | LPSPI_TCR_TXMSK(0) | LPSPI_TCR_PCS(kLPSPI_Pcs0);

	while (LPSPI_GetTxFifoCount(LT7381_LPSPI_BASEADDR) != 0) {
	}
}

//*****************************************************************************
//!
//! \brief This function initializes the LT7381
//!
//! \param  None
//!
//! \return \c ::BSP_TFTLT7381_RET_T
//!
//*****************************************************************************
BSP_TFTLT7381_RET_T BSP_TFT_LT7381_Init(void)
{
	gpio_pin_config_t pin_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    BSP_TFTLT7381_RET_T retval = BSP_TFTLT7381_NOERROR;

    /* Configure Power Pin */
    CLOCK_EnableClock(kCLOCK_Iomuxc);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_11_GPIO2_IO11, 0U);
	GPIO_PinInit(GPIO2, 11, &pin_config);
	GPIO_WritePinOutput(GPIO2, 11U, 1);

    /* Configure LCD_RESET */
	IOMUXC_SetPinMux(DIG_OUT_LCD_RESET_MUX, 0U);
	GPIO_PinInit(DIG_OUT_LCD_RESET_GPIO_BASE, DIG_OUT_LCD_RESET_IO_PIN, &pin_config);
	/* Configure LCD Backlight pin */
	if(BSP_TCA9535_NOERROR != BSP_TCA9535_Init()) {
		printf("BSP_TFT_LT7381_Init: Error in BSP_TCA9535_Init()\n");
	}

	_LT7381_InitSPI();

    _TFT_LT7381_Init();

    return retval;
}

void BSP_TFT_LT7381_Reset(uint8_t sts)
{
	GPIO_WritePinOutput(DIG_OUT_LCD_RESET_GPIO_BASE, DIG_OUT_LCD_RESET_IO_PIN, sts);
}

void BSP_TFT_LT7381_BL(uint8_t sts)
{
	if(BSP_TCA9535_NOERROR != BSP_TCA9535_SetPinState(TCA9535_P10, sts)) {
		printf("BSP_TFT_LT7381_BL: Error in BSP_TCA9535_SetPinState()\n");
	}
}
#if 0
//*****************************************************************************
//!
//! \brief This function check if the LT7381 is ready or not
//!
//! \param  None
//!
//! \return \c true if ready
//!             otherwise, false
//!
//*****************************************************************************
extern BSP_TFTLT7381_RET_T BSP_TFT_LT7381_IsReady(void)
{
    return bInit;
}
#endif
//*****************************************************************************
//!
//! \brief This function write Register's address to the LT7381
//!
//! \param  cmd     Register's address
//!
//! \return \c ::BSP_TFTLT7381_RET_T
//!
//*****************************************************************************
void LCD_CmdWrite(uint8_t cmd)
{
    SPI_CmdWrite(cmd);
}

//*****************************************************************************
//!
//! \brief This function writes data to Register or memory
//!
//! \param  data     Data to write to Register or Memory
//!
//! \return \c      None
//!
//*****************************************************************************
void LCD_DataWrite(uint8_t data)
{
    SPI_DataWrite(data);
}
#if 0
//*****************************************************************************
//!
//! \brief This function writes pixels to the LCD RAM
//!
//! \param  data     Data to write to the LCD RAM
//!
//! \return \c      None
//!
//*****************************************************************************
void LCD_DataWrite_Pixel(uint16_t data)
{
    SPI_DataWrite_Pixel(data);
}

//*****************************************************************************
//!
//! \brief This function write each line of the window to LCD RAM
//!
//! \param  data     Pointer to the buffer
//!         h        length of the buffer
//!
//! \return \c      None
//!
//*****************************************************************************
void LCD_DataWrite_Line(unsigned char *data, uint32_t w)
{
    SPI_DataWrite_Line(data, w);
}
#endif
//*****************************************************************************
//!
//! \brief This function read status register
//!
//! \param  None
//!
//! \return \c      Status Register
//!
//*****************************************************************************
uint8_t LCD_StatusRead(void)
{
    return SPI_StatusRead();
}

//*****************************************************************************
//!
//! \brief This function read Register's Data
//!
//! \param  None
//!
//! \return \c      Register's Data
//!
//*****************************************************************************
u8 LCD_DataRead(void)
{
    u8 temp = 0;
    temp = SPI_DataRead();
    return temp;
}

//*****************************************************************************
//!
//! \brief Delay function in ms used by LT738 Library
//!
//! \param  time    Delayed time
//!
//! \return \c      None
//!
//*****************************************************************************
void Delay_ms(uint16_t time)
{
    unsigned long dly_us = time * 1000U;
    udelay(dly_us);
}

//*****************************************************************************
//!
//! \brief Delay function in us used by LT738 Library
//!
//! \param  time    Delayed time
//!
//! \return \c      None
//!
//*****************************************************************************
void Delay_us(uint16_t time)
{
    udelay((unsigned long)time);
}

void LCD_Init(void)
{
    SPI_RST(1);
    Delay_us(10);
    SPI_RST(0);
    Delay_us(800);
    SPI_RST(1);
    Delay_us(800);

	//************* Start Initial Sequence **********//
	SPI_WriteComm(0x0001);
	SPI_WriteData (0x6300);

	SPI_WriteComm(0x0002);
	SPI_WriteData (0x0200);

	SPI_WriteComm(0x0003);
	SPI_WriteData (0x7166);

	SPI_WriteComm(0x0004);
	SPI_WriteData (0x0447);

	SPI_WriteComm(0x0005);
	SPI_WriteData (0xbcd4);
	//
	SPI_WriteComm(0x000A);
	SPI_WriteData (0x3f08);

	SPI_WriteComm(0x000b);
	SPI_WriteData (0xd400);

	SPI_WriteComm(0x000d);
	SPI_WriteData (0x123a);

	SPI_WriteComm(0x000e);
	SPI_WriteData (0x2a00);

	SPI_WriteComm(0x000f);       // Display Function Control
	SPI_WriteData (0x0000);

	SPI_WriteComm(0x0016);       //Power control
	SPI_WriteData (0x9f80);     //VRH[5:0]

	SPI_WriteComm(0x0017);       //Power control
	SPI_WriteData (0x1406);     //SAP[2:0];BT[3:0]

	SPI_WriteComm(0x001e);       //VCM control
	SPI_WriteData (0x00d4);
}

void SPI_RST(char val)
{
    unsigned char temp;
    LCD_CmdWrite(0xF1);
    temp=LCD_DataRead();
    if(val==1)  temp|=(1<<1);
    else        temp&=~(1<<1);
    LCD_DataWrite(temp);
}

void SPI_CS(char val)
{
    unsigned char temp;
    LCD_CmdWrite(0xF1);
    temp=LCD_DataRead();
    if(val==1)  temp|=(1<<2);
    else        temp&=~(1<<2);
    LCD_DataWrite(temp);
}

void SPI_CLK(char val)
{
    unsigned char temp;
    LCD_CmdWrite(0xF1);
    temp=LCD_DataRead();
    if(val==1)  temp|=(1<<3);
    else        temp&=~(1<<3);
    LCD_DataWrite(temp);
}

void SPI_SDA(char val)
{
    unsigned char temp;
    LCD_CmdWrite(0xF1);
    temp=LCD_DataRead();
    if(val==1)  temp|=(1<<4);
    else        temp&=~(1<<4);
    LCD_DataWrite(temp);
}

void SPI_WriteComm(unsigned int i)    //spec page 46
{
   unsigned char n;
   SPI_CS(0);

   SPI_CLK(0);
   SPI_SDA(0);   //0
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(0);
   SPI_SDA(1);  //1
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(0);
   SPI_SDA(1); //1
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(0);
   SPI_SDA(1); //1
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);

   SPI_CLK(0);
   SPI_SDA(0); //0
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(0);
   SPI_SDA(0);//0
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(0);
   SPI_SDA(0); //0
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(0);
   SPI_SDA(0); //0
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);

  for(n=0; n<16; n++)
   {
      SPI_CLK(0);
                if(i&0x8000) SPI_SDA(1);
        else       SPI_SDA(0);


//    SPI_DI=i&0x8000;
      Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
      SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
      i<<= 1;
   }
    SPI_CS(1);
}

void SPI_WriteData(unsigned int i)
{
   unsigned char n;
   SPI_CS(0);

     SPI_CLK(0);
   SPI_SDA(0);   //0
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(0);
   SPI_SDA(1);  //1
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(0);
   SPI_SDA(1); //1
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(0);
   SPI_SDA(1); //1
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);

   SPI_CLK(0);
   SPI_SDA(0); //0
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(0);
   SPI_SDA(0);//0
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(0);
   SPI_SDA(1); //1
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(0);
   SPI_SDA(0); //0
   Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
   SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);

  for(n=0; n<16; n++)
   {
      SPI_CLK(0);
        if(i&0x8000) SPI_SDA(1);
        else       SPI_SDA(0);

//    SPI_DI=i&0x8000;
      Delay_us(1); Delay_us(1);Delay_us(1);Delay_us(1);
      SPI_CLK(1);Delay_us(1);Delay_us(1);Delay_us(1);Delay_us(1);
      i<<=1;
   }
    SPI_CS(1);
}

#if 0
//*****************************************************************************
// Private function implementations.
//*****************************************************************************
static void _Worker_Task(void *pxParam)
{
    /* Wait until LPSPI manager is ready */
    while(!BSP_LPSPI_MANAGER_isReady())
    {
        vTaskDelay(100);
    }

    /* Initialize LT7381 */
    _TFT_LT7381_Init();

    /* Set the flag after initializing the LT7381 */
    bInit = true;
    while(1)
    {
        vTaskDelay(100);
        vTaskSuspend(NULL); /* Suspend this task because it's just for initialization */
    }
}
#endif
static void _TFT_LT7381_Init(void)
{
	uint32_t thickness;
    LT738_Init();
    LT768_IO_Spi();
    LCD_Init();

    LT738_PWM1_Init(1,0,200,100,100);
    LCD_Window_Init();

    /* Fill the background to white before turning ON the LCD */
    LT738_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Black);

    /* Turn ON the BL */
    BSP_TFT_LT7381_BL(1);

    Display_ON();

    for(thickness = 1; thickness <= LCD_YSIZE_TFT/12; thickness++) {
    	Delay_ms(25);
        LT738_DrawCircle_Width(LCD_XSIZE_TFT/2, LCD_YSIZE_TFT/2, LCD_YSIZE_TFT/6, Red, Black, thickness);
    }
}

static void SPI_DataWrite(u8 data)
{
	uint8_t tx_spi_buf[2];
	uint8_t rx_spi_buf[2];
	lpspi_transfer_t transfer;

	tx_spi_buf[1] = data;
	tx_spi_buf[0] = 0x80;

	transfer.txData = tx_spi_buf;
	transfer.rxData = rx_spi_buf;
	transfer.dataSize = 2;
	transfer.configFlags = kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous;

	LPSPI_MasterTransferBlocking(LT7381_LPSPI_BASEADDR, &transfer);
}

#if 0
static void SPI_WriteByte(u8 data)
{
    lt7381_tx_spi_buf[0] = data;
    BSP_DOUT_SetState(DIG_OUT_LPSPI1_CS, 0U);
    BSP_LPSPI_MANAGER_WriteRequest(BSP_LPSPI_TFT_ID, lt7381_tx_spi_buf, 1, semHdlDone);

    /* Wait for sending data to SPI */
    xSemaphoreTake(semHdlDone, portMAX_DELAY);
    BSP_DOUT_SetState(DIG_OUT_LPSPI1_CS, 1U);
}
#endif
static u8 SPI_DataRead(void)
{
	uint8_t tx_spi_buf[2];
	uint8_t rx_spi_buf[2];
	lpspi_transfer_t transfer;

	tx_spi_buf[1] = 0xFF;
	tx_spi_buf[0] = 0xC0;

	transfer.txData = tx_spi_buf;
	transfer.rxData = rx_spi_buf;
	transfer.dataSize = 2;
	transfer.configFlags = kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous;

	LPSPI_MasterTransferBlocking(LT7381_LPSPI_BASEADDR, &transfer);

    return rx_spi_buf[1];
}

static uint8_t SPI_StatusRead(void)
{
	uint8_t tx_spi_buf[2];
	uint8_t rx_spi_buf[2];
	lpspi_transfer_t transfer;

	tx_spi_buf[1] = 0xFF;
	tx_spi_buf[0] = 0x40;

	transfer.txData = tx_spi_buf;
	transfer.rxData = rx_spi_buf;
	transfer.dataSize = 2;
	transfer.configFlags = kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous;

	LPSPI_MasterTransferBlocking(LT7381_LPSPI_BASEADDR, &transfer);

	return rx_spi_buf[1];
}

static void SPI_CmdWrite(u8 cmd)
{
	uint8_t tx_spi_buf[2];
	uint8_t rx_spi_buf[2];
	lpspi_transfer_t transfer;

	tx_spi_buf[1] = cmd;
	tx_spi_buf[0] = 0x00;

	transfer.txData = tx_spi_buf;
	transfer.rxData = rx_spi_buf;
	transfer.dataSize = 2;
	transfer.configFlags = kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous;

	LPSPI_MasterTransferBlocking(LT7381_LPSPI_BASEADDR, &transfer);
}

static void LT768_IO_Spi(void)
{
    Set_GPIO_A_In_Out(0x00);
    Write_GPIO_A_7_0(0xff);
}

#if 0
static void SPI_DataWrite_Pixel(u16 data)
{
    SPI_DataWrite(data);
    SPI_DataWrite(data>>8);
}

static void SPI_DataWrite_Line(unsigned char *data, uint32_t w)
{
    uint32_t remainingLength = w;

    for (uint32_t i = 0; i < w / LCD_TFT_BUFFER_SIZE; i ++)
    {
            /* Check the status of the FIFO before sending */
            Check_Mem_WR_FIFO_not_Full();
            SS_RESET
            lt7381_tx_spi_buf[0] = 0x80;
            BSP_LPSPI_MANAGER_WriteRequest(BSP_LPSPI_TFT_ID, lt7381_tx_spi_buf, 1, semHdlDone);
            xSemaphoreTake(semHdlDone, portMAX_DELAY);
            BSP_LPSPI_MANAGER_WriteRequest(BSP_LPSPI_TFT_ID, data, LCD_TFT_BUFFER_SIZE, semHdlDone);
            xSemaphoreTake(semHdlDone, portMAX_DELAY);
            SS_SET
            remainingLength -= LCD_TFT_BUFFER_SIZE;
            data += LCD_TFT_BUFFER_SIZE;
    }

    /* Sending the last block */
    if (remainingLength > 0)
    {
        /* Check the status of the FIFO before sending */
        Check_Mem_WR_FIFO_not_Full();
        SS_RESET
        lt7381_tx_spi_buf[0] = 0x80;
        BSP_LPSPI_MANAGER_WriteRequest(BSP_LPSPI_TFT_ID, lt7381_tx_spi_buf, 1, semHdlDone);
        xSemaphoreTake(semHdlDone, portMAX_DELAY);
        BSP_LPSPI_MANAGER_WriteRequest(BSP_LPSPI_TFT_ID, data, remainingLength, semHdlDone);
        xSemaphoreTake(semHdlDone, portMAX_DELAY);
        SS_SET
    }
}
#endif

static void LCD_Window_Init(void)
{
    Select_Main_Window_16bpp();
    Main_Image_Start_Address(layer1_start_addr);
    Main_Image_Width(LCD_XSIZE_TFT);
    Main_Window_Start_XY(0,0);
    Canvas_Image_Start_address(layer1_start_addr);
    Canvas_image_width(LCD_XSIZE_TFT);
    Active_Window_XY(0,0);
    Active_Window_WH(LCD_XSIZE_TFT,LCD_YSIZE_TFT);
}

