/*
 * MXRT105x SLC NAND flash controller driver
 *
 * (C) Copyright 2018
 * Zimplistic Private Limited
 * Sicris Rey Embay <sicris@zimplistic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/errno.h>
#include "../../../board/freescale/mxrt105x-zb/boards/zb/fsl_nand.h"

#define MXRT105X_NAND_DEBUG                     0

struct mxrt105x_nand_info {
    uint8_t *data_buf;
    uint8_t *oob_buf;

    bool spare_only;
    bool status_request;
    uint16_t col_addr;
    uint32_t page_addr;
    uint8_t status;
};

static struct mxrt105x_nand_info _nand_info;
static uint8_t buf_main_oob[CONFIG_SYS_NAND_PAGE_SIZE + CONFIG_SYS_NAND_OOBSIZE];

/* NAND ECC Layout for small page NAND devices
 * Note: For large page devices, the default layouts are used. */
static struct nand_ecclayout mxrt105x_nand_oob_16 = {
	.eccbytes = 6,
	.eccpos = {10, 11, 12, 13, 14, 15},
	.oobfree = {
		{.offset = 0,
		 . length = 4},
		{.offset = 6,
		 . length = 4}
		}
};

static void mxrt105x_nand_init(void)
{
	NAND_Init();
}

static void mxrt105x_nand_command(struct mtd_info *mtd, unsigned command,
				int column, int page_addr)
{
	struct nand_chip *nand_chip = mtd_to_nand(mtd);
	struct mxrt105x_nand_info *nand_info = nand_get_controller_data(nand_chip);

#if(MXRT105X_NAND_DEBUG == 1)
	debug("mxrt105x_nand_command (cmd = 0x%x, col = 0x%x, page = 0x%x)\n",
	      command, column, page_addr);
#endif

	/* Reset command state information */
	nand_info->status_request = false;

	switch (command) {
	case NAND_CMD_RESET:
		NAND_Reset();
		break;

	case NAND_CMD_STATUS:
		nand_info->col_addr = 0;
		nand_info->status_request = true;
		nand_info->status = NAND_Read_Status();
		break;

	case NAND_CMD_READ0:
		nand_info->page_addr = page_addr;
		nand_info->col_addr = column;
		nand_info->spare_only = false;
		NAND_ReadPageDataOOB(page_addr, nand_info->data_buf);
		break;

	case NAND_CMD_READOOB:
		nand_info->page_addr = page_addr;
		nand_info->col_addr = column;
		nand_info->spare_only = true;
		NAND_ReadPageDataOOB(page_addr, nand_info->data_buf);
		break;

	case NAND_CMD_SEQIN:
//		if (column >= mtd->writesize) {
//			/*
//			 * before sending SEQIN command for partial write,
//			 * we need read one page out. FSL NFC does not support
//			 * partial write. It always sends out 512+ecc+512+ecc
//			 * for large page nand flash. But for small page nand
//			 * flash, it does support SPARE ONLY operation.
//			 */
//			if (host->pagesize_2k) {
//				/* call ourself to read a page */
//				mxc_nand_command(mtd, NAND_CMD_READ0, 0,
//						page_addr);
//			}
//
//			host->col_addr = column - mtd->writesize;
//			host->spare_only = true;
//
//			/* Set program pointer to spare region */
//			if (!host->pagesize_2k)
//				send_cmd(host, NAND_CMD_READOOB);
//		} else {
//			host->spare_only = false;
//			host->col_addr = column;
//
//			/* Set program pointer to page start */
//			if (!host->pagesize_2k)
//				send_cmd(host, NAND_CMD_READ0);
//		}
		break;

	case NAND_CMD_PAGEPROG:
//		send_prog_page(host, 0, host->spare_only);
//
//		if (host->pagesize_2k && is_mxc_nfc_1()) {
//			/* data in 4 areas */
//			send_prog_page(host, 1, host->spare_only);
//			send_prog_page(host, 2, host->spare_only);
//			send_prog_page(host, 3, host->spare_only);
//		}

		break;
	case NAND_CMD_READID:
		nand_info->col_addr = 0;
		NAND_ReadID(nand_info->data_buf);
		break;

	case NAND_CMD_ERASE1:
	case NAND_CMD_ERASE2:
		NAND_Erase(command, page_addr);
		break;
	}
#if 0
	/* Write out the command to the device. */
	send_cmd(host, command);

	/* Write out column address, if necessary */
	if (column != -1) {
		/*
		 * MXC NANDFC can only perform full page+spare or
		 * spare-only read/write. When the upper layers perform
		 * a read/write buffer operation, we will use the saved
		 * column address to index into the full page.
		 */
		send_addr(host, 0);
		if (host->pagesize_2k)
			/* another col addr cycle for 2k page */
			send_addr(host, 0);
	}

	/* Write out page address, if necessary */
	if (page_addr != -1) {
		u32 page_mask = nand_chip->pagemask;
		do {
			send_addr(host, page_addr & 0xFF);
			page_addr >>= 8;
			page_mask >>= 8;
		} while (page_mask);
	}

	/* Command post-processing step */
	switch (command) {

	case NAND_CMD_RESET:
		break;

	case NAND_CMD_READOOB:
	case NAND_CMD_READ0:
		if (host->pagesize_2k) {
			/* send read confirm command */
			send_cmd(host, NAND_CMD_READSTART);
			/* read for each AREA */
			send_read_page(host, 0, host->spare_only);
			if (is_mxc_nfc_1()) {
				send_read_page(host, 1, host->spare_only);
				send_read_page(host, 2, host->spare_only);
				send_read_page(host, 3, host->spare_only);
			}
		} else {
			send_read_page(host, 0, host->spare_only);
		}
		break;

	case NAND_CMD_READID:
		host->col_addr = 0;
		send_read_id(host);
		break;

	case NAND_CMD_PAGEPROG:
		break;

	case NAND_CMD_STATUS:
		break;

	case NAND_CMD_ERASE2:
		break;
	}
#endif
}

