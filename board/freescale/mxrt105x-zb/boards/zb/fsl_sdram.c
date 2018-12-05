/*
 * Copyright (c) 2018
 * Zimplistic Private Limited.
 * Sicris Rey Embay <sicris@zimplistic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include "stdint.h"
#include "fsl_sdram.h"
#include "fsl_iomuxc.h"

static void MEM_WriteU32(uint32_t addr, uint32_t val)
{
	*((volatile uint32_t*)addr) = val;
}

static uint32_t MEM_ReadU32(uint32_t addr)
{
	return((uint32_t)(*((volatile uint32_t*)addr)));
}

static void SDRAM_WaitIpCmdDone (void)
{
  unsigned int reg;
  do {
      reg = MEM_ReadU32(0x402F003C);
  }while((reg & 0x3) == 0);
}

void SDRAM_Init(void)
{
    // Config SDR Controller Registers/
    MEM_WriteU32(0x402F0000,0x10000004); // MCR
    MEM_WriteU32(0x402F0008,0x00030524); // BMCR0
    MEM_WriteU32(0x402F000C,0x06030524); // BMCR1
    MEM_WriteU32(0x402F0010,0x8000001B); // BR0, 32MB
    MEM_WriteU32(0x402F0014,0x8200001B); // BR1, 32MB
    MEM_WriteU32(0x402F0018,0x8400001B); // BR2, 32MB
    MEM_WriteU32(0x402F001C,0x8600001B); // BR3, 32MB
    MEM_WriteU32(0x402F0020,0x90000021); // BR4,
    MEM_WriteU32(0x402F0024,0xA0000019); // BR5,
    MEM_WriteU32(0x402F0028,0xA8000017); // BR6,
    MEM_WriteU32(0x402F002C,0xA900001B); // BR7,
    MEM_WriteU32(0x402F0030,0x00000021); // BR8,
    MEM_WriteU32(0x402F0004,0x000079A8);  //IOCR,SEMC_CCSX0 as NOR CE, SEMC_CSX1 as PSRAM CE, SEMC_CSX2 as NAND CE, SEMC_CSX3 as DBI CE.

    //MEM_WriteU32(0x402F0004,0x00000008); // IOCR, SEMC_CCSX0 as SDRAM_CS1
    MEM_WriteU32(0x402F0040,0x00000F31); // SDRAMCR0
    MEM_WriteU32(0x402F0044,0x00652922); // SDRAMCR1
    MEM_WriteU32(0x402F0048,0x00010920); // SDRAMCR2
    MEM_WriteU32(0x402F004C,0x50210A08); // SDRAMCR3

    MEM_WriteU32(0x402F0080,0x00000021); // DBICR0
    MEM_WriteU32(0x402F0084,0x00888888); // DBICR1
    MEM_WriteU32(0x402F0094,0x00000002); // IPCR1
    MEM_WriteU32(0x402F0098,0x00000000); // IPCR2

    MEM_WriteU32(0x402F0090,0x80000000); // IPCR0
    MEM_WriteU32(0x402F009C,0xA55A000F); // IPCMD, SD_CC_IPREA
    SDRAM_WaitIpCmdDone();
    MEM_WriteU32(0x402F0090,0x80000000); // IPCR0
    MEM_WriteU32(0x402F009C,0xA55A000C); // SD_CC_IAF
    SDRAM_WaitIpCmdDone();
    MEM_WriteU32(0x402F0090,0x80000000); // IPCR0
    MEM_WriteU32(0x402F009C,0xA55A000C); // SD_CC_IAF
    SDRAM_WaitIpCmdDone();
    MEM_WriteU32(0x402F00A0,0x00000033); // IPTXDAT
    MEM_WriteU32(0x402F0090,0x80000000); // IPCR0
    MEM_WriteU32(0x402F009C,0xA55A000A); // SD_CC_IMS
    SDRAM_WaitIpCmdDone();

    MEM_WriteU32(0x402F004C,0x50210A09 ); // enable sdram self refresh again after initialization done.
}

