#
# (C) Copyright 2017 Zimplistic Private Limited
# Sicris Rey Embay <sicris@zimplistic.com>
#
# SPDX-License-Identifier:	GPL-2.0+
#

ccflags-y := -I$(CONFIG_BOARDDIR)/CMSIS/Include -I$(CONFIG_BOARDDIR)/CMSIS/Driver/Include \
	-I$(CONFIG_BOARDDIR)/devices/MIMXRT1052 -I$(CONFIG_BOARDDIR)/devices/MIMXRT1052/drivers \
	-I$(CONFIG_BOARDDIR)/boards/zb/lt7381 \
	-I$(CONFIG_BOARDDIR)/boards/zb/lt7381/levetop-tft/LT738 \
	-I$(CONFIG_BOARDDIR)/boards/zb/lt7381/levetop-tft/LT738_Lib \
	-Iinclude \
	-I$(CONFIG_BOARDDIR)/devices/MIMXRT1052/utilities \
	-DCPU_MIMXRT1052DVL6A -DPRINTF_FLOAT_ENABLE=0 -DSCANF_FLOAT_ENABLE=0 \
	-DPRINTF_ADVANCED_ENABLE=0 -DSCANF_ADVANCED_ENABLE=0 -DEVK_MCIMXRM
	
obj-$(CONFIG_MXRT105X_ZB_DISP_LT7381) += BSP_tftLT7381.o
obj-$(CONFIG_MXRT105X_ZB_DISP_LT7381) += BSP_tca9535.o
obj-$(CONFIG_MXRT105X_ZB_DISP_LT7381) += levetop-tft/LT738/LT738.o
obj-$(CONFIG_MXRT105X_ZB_DISP_LT7381) += levetop-tft/LT738_Lib/LT738_Lib.o