static void mxrt105x_nand_select_chip(struct mtd_info *mtd, int chipnr)
{
	/* ZB booard has only 1 nand chip */
	/// Empty
}

static int mxrt105x_nand_dev_ready(struct mtd_info *mtd)
{
	return(NAND_IsReady());
}

#if defined(CONFIG_DMA_MXRT105X)
static void mxrt105x_nand_dma_configure(struct nand_chip *chip,
				       const u8 *buffer, int size,
				       int read)
{
	/// TODO:
}

static void mxrt105x_nand_xfer(struct mtd_info *mtd, const u8 *buf,
			      int len, int read)
{
	/// TODO:
}

static u32 slc_ecc_copy_to_buffer(u8 *spare, const u32 *ecc, int count)
{
	/// TODO:
	return 0;
}

static int mxrt105x_ecc_calculate(struct mtd_info *mtd, const uint8_t *dat,
				 uint8_t *ecc_code)
{
	/// TODO:
	return 0;
}

/*
 * Enables and prepares SLC NAND controller
 * for doing data transfers with H/W ECC enabled.
 */
static void mxrt105x_hwecc_enable(struct mtd_info *mtd, int mode)
{
	/// TODO:
}

/*
 * mxrt105x_correct_data - [NAND Interface] Detect and correct bit error(s)
 * mtd:	MTD block structure
 * dat:	raw data read from the chip
 * read_ecc:	ECC from the chip
 * calc_ecc:	the ECC calculated from raw data
 *
 * Detect and correct a 1 bit error for 256 byte block
 */
int mxrt105x_correct_data(struct mtd_info *mtd, u_char *dat,
			 u_char *read_ecc, u_char *calc_ecc)
{
	/// TODO
	return 0;
}
#endif

#if defined(CONFIG_DMA_MXRT105X)
static void mxrt105x_dma_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	/// TODO:
}
#else
static void mxrt105x_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct nand_chip *nand_chip = mtd_to_nand(mtd);
	struct mxrt105x_nand_info *nand_info = nand_get_controller_data(nand_chip);
	uint32_t col = 0;
	uint8_t *p;

#if(MXRT105X_NAND_DEBUG == 1)
	debug("mxc_nand_read_buf(col = %d, len = %d)\n", nand_info->col_addr, len);
#endif
	col = nand_info->col_addr;

	if(nand_info->spare_only) {
		p = nand_info->oob_buf;
	} else {
		p = nand_info->data_buf;
	}

	while(len > 0) {
		*buf = p[col];
		buf++;
		col++;
		len--;
	}

	/* Update saved column address */
	nand_info->col_addr = col;
}
#endif

static uint8_t mxrt105x_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *nand_chip = mtd_to_nand(mtd);
	struct mxrt105x_nand_info *nand_info = nand_get_controller_data(nand_chip);
	uint8_t ret = 0;

	/* Check for status request */
    if (nand_info->status_request) {
        return(nand_info->status);
	}

	/* If we are accessing the spare region */
	if (nand_info->spare_only) {
		ret = nand_info->oob_buf[nand_info->col_addr];
	} else {
        ret = nand_info->data_buf[nand_info->col_addr];
	}

	/* Update saved column address */
	nand_info->col_addr++;

	return ret;
}

#if defined(CONFIG_DMA_MXRT105X)
static void mxrt105x_dma_write_buf(struct mtd_info *mtd, const uint8_t *buf,
				  int len)
{
	/// TODO:
}
#else
static void mxrt105x_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	/// TODO:
}
#endif

