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

#define DEBUG_NAND_CONFIG  0

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
	//BOARD_WriteU32(0x402F0054, 0x00111103); // NANDCR1
	BOARD_WriteU32(0x402F0054, 0x0047472A);
	//BOARD_WriteU32(0x402F0058, 0x10002109); // NANDCR2
	BOARD_WriteU32(0x402F0058, 0x20160813);
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

bool NAND_IsReady(void)
{
	return(SEMC_IsNandReady(SEMC));
}

void NAND_ProcessCommand(uint8_t * cmd_buf, uint32_t cmd_queue_len)
{
	uint32_t col;
	uint32_t row;
	uint32_t address;
	uint16_t commandCode = 0;

#if(DEBUG_NAND_CONFIG == 1)
    uint32_t idx;
    debug("\nNAND_ProcessCommand: Len:%d, Buf:", cmd_queue_len);
    for(idx = 0; idx < cmd_queue_len; idx++) {
        debug(" %02X", cmd_buf[idx]);
    }
#endif

	if(cmd_queue_len == 0) {
		return;
	}

	address = 0;
    if(cmd_queue_len == 1) {
        /* Command only */
        commandCode = SEMC_BuildNandIPCommand(cmd_buf[0], kSEMC_NANDAM_ColumnRow, kSEMC_NANDCM_CommandHold);
	} else if(cmd_queue_len == 2) {
        /* Command + 1 Byte Addr */
        commandCode = SEMC_BuildNandIPCommand(cmd_buf[0], kSEMC_NANDAM_ColumnCA0, kSEMC_NANDCM_CommandAddressHold);
        address = cmd_buf[1];
	} else {
        commandCode = SEMC_BuildNandIPCommand(cmd_buf[0], kSEMC_NANDAM_ColumnRow, kSEMC_NANDCM_CommandAddressHold);
        col = cmd_buf[1] + (cmd_buf[2] << 8);
        row = cmd_buf[3] + (cmd_buf[4] << 8) + (cmd_buf[5] << 16);
        address = col | (row << 12);
	}

#if(DEBUG_NAND_CONFIG == 1)
    debug("\nCommandCode:%02X, address:%08X", commandCode, address);
#endif

	SEMC_SendIPCommand(SEMC, kSEMC_MemType_NAND, address, commandCode, 0, NULL);
}

void NAND_Read_Buf(uint8_t *buf, int len)
{
	SEMC_IPCommandNandRead(SEMC, 0, buf, len);
#if(DEBUG_NAND_CONFIG == 1)
	uint32_t idx;
	debug("\nNAND_Read_Buf:");
	for(idx = 0; idx < len; idx++) {
		debug(" %02X", buf[idx]);
	}
#endif
}

uint8_t NAND_Read_Status(void)
{
	uint8_t ret;
	status_t status = kStatus_Success;
    uint32_t readoutData;
    uint16_t commandCode;
    uint32_t slaveAddress;

    // Note: If there is only one plane per target, the READ STATUS (70h) command can be used
    //   to return status following any NAND command.
    // Note: In devices that have more than one plane per target, during and following interleaved
    //  die (multi-plane) operations, the READ STATUS ENHANCED command must be used to select the
    //  die (LUN) that should report status.

	// READ STATUS command is accepted by device even when it is busy (RDY = 0).
    commandCode = SEMC_BuildNandIPCommand(0x70, kSEMC_NANDAM_ColumnRow, kSEMC_NANDCM_CommandRead);

	// Note: For those command without address, the address should be valid as well,
	//  it shouldn't be out of IPG memory space, or SEMC IP will ignore this command.
	slaveAddress = CONFIG_SYS_NAND_BASE;

	status = SEMC_SendIPCommand(SEMC, kSEMC_MemType_NAND, slaveAddress, commandCode, 0, &readoutData);
    if (status != kStatus_Success)
    {
        printf("fsl_nand, NAND_Read_Status, SEMC_SendIPCommand Failed!\n");
    }

    return((uint8_t)(readoutData & 0xFF));
}

void NAND_Reset(void)
{
    status_t status = kStatus_Success;
    uint32_t dummyData = 0;

    // The RESET command may be executed with the target in any state.
    uint16_t commandCode = SEMC_BuildNandIPCommand(0xFF, kSEMC_NANDAM_ColumnRow, kSEMC_NANDCM_CommandHold);
    status = SEMC_SendIPCommand(SEMC, kSEMC_MemType_NAND, CONFIG_SYS_NAND_BASE, commandCode, 0, &dummyData);
    // wait
    while(SEMC_IsNandReady(SEMC) != true);
}

void NAND_ReadID(uint8_t *buf)
{
    status_t status = kStatus_Success;
    uint32_t readyCheckIntervalInUs;
    uint32_t dummyData = 0;
    uint32_t slaveAddress;
    uint16_t commandCode;

    // READ PAGE command is accepted by the device when it is ready (RDY = 1, ARDY = 1).
    while(SEMC_IsNandReady(SEMC) != true);

    commandCode = SEMC_BuildNandIPCommand(0x90U, kSEMC_NANDAM_ColumnCA0, kSEMC_NANDCM_CommandAddressHold);

    // Note: For those command without address, the address should be valid as well,
    //  it shouldn't be out of IPG memory space, or SEMC IP will ignore this command.
    slaveAddress = CONFIG_SYS_NAND_BASE;
    status = SEMC_SendIPCommand(SEMC, kSEMC_MemType_NAND, slaveAddress, commandCode, 0, &dummyData);

    if (status != kStatus_Success) {
    	printf("fsl_nand, NAND_ReadID, SEMC_SendIPCommand Failed!\n");
        return;
    }

    while(SEMC_IsNandReady(SEMC) != true);

    // Get ID Bytes
    status = SEMC_IPCommandNandRead(SEMC, slaveAddress, buf, 5);
#if(DEBUG_NAND_CONFIG == 1)
    int idx;
    debug("NAND Read ID: ");
    for(idx = 0; idx < 5; idx++) {
    	debug(" %02X", buf[idx]);
    }
    debug("\n");
#endif
    if (status != kStatus_Success) {
    	printf("fsl_nand, NAND_ReadID, SEMC_SendIPCommand Failed!\n");
        return;
    }
}


