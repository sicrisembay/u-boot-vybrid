/*
 * Copyright (c) 2018
 * Zimplistic Private Limited.
 * Sicris Rey Embay <sicris@zimplistic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include "fsl_sdram.h"
#include "fsl_iomuxc.h"
#include "fsl_semc.h"
#include "../../../../../include/common.h"

#define DEBUG_SDRAM_CONFIG 0

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
#if 0 /* Done in DCD */
	// Configure SDRAM
	MEM_WriteU32(0x402F0010,0x8000001B); // BR0, 32MB
    MEM_WriteU32(0x402F0040,0x00000F31); // SDRAMCR0, Bus Width:16, Burst Len:8, Col Addr:9, CAS Lat:3
    MEM_WriteU32(0x402F0044,0x00652922); // SDRAMCR1, PRE2ACT: 23ns, ACT2RW: 23ns, RFRC:75ns, WRC:23ns, CKEOFF:45ns, ACT2PRE:53ns
    MEM_WriteU32(0x402F0048,0x00010920); // SDRAMCR2, SRRC:248ns, REF2EF:75ns, ACT2ACT:15ns
    MEM_WriteU32(0x402F004C,0x50210A08); // SDRAMCR3, Refresh Burst len:5, Prescale:160cycles, Refresh Period:528cyc, Refresh Urgent Thres: 1280

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
#endif // #if 0 /* Done in DCD */

#if(DEBUG_SDRAM_CONFIG == 1)
    printf("\nSEMC Configuration");
    printf("\n MCR:   %08X", MEM_ReadU32(0x402F0000));
    printf("\n IOCR:  %08X", MEM_ReadU32(0x402F0004));
    printf("\n BMCR0: %08X", MEM_ReadU32(0x402F0008));
    printf("\n BMCR1: %08X", MEM_ReadU32(0x402F000C));
    printf("\n BR0:   %08X", MEM_ReadU32(0x402F0010));
    printf("\n BR1:   %08X", MEM_ReadU32(0x402F0014));
    printf("\n BR2:   %08X", MEM_ReadU32(0x402F0018));
    printf("\n BR3:   %08X", MEM_ReadU32(0x402F001C));
    printf("\n BR4:   %08X", MEM_ReadU32(0x402F0020));
    printf("\n BR5:   %08X", MEM_ReadU32(0x402F0024));
    printf("\n BR6:   %08X", MEM_ReadU32(0x402F0028));
    printf("\n BR7:   %08X", MEM_ReadU32(0x402F002C));
    printf("\n BR8:   %08X", MEM_ReadU32(0x402F0030));
    printf("\nSDRAM SEMC Configuration");
    printf("\n ClockFreq: %d", CLOCK_GetFreq(kCLOCK_SemcClk));
    printf("\n SDRAMCR0: %08X", MEM_ReadU32(0x402F0040));
    printf("\n SDRAMCR1: %08X", MEM_ReadU32(0x402F0044));
    printf("\n SDRAMCR2: %08X", MEM_ReadU32(0x402F0048));
    printf("\n SDRAMCR3: %08X", MEM_ReadU32(0x402F004C));
    printf("\n IPCR0: %08X", MEM_ReadU32(0x402F0090));
    printf("\n IPCR1: %08X", MEM_ReadU32(0x402F0094));
    printf("\n IPCR2: %08X", MEM_ReadU32(0x402F0098));
    printf("\n");
#endif
}