static void mxrt105x_write_byte(struct mtd_info *mtd, uint8_t byte)
{
	/// TODO:
}

#if defined(CONFIG_DMA_MXRT105X)
/* Reuse the logic from "nand_read_page_hwecc()" */
static int mxrt105x_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
				uint8_t *buf, int oob_required, int page)
{
	/// TODO:
	return 0;
}

/* Reuse the logic from "nand_write_page_hwecc()" */
static int mxrt105x_write_page_hwecc(struct mtd_info *mtd,
				    struct nand_chip *chip,
				    const uint8_t *buf, int oob_required,
				    int page)
{
	/// TODO:
	return 0;
}
#endif

/*
 * MXRT105X has only one SLC NAND controller, don't utilize
 * CONFIG_SYS_NAND_SELF_INIT to be able to reuse this function
 * both in SPL NAND and U-Boot images.
 */
int board_nand_init(struct nand_chip *mxrt105x_chip)
{
#if defined(CONFIG_DMA_MXRT105X)
	int ret;

	/* Acquire a channel for our use */
	ret = mxrt105x_dma_get_channel();
	if (unlikely(ret < 0)) {
		pr_info("Unable to get free DMA channel for NAND transfers\n");
		return -1;
	}
	dmachan = (unsigned int)ret;
#endif
	memset(&_nand_info, 0, sizeof(struct mxrt105x_nand_info));
	/* Initialize buffers */
	memset(buf_main_oob, 0, CONFIG_SYS_NAND_PAGE_SIZE + CONFIG_SYS_NAND_OOBSIZE);
	_nand_info.data_buf = &(buf_main_oob[0]);
	_nand_info.oob_buf = &(buf_main_oob[CONFIG_SYS_NAND_PAGE_SIZE]);

	nand_set_controller_data(mxrt105x_chip, &_nand_info);

	mxrt105x_chip->cmdfunc   = mxrt105x_nand_command;
	mxrt105x_chip->dev_ready = mxrt105x_nand_dev_ready;
	mxrt105x_chip->select_chip = mxrt105x_nand_select_chip;

	/*
	 * The implementation of these functions is quite common, but
	 * they MUST be defined, because access to data register
	 * is strictly 32-bit aligned.
	 */
	mxrt105x_chip->read_byte  = mxrt105x_read_byte;
	mxrt105x_chip->write_byte = mxrt105x_write_byte;

#if defined(CONFIG_DMA_MXRT105X)
	/* Hardware ECC calculation is supported when DMA driver is selected */
	mxrt105x_chip->ecc.mode		= NAND_ECC_HW;

	mxrt105x_chip->read_buf		= mxrt105x_dma_read_buf;
	mxrt105x_chip->write_buf		= mxrt105x_dma_write_buf;

	mxrt105x_chip->ecc.calculate	= mxrt105x_ecc_calculate;
	mxrt105x_chip->ecc.correct	= mxrt105x_correct_data;
	mxrt105x_chip->ecc.hwctl		= mxrt105x_hwecc_enable;
	mxrt105x_chip->chip_delay	= 2000;

	mxrt105x_chip->ecc.read_page	= mxrt105x_read_page_hwecc;
	mxrt105x_chip->ecc.write_page	= mxrt105x_write_page_hwecc;
	mxrt105x_chip->options		|= NAND_NO_SUBPAGE_WRITE;
#else
	/*
	 * Hardware ECC calculation is not supported by the driver,
	 * because it requires DMA support
	 */
	mxrt105x_chip->ecc.mode = NAND_ECC_SOFT;

	/*
	 * The implementation of these functions is quite common, but
	 * they MUST be defined, because access to data register
	 * is strictly 32-bit aligned.
	 */
	mxrt105x_chip->read_buf   = mxrt105x_read_buf;
	mxrt105x_chip->write_buf  = mxrt105x_write_buf;
#endif

	/*
	 * These values are predefined
	 * for both small and large page NAND flash devices.
	 */
	mxrt105x_chip->ecc.size     = CONFIG_SYS_NAND_ECCSIZE;
	mxrt105x_chip->ecc.bytes    = CONFIG_SYS_NAND_ECCBYTES;
	mxrt105x_chip->ecc.strength = 1;

	if (CONFIG_SYS_NAND_PAGE_SIZE != NAND_LARGE_BLOCK_PAGE_SIZE)
		mxrt105x_chip->ecc.layout = &mxrt105x_nand_oob_16;

#if defined(CONFIG_SYS_NAND_USE_FLASH_BBT)
	mxrt105x_chip->bbt_options |= NAND_BBT_USE_FLASH;
#endif

	/* Initialize NAND interface */
	mxrt105x_nand_init();

	return 0;
}
