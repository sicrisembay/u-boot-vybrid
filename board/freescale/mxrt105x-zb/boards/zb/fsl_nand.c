/*
 * Copyright (c) 2018
 * Zimplistic Private Limited.
 * Sicris Rey Embay <sicris@zimplistic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include "fsl_nand.h"
#include "fsl_semc.h"
#include "pin_mux.h"
#include "../../../../../include/configs/mxrt105x-zb.h"
#include "../../../../../include/common.h"

#define DEBUG_NAND_CONFIG 1

#define NAND_SIZE_KB    (131072)

void NAND_Init(void)
{
	uint32_t clockFreq = CLOCK_GetFreq(kCLOCK_SemcClk);
#if 0
	SEMC_Type *base = SEMC;
	semc_nand_config_t nandConfig;

	/* Initialize configure structure */
	memset(&nandConfig, 0, sizeof(semc_nand_config_t));
	nandConfig.cePinMux = kSEMC_MUXCSX0;
	nandConfig.address = CONFIG_SYS_NAND_BASE;
	nandConfig.memsize_kbytes = NAND_SIZE_KB;
	nandConfig.waitRdyHighPolarity = true;
	nandConfig.edoModeEnabled = false;
	nandConfig.columnAddrBitNum = kSEMC_NandColum_12bit;
	nandConfig.burstLen = kSEMC_Nand_BurstLen1;
	nandConfig.portSize = kSEMC_PortSize8Bit;
	nandConfig.csSetup_Ns = 20;
	nandConfig.ceHold_Ns = 5;
	nandConfig.weLow_Ns = 12;
	nandConfig.weHigh_Ns = 10;
	nandConfig.reLow_Ns = 12;
	nandConfig.reHigh_Ns = 10;
	nandConfig.turnAround_Ns = 10;                       /*!< Turnaround time. */
	nandConfig.ceInterval_Ns = 25;
	nandConfig.wehigh2Relow_Ns = 60;
	nandConfig.rehigh2Welow_Ns = 30;
	nandConfig.ale2WriteStart_Ns = 70;
	nandConfig.ready2Relow_Ns = 20;
	nandConfig.wehigh2Busy_Ns = 100;

	SEMC_ConfigureNAND(SEMC, kSEMC_NAND_IP, &nandConfig, clockFreq);
#else
	uint32_t tempRegVal = 0;
	BOARD_WriteU32(0x402F0030, 0x0000001F); // BR8
	BOARD_WriteU32(0x402F0050, 0x00000560); // NANDCR0
	BOARD_WriteU32(0x402F0054, 0x00111103); // NANDCR1
	BOARD_WriteU32(0x402F0058, 0x10002109); // NANDCR2
	BOARD_WriteU32(0x402F005C, 0x00000000); // NANDCR3
	/* Set IOCR */
	tempRegVal = BOARD_ReadU32(0x402F0004);
	tempRegVal |= 0x20;                     // MUX_CSX0 - NAND CE#
	BOARD_WriteU32(0x402F0004, tempRegVal);
#endif

#if(DEBUG_NAND_CONFIG == 1)
    debug("\nSEMC Configuration");
    debug("\n MCR:   %08X", BOARD_ReadU32(0x402F0000));
    debug("\n IOCR:  %08X", BOARD_ReadU32(0x402F0004));
    debug("\n BMCR0: %08X", BOARD_ReadU32(0x402F0008));
    debug("\n BMCR1: %08X", BOARD_ReadU32(0x402F000C));
    debug("\n BR0:   %08X", BOARD_ReadU32(0x402F0010));
    debug("\n BR1:   %08X", BOARD_ReadU32(0x402F0014));
    debug("\n BR2:   %08X", BOARD_ReadU32(0x402F0018));
    debug("\n BR3:   %08X", BOARD_ReadU32(0x402F001C));
    debug("\n BR4:   %08X", BOARD_ReadU32(0x402F0020));
    debug("\n BR5:   %08X", BOARD_ReadU32(0x402F0024));
    debug("\n BR6:   %08X", BOARD_ReadU32(0x402F0028));
    debug("\n BR7:   %08X", BOARD_ReadU32(0x402F002C));
    debug("\n BR8:   %08X", BOARD_ReadU32(0x402F0030));
    debug("\nNAND SEMC Configuration");
    debug("\n ClockFreq: %d", clockFreq);
    debug("\n NANDCR0: %08X", BOARD_ReadU32(0x402F0050));
    debug("\n NANDCR1: %08X", BOARD_ReadU32(0x402F0054));
    debug("\n NANDCR2: %08X", BOARD_ReadU32(0x402F0058));
    debug("\n NANDCR3: %08X", BOARD_ReadU32(0x402F005C));
    debug("\n IPCR0: %08X", BOARD_ReadU32(0x402F0090));
    debug("\n IPCR1: %08X", BOARD_ReadU32(0x402F0094));
    debug("\n IPCR2: %08X", BOARD_ReadU32(0x402F0098));
    debug("\n");
#endif
}

