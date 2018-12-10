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
void NAND_Read_Buf(uint8_t *buf, int len);
uint8_t NAND_Read_Status(void);
void NAND_Reset(void);
void NAND_ReadID(uint8_t *buf);
void NAND_ReadPageDataOOB(uint32_t pageAddress, uint8_t *buf);
void NAND_Erase(uint8_t command, int32_t page_addr);
void NAND_ProgramPage(int32_t page_addr, int32_t column, uint32_t len, uint8_t *buf);

#endif // #ifndef _FSL_NAND_H_

