/*
 * Copyright (c) 2018
 * Zimplistic Private Limited.
 * Sicris Rey Embay <sicris@zimplistic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include "fsl_usb_ehci.h"
#include "../../devices/MIMXRT1052/MIMXRT1052.h"
#include "../../devices/MIMXRT1052/drivers/fsl_clock.h"
#include "../../../../../include/configs/mxrt105x-zb.h"

#define BOARD_USB_PHY_D_CAL      (0x0CU)
#define BOARD_USB_PHY_TXCAL45DP  (0x06U)
#define BOARD_USB_PHY_TXCAL45DM  (0x06U)

void USB_EHCI_ResetPhy(int index)
{
	/* Assert soft reset */
    switch(index) {
    case 0:
    	USBPHY1->CTRL_SET = USBPHY_CTRL_SET_SFTRST_MASK;
    	break;
    case 1:
    	USBPHY2->CTRL_SET = USBPHY_CTRL_SET_SFTRST_MASK;
    	break;
    default:
    	break;
    }

    /* delay */
    udelay(10);

    /* De-assert soft reset and Gate UTMI clocks */
    switch(index) {
    case 0:
    	USBPHY1->CTRL_CLR = USBPHY_CTRL_SET_SFTRST_MASK | USBPHY_CTRL_CLKGATE_MASK;
    	break;
    case 1:
    	USBPHY2->CTRL_CLR = USBPHY_CTRL_SET_SFTRST_MASK | USBPHY_CTRL_CLKGATE_MASK;
    	break;
    default:
    	break;
    }
}

void USB_EHCI_EnableClock(int index, bool bEnable)
{
    switch(index) {
    case 0:
    	if(bEnable) {
			CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
			CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
    	} else {
    		//CLOCK_DisableUsbhs0PhyPllClock();
    	}
    	break;
    case 1:
    	if(bEnable) {
			CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
			CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, 480000000U);
    	} else {
    		//CLOCK_DisableUsbhs1PhyPllClock();
    	}
    	break;
    default:
    	break;
    }
}


void USB_EHCI_StartPhy(int index)
{
	switch(index) {
	case 0:
		USB_ANALOG->INSTANCE[0].CHRG_DETECT_SET = USB_ANALOG_CHRG_DETECT_CHK_CHRG_B(1) |
									USB_ANALOG_CHRG_DETECT_EN_B(1);
		USBPHY1->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL2_MASK;
		USBPHY1->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL3_MASK;
		USBPHY1->PWD = 0U;
		USBPHY1->TX = ((USBPHY1->TX & (~(USBPHY_TX_D_CAL_MASK | USBPHY_TX_TXCAL45DM_MASK | USBPHY_TX_CLR_TXCAL45DP_MASK))) |
				 (USBPHY_TX_D_CAL(BOARD_USB_PHY_D_CAL) | USBPHY_TX_TXCAL45DP(BOARD_USB_PHY_TXCAL45DP) |
				  USBPHY_TX_TXCAL45DM(BOARD_USB_PHY_TXCAL45DM)));
		break;
	case 1:
		USB_ANALOG->INSTANCE[1].CHRG_DETECT_SET = USB_ANALOG_CHRG_DETECT_CHK_CHRG_B(1) |
									USB_ANALOG_CHRG_DETECT_EN_B(1);
		USBPHY2->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL2_MASK;
		USBPHY2->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL3_MASK;
		USBPHY2->PWD = 0U;
		USBPHY2->TX = ((USBPHY1->TX & (~(USBPHY_TX_D_CAL_MASK | USBPHY_TX_TXCAL45DM_MASK | USBPHY_TX_CLR_TXCAL45DP_MASK))) |
				 (USBPHY_TX_D_CAL(BOARD_USB_PHY_D_CAL) | USBPHY_TX_TXCAL45DP(BOARD_USB_PHY_TXCAL45DP) |
				  USBPHY_TX_TXCAL45DM(BOARD_USB_PHY_TXCAL45DM)));
		break;
	default:
		break;
	}
}


void USB_EHCI_StopPhy(int index)
{
	uint32_t tmp = USBPHY_PWD_RXPWDRX_MASK | USBPHY_PWD_RXPWDDIFF_MASK |
					USBPHY_PWD_RXPWD1PT1_MASK | USBPHY_PWD_RXPWDENV_MASK |
					USBPHY_PWD_TXPWDV2I_MASK | USBPHY_PWD_TXPWDIBIAS_MASK |
					USBPHY_PWD_TXPWDFS_MASK;
	switch(index) {
	case 0:
		USBPHY1->PWD = tmp;
		break;
	case 1:
		USBPHY2->PWD = tmp;
		break;
	default:
		break;
	}
}
