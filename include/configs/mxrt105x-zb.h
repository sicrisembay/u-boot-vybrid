/*
 * (C) Copyright 2017 Emcraft Systems
 * Dmitry Konyshev, <probables@emcraft.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#if 0
#define DEBUG
#endif

#define CONFIG_USE_STDINT

#if 0
#define CONFIG_SYS_ICACHE_OFF
#define CONFIG_SYS_DCACHE_OFF
#endif

#define CONFIG_SYS_ARCH_TIMER

#define CONFIG_SYS_TEXT_BASE		0x00001000

/*
 * To get Image data right at the 'Load Address' (0x80008000), and thus avoid
 * additional uImage relocation:
 * - 0x80007fc0 reserve place for uImage header; two separate image/dtb files
 * - 0x80007fb4 reserve place for 2-files multi-image header; one image+dtb file
 *
 * Note, that unaligned address can't be used when load from FAT as this results
 * to the slow copy path (and 'FAT: Misaligned buffer address') in fs/fat/fat.c.
 */
#define CONFIG_SYS_LOAD_ADDR		0x80007fc0
#define CONFIG_LOADADDR			0x80007fc0

#define PHYS_SDRAM                  0x80000000
#define PHYS_SDRAM_SIZE_MB          (16)
#define PHYS_SDRAM_SIZE_KB          (PHYS_SDRAM_SIZE_MB * 1024)
#define PHYS_SDRAM_SIZE             (PHYS_SDRAM_SIZE_KB * 1024)

#define DMAMEM_SZ_ALL			(1 * 1024 * 1024)
#define DMAMEM_BASE			(PHYS_SDRAM + PHYS_SDRAM_SIZE - \
					 DMAMEM_SZ_ALL)

#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	PHYS_SDRAM
#define CONFIG_SYS_MEMTEST_END      	(CONFIG_SYS_MEMTEST_START + PHYS_SDRAM_SIZE - (1024 * 1024))

#define CONFIG_SYS_INIT_SP_ADDR		(0x00000000 + 384 * 1024) /* points to end of OCRAM */

#define CONFIG_BOUNCE_BUFFER
/* #define CONFIG_FSL_ESDHC */
#define CONFIG_FSL_USDHC

#define CONFIG_SYS_FSL_ESDHC_ADDR	0x402c0000
#define CONFIG_SYS_FSL_USDHC_NUM        1

#define CONFIG_SYS_FSL_ERRATUM_ESDHC135 1
#define ESDHCI_QUIRK_BROKEN_TIMEOUT_VALUE

/* UART */
#define LPUART_BASE			LPUART1_RBASE

/* LCD */
#ifdef CONFIG_VIDEO
#define CONFIG_FB_ADDR			DMAMEM_BASE
#define MXS_LCDIF_BASE			0x402B8000
#define CONFIG_VIDEO_MXS
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_CMD_BMP
#define CONFIG_BMP_16BPP
#endif

/*
 * Configuration of the external SDRAM memory
 */
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM

#define CONFIG_SYS_MAX_FLASH_SECT	8
#define CONFIG_SYS_MAX_FLASH_BANKS	1

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_ENV_SIZE			(8 << 10)

#define CONFIG_SYS_CLK_FREQ		600000000 /* 600 MHz */

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG

#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE \
					+ sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_MALLOC_LEN		(4 * 1024 * 1024)

#define CONFIG_SYS_MALLOC_F
#define CONFIG_SYS_MALLOC_F_LEN		(32 * 1024)

#define CONFIG_BOOTARGS							\
	"console=ttyLP0,115200 consoleblank=0 ignore_loglevel "


