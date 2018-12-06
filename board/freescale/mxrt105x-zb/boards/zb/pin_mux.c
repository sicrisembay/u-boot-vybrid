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

#define BOARD_LED_GPIO      GPIO2
#define BOARD_LED_GPIO_PIN  31U
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
	gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    CLOCK_EnableClock(kCLOCK_Iomuxc);

    /********************* Initialize LED IO *****************************************************/
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_GPIO2_IO31, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_15_GPIO2_IO31, 0x10B0u);
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);

    /* Initialize UART IO */
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

    /* Initialize SEMC IO used for SDRAM and NAND Flash */
    // Config IOMUX for sdr
    BOARD_WriteU32(0x401F8014,0x00000000); // EMC_00
    BOARD_WriteU32(0x401F8018,0x00000000); // EMC_01
    BOARD_WriteU32(0x401F801C,0x00000000); // EMC_02
    BOARD_WriteU32(0x401F8020,0x00000000); // EMC_03
    BOARD_WriteU32(0x401F8024,0x00000000); // EMC_04
    BOARD_WriteU32(0x401F8028,0x00000000); // EMC_05
    BOARD_WriteU32(0x401F802C,0x00000000); // EMC_06
    BOARD_WriteU32(0x401F8030,0x00000000); // EMC_07
    BOARD_WriteU32(0x401F8034,0x00000000); // EMC_08
    BOARD_WriteU32(0x401F8038,0x00000000); // EMC_09
    BOARD_WriteU32(0x401F803C,0x00000000); // EMC_10
    BOARD_WriteU32(0x401F8040,0x00000000); // EMC_11
    BOARD_WriteU32(0x401F8044,0x00000000); // EMC_12
    BOARD_WriteU32(0x401F8048,0x00000000); // EMC_13
    BOARD_WriteU32(0x401F804C,0x00000000); // EMC_14
    BOARD_WriteU32(0x401F8050,0x00000000); // EMC_15
    BOARD_WriteU32(0x401F8054,0x00000000); // EMC_16
    BOARD_WriteU32(0x401F8058,0x00000000); // EMC_17
    BOARD_WriteU32(0x401F805C,0x00000000); // EMC_18
    BOARD_WriteU32(0x401F8060,0x00000000); // EMC_19
    BOARD_WriteU32(0x401F8064,0x00000000); // EMC_20
    BOARD_WriteU32(0x401F8068,0x00000000); // EMC_21
    BOARD_WriteU32(0x401F806C,0x00000000); // EMC_22
    BOARD_WriteU32(0x401F8070,0x00000000); // EMC_23
    BOARD_WriteU32(0x401F8074,0x00000000); // EMC_24
    BOARD_WriteU32(0x401F8078,0x00000000); // EMC_25
    BOARD_WriteU32(0x401F807C,0x00000000); // EMC_26
    BOARD_WriteU32(0x401F8080,0x00000000); // EMC_27
    BOARD_WriteU32(0x401F8084,0x00000000); // EMC_28
    BOARD_WriteU32(0x401F8088,0x00000000); // EMC_29
    BOARD_WriteU32(0x401F808C,0x00000000); // EMC_30
    BOARD_WriteU32(0x401F8090,0x00000000); // EMC_31
    BOARD_WriteU32(0x401F8094,0x00000000); // EMC_32
    BOARD_WriteU32(0x401F8098,0x00000000); // EMC_33
    BOARD_WriteU32(0x401F809C,0x00000000); // EMC_34
    BOARD_WriteU32(0x401F80A0,0x00000000); // EMC_35
    BOARD_WriteU32(0x401F80A4,0x00000000); // EMC_36
    BOARD_WriteU32(0x401F80A8,0x00000000); // EMC_37
    BOARD_WriteU32(0x401F80AC,0x00000000); // EMC_38
    BOARD_WriteU32(0x401F80B0,0x00000010); // EMC_39, DQS PIN, enable SION
    BOARD_WriteU32(0x401F80B4,0x00000000); // EMC_40, RDY PIN
    BOARD_WriteU32(0x401F80B8,0x00000000); // EMC_41, CSX0 Pin

    // PAD ctrl
    //drive strength = 0x7 to increase drive strength
    //otherwise the data7 bit may fail.
    //0x000000F1: the driver strength may not enough.
    BOARD_WriteU32(0x401F8204,0x000110F9); // EMC_00
    BOARD_WriteU32(0x401F8208,0x000110F9); // EMC_01
    BOARD_WriteU32(0x401F820C,0x000110F9); // EMC_02
    BOARD_WriteU32(0x401F8210,0x000110F9); // EMC_03
    BOARD_WriteU32(0x401F8214,0x000110F9); // EMC_04
    BOARD_WriteU32(0x401F8218,0x000110F9); // EMC_05
    BOARD_WriteU32(0x401F821C,0x000110F9); // EMC_06
    BOARD_WriteU32(0x401F8220,0x000110F9); // EMC_07
    BOARD_WriteU32(0x401F8224,0x000110F9); // EMC_08
    BOARD_WriteU32(0x401F8228,0x000110F9); // EMC_09
    BOARD_WriteU32(0x401F822C,0x000110F9); // EMC_10
    BOARD_WriteU32(0x401F8230,0x000110F9); // EMC_11
    BOARD_WriteU32(0x401F8234,0x000110F9); // EMC_12
    BOARD_WriteU32(0x401F8238,0x000110F9); // EMC_13
    BOARD_WriteU32(0x401F823C,0x000110F9); // EMC_14
    BOARD_WriteU32(0x401F8240,0x000110F9); // EMC_15
    BOARD_WriteU32(0x401F8244,0x000110F9); // EMC_16
    BOARD_WriteU32(0x401F8248,0x000110F9); // EMC_17
    BOARD_WriteU32(0x401F824C,0x000110F9); // EMC_18
    BOARD_WriteU32(0x401F8250,0x000110F9); // EMC_19
    BOARD_WriteU32(0x401F8254,0x000110F9); // EMC_20
    BOARD_WriteU32(0x401F8258,0x000110F9); // EMC_21
    BOARD_WriteU32(0x401F825C,0x000110F9); // EMC_22
    BOARD_WriteU32(0x401F8260,0x000110F9); // EMC_23
    BOARD_WriteU32(0x401F8264,0x000110F9); // EMC_24
    BOARD_WriteU32(0x401F8268,0x000110F9); // EMC_25
    BOARD_WriteU32(0x401F826C,0x000110F9); // EMC_26
    BOARD_WriteU32(0x401F8270,0x000110F9); // EMC_27
    BOARD_WriteU32(0x401F8274,0x000110F9); // EMC_28
    BOARD_WriteU32(0x401F8278,0x000110F9); // EMC_29
    BOARD_WriteU32(0x401F827C,0x000110F9); // EMC_30
    BOARD_WriteU32(0x401F8280,0x000110F9); // EMC_31
    BOARD_WriteU32(0x401F8284,0x000110F9); // EMC_32
    BOARD_WriteU32(0x401F8288,0x000110F9); // EMC_33
    BOARD_WriteU32(0x401F828C,0x000110F9); // EMC_34
    BOARD_WriteU32(0x401F8290,0x000110F9); // EMC_35
    BOARD_WriteU32(0x401F8294,0x000110F9); // EMC_36
    BOARD_WriteU32(0x401F8298,0x000110F9); // EMC_37
    BOARD_WriteU32(0x401F829C,0x000110F9); // EMC_38
    BOARD_WriteU32(0x401F82A0,0x000110F9); // EMC_39, DQS Pin
    BOARD_WriteU32(0x401F82A4,0x0000F0F1); // EMC_40, RDY Pin
    BOARD_WriteU32(0x401F82A8,0x000110F9); // EMC_41, CSX0 Pin

