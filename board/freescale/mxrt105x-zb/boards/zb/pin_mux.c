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
#include "pin_mux.h"

#if defined(CONFIG_MXRT105X_ZB_VER3)
#define BOARD_LED_GPIO      GPIO2
#define BOARD_LED_GPIO_PIN  31U
#define BOARD_PWR_SW_GPIO       GPIO2
#define BOARD_PWR_SW_GPIO_PIN   11U
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_WriteU32(uint32_t addr, uint32_t val)
{
	*((volatile uint32_t*)addr) = val;
}

uint32_t BOARD_ReadU32(uint32_t addr)
{
	return((uint32_t)(*((volatile uint32_t*)addr)));
}

void BOARD_InitPins(void) {
#if defined(CONFIG_MXRT105X_ZB_VER3)
	gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
	gpio_pin_config_t outputHigh_config = {kGPIO_DigitalOutput, 1, kGPIO_NoIntmode};
    CLOCK_EnableClock(kCLOCK_Iomuxc);

    /********************* Initialize LED IO *****************************************************/
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_GPIO2_IO31, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_15_GPIO2_IO31, 0x10B0u);
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);

    /** Initialize UART IO ***********************************************************************/
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 is configured as LPUART1_TX */
                     0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 is configured as LPUART1_RX */
                     0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 PAD functional properties : */
                     0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 PAD functional properties : */
                     0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
    /** Enable Board Voltage Supply **************************************************************/
    GPIO_PinInit(BOARD_PWR_SW_GPIO, BOARD_PWR_SW_GPIO_PIN, &outputHigh_config);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_11_GPIO2_IO11, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_11_GPIO2_IO11, 0xB0U);

    /** Initialize USB IO ************************************************************************/
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_01_USB_OTG1_ID, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_01_USB_OTG1_ID, 0xF8B0U);
#define USB_OTG1_PWR_GPIO		GPIO1
#define USB_OTG1_PWR_GPIO_PIN   17U
    GPIO_PinInit(USB_OTG1_PWR_GPIO, USB_OTG1_PWR_GPIO_PIN, &outputHigh_config);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17, 0xB8B0u);

#elif defined(CONFIG_MXRT105X_ZB_VER4)
    gpio_pin_config_t outputHigh_config = {kGPIO_DigitalOutput, 1, kGPIO_NoIntmode};
    CLOCK_EnableClock(kCLOCK_Iomuxc);

    /** Initialize Pins for ESP32 EN and Boot ****************************************************/
    GPIO_PinInit(GPIO3, 7U, &outputHigh_config);
    GPIO_PinInit(GPIO3, 8U, &outputHigh_config);
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B1_07_GPIO3_IO07,        /* GPIO_SD_B1_07 is configured as GPIO3_IO07 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B1_08_GPIO3_IO08,        /* GPIO_SD_B1_08 is configured as GPIO3_IO08 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B1_07_GPIO3_IO07,        /* GPIO_SD_B1_07 PAD functional properties : */
        0xA8B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                   Drive Strength Field: R0/6
                                                   Speed Field: medium(100MHz)
                                                   Open Drain Enable Field: Open Drain Enabled
                                                   Pull / Keep Enable Field: Pull/Keeper Disabled
                                                   Pull / Keep Select Field: Pull
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                   Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B1_08_GPIO3_IO08,        /* GPIO_SD_B1_08 PAD functional properties : */
        0xA8B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                   Drive Strength Field: R0/6
                                                   Speed Field: medium(100MHz)
                                                   Open Drain Enable Field: Open Drain Enabled
                                                   Pull / Keep Enable Field: Pull/Keeper Disabled
                                                   Pull / Keep Select Field: Pull
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                   Hyst. Enable Field: Hysteresis Disabled */

    /** Initialize UART IO ***********************************************************************/
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 is configured as LPUART1_TX */
					 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 is configured as LPUART1_RX */
					 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 PAD functional properties : */
					 0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
												 Drive Strength Field: R0/6
												 Speed Field: medium(100MHz)
												 Open Drain Enable Field: Open Drain Disabled
												 Pull / Keep Enable Field: Pull/Keeper Enabled
												 Pull / Keep Select Field: Keeper
												 Pull Up / Down Config. Field: 100K Ohm Pull Down
												 Hyst. Enable Field: Hysteresis Disabled */
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 PAD functional properties : */
					 0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
												 Drive Strength Field: R0/6
												 Speed Field: medium(100MHz)
												 Open Drain Enable Field: Open Drain Disabled
												 Pull / Keep Enable Field: Pull/Keeper Enabled
												 Pull / Keep Select Field: Keeper
												 Pull Up / Down Config. Field: 100K Ohm Pull Down
												 Hyst. Enable Field: Hysteresis Disabled */
    /** Initialize USB IO ************************************************************************/
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_01_USB_OTG1_ID, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_01_USB_OTG1_ID, 0xF8B0U);
#define USB_OTG1_PWR_GPIO		GPIO1
#define USB_OTG1_PWR_GPIO_PIN   17U
    GPIO_PinInit(USB_OTG1_PWR_GPIO, USB_OTG1_PWR_GPIO_PIN, &outputHigh_config);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17, 0xB8B0u);
#endif
}

void BOARD_SetLedState(uint8_t state)
{
#if defined(CONFIG_MXRT105X_ZB_VER3)
	if(state != 0) {
		GPIO_WritePinOutput(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, 1U);
	} else {
		GPIO_WritePinOutput(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, 0U);
	}
#endif
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
