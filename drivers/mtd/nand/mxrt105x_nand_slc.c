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

struct mxrt105x_nand_slc_regs {
	u32 data;
	u32 addr;
	u32 cmd;
	u32 stop;
	u32 ctrl;
	u32 cfg;
	u32 stat;
	u32 int_stat;
	u32 ien;
	u32 isr;
	u32 icr;
	u32 tac;
	u32 tc;
	u32 ecc;
	u32 dma_data;
};

/* CFG register */
#define CFG_CE_LOW		(1 << 5)
#define CFG_DMA_ECC		(1 << 4) /* Enable DMA ECC bit */
#define CFG_ECC_EN		(1 << 3) /* ECC enable bit */
#define CFG_DMA_BURST		(1 << 2) /* DMA burst bit */
#define CFG_DMA_DIR		(1 << 1) /* DMA write(0)/read(1) bit */

/* CTRL register */
#define CTRL_SW_RESET		(1 << 2)
#define CTRL_ECC_CLEAR		(1 << 1) /* Reset ECC bit */
#define CTRL_DMA_START		(1 << 0) /* Start DMA channel bit */

/* STAT register */
#define STAT_DMA_FIFO		(1 << 2) /* DMA FIFO has data bit */
#define STAT_NAND_READY		(1 << 0)

/* INT_STAT register */
#define INT_STAT_TC		(1 << 1)
#define INT_STAT_RDY		(1 << 0)

/* TAC register bits, be aware of overflows */
#define TAC_W_RDY(n)		(max_t(uint32_t, (n), 0xF) << 28)
#define TAC_W_WIDTH(n)		(max_t(uint32_t, (n), 0xF) << 24)
#define TAC_W_HOLD(n)		(max_t(uint32_t, (n), 0xF) << 20)
#define TAC_W_SETUP(n)		(max_t(uint32_t, (n), 0xF) << 16)
#define TAC_R_RDY(n)		(max_t(uint32_t, (n), 0xF) << 12)
#define TAC_R_WIDTH(n)		(max_t(uint32_t, (n), 0xF) << 8)
#define TAC_R_HOLD(n)		(max_t(uint32_t, (n), 0xF) << 4)
#define TAC_R_SETUP(n)		(max_t(uint32_t, (n), 0xF) << 0)

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
	/// TODO:
}

static void mxrt105x_nand_cmd_ctrl(struct mtd_info *mtd,
				  int cmd, unsigned int ctrl)
{
	/// TODO:
}

static int mxrt105x_nand_dev_ready(struct mtd_info *mtd)
{
	/// TODO:
	return 0;
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
	/// TODO:
}
#endif

static uint8_t mxrt105x_read_byte(struct mtd_info *mtd)
{
	/// TODO:
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

	mxrt105x_chip->cmd_ctrl  = mxrt105x_nand_cmd_ctrl;
	mxrt105x_chip->dev_ready = mxrt105x_nand_dev_ready;

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