#if 0
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_USDHC1_CMD, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_USDHC1_CLK, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3, 0);
    /* card detect pin */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_USDHC1_CD_B, 0);
    /* SD0_VSELECT */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_USDHC1_VSELECT, 0);// IOMUXC_GPIO_B1_14_USDHC1_VSELECT, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_GPIO2_IO31, 0);// IOMUXC_GPIO_B1_14_USDHC1_VSELECT, 0);

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_USDHC1_CMD, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                       IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                       IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
                                                       IOMUXC_SW_PAD_CTL_PAD_DSE(4));
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_USDHC1_CLK, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                       IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
                                                       IOMUXC_SW_PAD_CTL_PAD_DSE(4));
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(1) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(4));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(1) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(4));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(1) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(4));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(1) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(4));

    /* card detect pin */
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_12_USDHC1_CD_B, IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                            IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                            IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);

    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_USDHC1_VSELECT, IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | 
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 

    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_15_GPIO2_IO31, IOMUXC_SW_PAD_CTL_PAD_PUS(3) | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | 
                                                            IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                            IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
#endif
}

void BOARD_SetLedState(uint8_t state)
{
	if(state != 0) {
		GPIO_WritePinOutput(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, 1U);
	} else {
		GPIO_WritePinOutput(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, 0U);
	}
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
