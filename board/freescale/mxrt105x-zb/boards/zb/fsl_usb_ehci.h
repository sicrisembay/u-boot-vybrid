/*
 * Copyright (c) 2018
 * Zimplistic Private Limited.
 * Sicris Rey Embay <sicris@zimplistic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _FSL_USB_EHCI_H_
#define _FSL_USB_EHCI_H_

#include <stdbool.h>

void USB_EHCI_ResetPhy(int index);
void USB_EHCI_EnableClock(int index, bool bEnable);
void USB_EHCI_StartPhy(int index);
void USB_EHCI_StopPhy(int index);

#endif // #ifndef _FSL_USB_EHCI_H_