#define CONFIG_BOOTCOMMAND						\
	"run bootcond"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"ubootFirstRun=true\0" \
	"initUbiVol=echo ===== mount rootfs partition =====;" \
	            "ubi part rootfs;" \
	            "echo ===== creating ubi volumes =====;" \
	            "if ubi check config1;" \
	                "then ubi remove config1;" \
                "fi;" \
                "if ubi check config2;" \
                    "then ubi remove config2;" \
                "fi;" \
                "if ubi check fs;" \
                    "then ubi remove fs;" \
				"fi;" \
				"ubi create config1 1048576;" \
	            "ubi create config2 1048576;" \
	            "ubi create fs;" \
	            "setenv ubootFirstRun false;" \
				"saveenv\0" \
	"bootfile=zpl_script.img\0" \
	"bootcond=if $ubootFirstRun;" \
	          "then echo ===== u-boot first run =====;" \
	               "env default -a;" \
	               "run initUbiVol;" \
	          "fi;" \
              "usb reset;" \
	          "if fatload usb 0:auto $loadaddr $bootfile;" \
	              "then echo $bootfile loaded at 0x$loadaddr;" \
	                  "if imi $loadaddr;" \
                          "then source $loadaddr;" \
                          "else echo Invalid Script File Header!;" \
                      "fi;" \
                  "else echo $bootfile not Found!;" \
	          "fi;" \
	          "usb stop;" \
	          "echo ===== Running Application =====;" \
	          "run nandboot\0" \
    "appfile=/firmware/project_tres.bin\0" \
	"apploadaddr=0x00000000\0" \
	"nandboot=ubifsmount ubi0:fs && " \
			  "ubifsload $apploadaddr $appfile && " \
			  "go $apploadaddr\0"

/*
 * Command line configuration.
 */
#define CONFIG_SYS_LONGHELP
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_CMD_CACHE
#define CONFIG_BOARD_LATE_INIT

/*#define CONFIG_CMD_FUSE
  #define CONFIG_MXC_OCOTP*/

/* For SPL */
#ifdef CONFIG_SUPPORT_SPL
#define CONFIG_SPL_STACK		CONFIG_SYS_INIT_SP_ADDR
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_TEXT_BASE		CONFIG_SYS_FLASH_BASE
#define CONFIG_SYS_MONITOR_LEN		(512 * 1024)
#define CONFIG_SYS_SPL_LEN		0x00008000
#define CONFIG_SYS_UBOOT_START		0x080083FD
#define CONFIG_SYS_UBOOT_BASE		(CONFIG_SYS_FLASH_BASE + \
					 CONFIG_SYS_SPL_LEN)

/* DT blob (fdt) address */
#define CONFIG_SYS_FDT_BASE		(CONFIG_SYS_FLASH_BASE + \
					0x1C0000)
#endif
/* For SPL ends */

/* NAND stuff */
#define CONFIG_SYS_MAX_NAND_DEVICE             1
#define CONFIG_SYS_NAND_BASE                   0x00000000
/* #define CONFIG_SYS_NAND_5_ADDR_CYCLE */
#define CONFIG_SYS_NAND_PAGE_COUNT	           64
/* number of main bytes in NAND page */
#define CONFIG_SYS_NAND_PAGE_SIZE	           2048
/* number of OOB bytes in NAND page */
#define CONFIG_SYS_NAND_OOBSIZE		           64
/* number of bytes in NAND erase-block */
#define CONFIG_SYS_NAND_BLOCK_SIZE	           (CONFIG_SYS_NAND_PAGE_COUNT * CONFIG_SYS_NAND_PAGE_SIZE)
/* data bytes per ECC step */
#define CONFIG_SYS_NAND_ECCSIZE		           512
/* ECC bytes per step */
#define CONFIG_SYS_NAND_ECCBYTES	           5

/* MTD partition */
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE	                   /* needed for mtdparts commands */
#define MTDIDS_DEFAULT                         "nand0=gpmi-nand"
#define MTDPARTS_DEFAULT                       "mtdparts=gpmi-nand:" \
                                               "512k(bcb),"          \
                                               "2m(u-boot1)ro,"      \
                                               "2m(u-boot1)ro,"      \
                                               "-(rootfs)"
#define CONFIG_ENV_UBI_PART                    "rootfs"
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_ENV_UBI_VOLUME                  "config1"
#define CONFIG_ENV_UBI_VOLUME_REDUND           "config2"
/* environment organization */
#define CONFIG_ENV_SECT_SIZE                   (128 * 1024)
#define CONFIG_ENV_OFFSET                      (28 * CONFIG_ENV_SECT_SIZE)

/* USB */
#ifdef CONFIG_CMD_USB
#define CONFIG_EHCI_IS_TDI
#endif /* #ifdef CONFIG_CMD_USB */

#undef CONFIG_CMD_IMLS

#endif /* __CONFIG_H */
