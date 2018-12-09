/*
 * Copyright (c) 2018
 * Zimplistic Private Limited.
 * Sicris Rey Embay <sicris@zimplistic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _FSL_NAND_H_
#define _FSL_NAND_H_

#include <stdbool.h>
#include <stdint.h>

void NAND_Init(void);
bool NAND_IsReady(void);
void NAND_ProcessCommand(uint8_t * cmd_buf, uint32_t cmd_queue_len);
void NAND_Read_Buf(uint8_t *buf, int len);
uint8_t NAND_Read_Status(void);
void NAND_Reset(void);
void NAND_ReadID(uint8_t *buf);

#endif // #ifndef _FSL_NAND_H_

