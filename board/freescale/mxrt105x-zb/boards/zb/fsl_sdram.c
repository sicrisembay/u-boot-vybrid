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

#define DEBUG_SDRAM_CONFIG 1

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
	semc_sdram_config_t sdramConfig;
	uint32_t clockFreq = CLOCK_GetFreq(kCLOCK_SemcClk);

	// Configure SDRAM
#if 1
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
#else
	memset(&sdramConfig, 0, sizeof(semc_sdram_config_t));
	sdramConfig.csxPinMux = kSEMC_MUXCSX0;
	sdramConfig.address = PHYS_SDRAM;
	sdramConfig.memsize_kbytes = PHYS_SDRAM_SIZE_KB;
	sdramConfig.portSize = kSEMC_PortSize16Bit;
	sdramConfig.burstLen = kSEMC_BurstLen8;
	sdramConfig.columnAddrBitNum = kSEMC_9bit;
	sdramConfig.casLatency = kSEMC_LatencyThree;
	sdramConfig.precharge2Active_Ns = 23;
	sdramConfig.active2ReadWrtie_Ns = 23;
	sdramConfig.refreshRecovery_Ns = 75;
	sdramConfig.writeRecovery_Ns = 23;
	sdramConfig.ckeOff_Ns = 45;
	sdramConfig.active2Prechage_Ns = 53;
	sdramConfig.selfRefRecovery_Ns = 248;
	sdramConfig.refresh2Refresh_Ns = 75;
	sdramConfig.active2Active_Ns = 15;
	sdramConfig.prescalePeriod_Ns = 160 * (1000000000 / clockFreq);
	sdramConfig.idleTimeout_Ns = 0;
	sdramConfig.refreshPeriod_nsPerRow = 528 * (1000000/8192);
	sdramConfig.refreshUrgThreshold = sdramConfig.refreshPeriod_nsPerRow;
	sdramConfig.refreshBurstLen = 5;
	SEMC_ConfigureSDRAM(SEMC, kSEMC_SDRAM_CS0, &sdramConfig, clockFreq);
#endif
#if 0
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
    MEM_WriteU32(0x402F0040,0x00000F31); // SDRAMCR0, Bus Width:16, Burst Len:8, Col Addr:9, CAS Lat:3
    MEM_WriteU32(0x402F0044,0x00652922); // SDRAMCR1, PRE2ACT: 23ns, ACT2RW: 23ns, RFRC:75ns, WRC:23ns, CKEOFF:45ns, ACT2PRE:53ns
    MEM_WriteU32(0x402F0048,0x00010920); // SDRAMCR2, SRRC:248ns, REF2EF:75ns, ACT2ACT:15ns
    MEM_WriteU32(0x402F004C,0x50210A08); // SDRAMCR3, Refresh Burst len:5, Prescale:160cycles, Refresh Period:528cyc, Refresh Urgent Thres: 1280

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
#endif

#if(DEBUG_SDRAM_CONFIG == 1)
    debug("\nSEMC Configuration");
    debug("\n MCR:   %08X", MEM_ReadU32(0x402F0000));
    debug("\n IOCR:  %08X", MEM_ReadU32(0x402F0004));
    debug("\n BMCR0: %08X", MEM_ReadU32(0x402F0008));
    debug("\n BMCR1: %08X", MEM_ReadU32(0x402F000C));
    debug("\n BR0:   %08X", MEM_ReadU32(0x402F0010));
    debug("\n BR1:   %08X", MEM_ReadU32(0x402F0014));
    debug("\n BR2:   %08X", MEM_ReadU32(0x402F0018));
    debug("\n BR3:   %08X", MEM_ReadU32(0x402F001C));
    debug("\n BR4:   %08X", MEM_ReadU32(0x402F0020));
    debug("\n BR5:   %08X", MEM_ReadU32(0x402F0024));
    debug("\n BR6:   %08X", MEM_ReadU32(0x402F0028));
    debug("\n BR7:   %08X", MEM_ReadU32(0x402F002C));
    debug("\n BR8:   %08X", MEM_ReadU32(0x402F0030));
    debug("\nSDRAM SEMC Configuration");
    debug("\n ClockFreq: %d", clockFreq);
    debug("\n SDRAMCR0: %08X", MEM_ReadU32(0x402F0040));
    debug("\n SDRAMCR1: %08X", MEM_ReadU32(0x402F0044));
    debug("\n SDRAMCR2: %08X", MEM_ReadU32(0x402F0048));
    debug("\n SDRAMCR3: %08X", MEM_ReadU32(0x402F004C));
    debug("\n IPCR0: %08X", MEM_ReadU32(0x402F0090));
    debug("\n IPCR1: %08X", MEM_ReadU32(0x402F0094));
    debug("\n IPCR2: %08X", MEM_ReadU32(0x402F0098));
    debug("\n");
#endif
}

