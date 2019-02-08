/*
 * Freescale iMXRT105x USB Host driver
 *
 * Copyright (C) 2019 Sicris Rey Embay <sicris.embay@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include "../../../board/freescale/mxrt105x-zb/boards/zb/fsl_usb_ehci.h"
#include "ehci.h"

#define DEBUG_EHCI_MXRT105X     (0)

int __weak board_ehci_hcd_init(int port)
{
	return 0;
}

int __weak board_ehci_hcd_exit(int port)
{
	return 0;
}

int ehci_hcd_init(int index, enum usb_init_type init,
		struct ehci_hccr **hccr, struct ehci_hcor **hcor)
{
	int ret;
	uint32_t usb_base, cap_base;

	if (index != 0) {
		printf("Invalid port index (index = %d)!\n", index);
		return -EINVAL;
	}
#if(DEBUG_EHCI_MXRT105X != 0)
	// debug
	printf("ehci_mxrt105x.c:ehci_hcd_init()... start\n");
#endif /* #if(DEBUG_EHCI_MXRT105X != 0) */

	ret = board_ehci_hcd_init(index);
	if (ret) {
		return ret;
	}
	/* Reset the PHY block */
	USB_EHCI_ResetPhy(index);

	/* Enable USB clock */
	USB_EHCI_EnableClock(index, true);

	/* Start USB PHY */
	USB_EHCI_StartPhy(index);

	usb_base = 0x402E0000 + 0x100; /* MXRT1052X USB_0 CAPLENTH */
	*hccr = (struct ehci_hccr *)usb_base;

	cap_base = (*hccr)->cr_capbase;
	*hcor = (struct ehci_hcor *)(usb_base + HC_LENGTH(cap_base));
#if(DEBUG_EHCI_MXRT105X != 0)
	// debug
	printf("ehci_mxrt105x.c:ehci_hcd_init()... end\n");
#endif /* #if(DEBUG_EHCI_MXRT105X != 0) */
	return 0;
}

int ehci_hcd_stop(int index)
{
	uint32_t usb_base, cap_base;
	struct ehci_hccr *hccr;
	struct ehci_hcor *hcor;

	if (index != 0) {
		printf("Invalid port index (index = %d)!\n", index);
		return -EINVAL;
	}

#if(DEBUG_EHCI_MXRT105X != 0)
	// debug
	printf("ehci_mxrt105x.c:ehci_hcd_stop()... start\n");
#endif /* #if(DEBUG_EHCI_MXRT105X != 0) */

	/* Stop the USB port */
	usb_base = 0x402E0000 + 0x100; /* MXRT1052X USB_0 CAPLENTH */
	hccr = (struct ehci_hccr *)usb_base;
	cap_base = hccr->cr_capbase;
	hcor = (struct ehci_hcor *)(usb_base + HC_LENGTH(cap_base));
	hcor->or_usbcmd &= ~CMD_RUN;

	/* Disable the PHY */
	USB_EHCI_StopPhy(index);

	/* Disable USB clock */
	USB_EHCI_EnableClock(index, false);

	board_ehci_hcd_exit(index);

#if(DEBUG_EHCI_MXRT105X != 0)
	printf("ehci_hcd_stop()... end\n");
#endif /* #if(DEBUG_EHCI_MXRT105X != 0) */

	return 0;
}
