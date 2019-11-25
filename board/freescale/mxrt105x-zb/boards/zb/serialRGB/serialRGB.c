/*
 * Copyright (c) 2018
 * Zimplistic Private Limited.
 * Sicris Rey Embay <sicris@zimplistic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "fsl_elcdif.h"
#include "serialRGB.h"

#define COLOR_BLACK						(0x00000000)
#define COLOR_RED						(0x00FF0000)
#define COLOR_GREEN						(0x0000FF00)
#define COLOR_BLUE						(0x000000FF)

/* Definition of the panel Properties */
#define LCD_HSYNC_WIDTH                 (4)
#define LCD_HFP                         (8)
#define LCD_HBP                         (43)
#define LCD_VSYNC_WIDTH                 (4)
#define LCD_VFP                         (8)
#define LCD_VBP                         (12)
/*
 * Notes from panel properties
 * The desired output frame rate is 30Hz.
 * Pixel clock is = 30 * ((320 * 3) + 2 + 18 + 68) * (240 + 2+ 10 + 11) = 8.269 MHz
 * Actual LCDIF clock is set to 8.25MHz (see below)
 */
#define LCD_PLL_LOOPDIV     (33)
#define LCD_PLL_POSTDIV     (16)
/* LCD_IF_CLK = 24MHz * LCD_PLL_LOOPDIV / LCD_PLL_POSTDIV
 * LCD_IF_CLK = 24MHz * 33 / 16 = 49.5MHz
 */
#define LCD_IF_PREDIV       (2)
/* LCD_DOT_CLK = 49.5MHz / 3 / 1 = 16.5MHz */
#define LCD_IF_DIV          (0)

void serialRGB_init(void)
{
	int i=0;
	uint32_t *pFrame = 0;
	clock_video_pll_config_t rgbPllConfig = {
	    /* 24MHz * (32) / 16 = 49.5MHz */
	    .loopDivider = LCD_PLL_LOOPDIV,
	    .postDivider = LCD_PLL_POSTDIV,
	    .numerator = 0,
	    .denominator = 0
	};

	elcdif_rgb_mode_config_t lcd_rgbConfig = {
	    .panelWidth = CONFIG_MXRT105X_ZB_DISP_WIDTH * 3,
	    .panelHeight = CONFIG_MXRT105X_ZB_DISP_HEIGHT,
	    .hsw = LCD_HSYNC_WIDTH,
	    .hfp = LCD_HFP,
	    .hbp = LCD_HBP,
	    .vsw = LCD_VSYNC_WIDTH,
	    .vfp = LCD_VFP,
	    .vbp = LCD_VBP,
	    .polarityFlags = (kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DataEnableActiveHigh | kELCDIF_DriveDataOnFallingClkEdge),
	    .bufferAddr = (uint32_t)CONFIG_FB_ADDR,
	    .pixelFormat = kELCDIF_PixelFormatRAW8,
	    .dataBus = kELCDIF_DataBus8Bit
	};

    /** Initialize LCD pins **********************************************************************/
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_00_LCD_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_01_LCD_ENABLE, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_02_LCD_HSYNC, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_03_LCD_VSYNC, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_04_LCD_DATA00, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_05_LCD_DATA01, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_06_LCD_DATA02, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_07_LCD_DATA03, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_08_LCD_DATA04, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_09_LCD_DATA05, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_10_LCD_DATA06, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_11_LCD_DATA07, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_00_LCD_CLK, 0x0100E1u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_01_LCD_ENABLE, 0x0100E1u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_02_LCD_HSYNC, 0x0100E1u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_03_LCD_VSYNC, 0x0100E1u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_04_LCD_DATA00, 0x0100E1u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_05_LCD_DATA01, 0x0100E1u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_06_LCD_DATA02, 0x0100E1u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_07_LCD_DATA03, 0x0100E1u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_08_LCD_DATA04, 0x0100E1u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_09_LCD_DATA05, 0x0100E1u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_10_LCD_DATA06, 0x0100E1u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_11_LCD_DATA07, 0x0100E1u);
#if !defined(CONFIG_MXRT105X_ZB_DISP_STM8_DAUGHTER)
    // Backlight
	gpio_pin_config_t outputHigh_config = {kGPIO_DigitalOutput, 1, kGPIO_NoIntmode};
	CLOCK_EnableClock(kCLOCK_Iomuxc);
    GPIO_PinInit(GPIO1, 2U, &outputHigh_config);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_02_GPIO1_IO02, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_02_GPIO1_IO02, 0xB0U);
#endif

	/* Configure RGB LCD PLL */
	CLOCK_InitVideoPll(&rgbPllConfig);
    CLOCK_SetMux(kCLOCK_Lcdif1PreMux, 2);
    CLOCK_SetDiv(kCLOCK_Lcdif1PreDiv, LCD_IF_PREDIV);
    CLOCK_SetDiv(kCLOCK_Lcdif1Div, LCD_IF_DIV);
    /* Initialize RGB IF */
	ELCDIF_RgbModeInit(LCDIF, &lcd_rgbConfig);
	/* Additional initialization not supported by NXP driver */
	LCDIF->CTRL_SET = LCDIF_CTRL_DATA_SHIFT_DIR(1U);
	LCDIF->CTRL_CLR = LCDIF_CTRL_INPUT_DATA_SWIZZLE_MASK;
	LCDIF->CTRL_SET = LCDIF_CTRL_INPUT_DATA_SWIZZLE(1U);
	LCDIF->CTRL1_CLR = LCDIF_CTRL1_BYTE_PACKING_FORMAT_MASK;
	LCDIF->CTRL1_SET = LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x07U);

	/* Clear frame buffer */
	pFrame = (uint32_t *)CONFIG_FB_ADDR;
	for(i = 0; i < (CONFIG_MXRT105X_ZB_DISP_HEIGHT * CONFIG_MXRT105X_ZB_DISP_WIDTH * CONFIG_MXRT105X_ZB_DISP_BUFFER_CNT); i++) {
		pFrame[i] = COLOR_RED;
	}

	ELCDIF_RgbModeStart(LCDIF);
}


/*******************************************************************************
 * EOF
 ******************************************************************************/
