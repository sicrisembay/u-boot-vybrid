/*
 * BSP_tftLT7381.h
 *
 *  Created on: 11 Feb 2019
 *      Author: Toan
 */

#ifndef BSP_BSP_TFTLT7381_H_
#define BSP_BSP_TFTLT7381_H_

//*****************************************************************************
// File dependencies.
//*****************************************************************************
#include "stdint.h"
#include "stdbool.h"
#if 0
#include "appConfig.h"
#include "BSP_config.h"
#include "lcd.h"
#endif
//*****************************************************************************
// Public / Internal definitions.
//*****************************************************************************

typedef enum {
    BSP_TFTLT7381_NOERROR = 0,      //!< Value: 0, No Error
    BSP_TFTLT7381_INIT_ERROR = 1,   //!< Value: 1, Init Error

    N_TFTLT7381_RET
} BSP_TFTLT7381_RET_T;

//*****************************************************************************
// Public function prototypes.
//*****************************************************************************
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

//*****************************************************************************
//!
//! \brief This function initializes the LT7381
//!
//! \param  None
//!
//! \return \c ::BSP_TFTLT7381_RET_T
//!
//*****************************************************************************
extern BSP_TFTLT7381_RET_T BSP_TFT_LT7381_Init(void);

#if 0
//*****************************************************************************
//!
//! \brief This function check if the LT7381 is ready or not
//!
//! \param  None
//!
//! \return \c true if ready
//!             otherwise, false
//!
//*****************************************************************************
extern BSP_TFTLT7381_RET_T BSP_TFT_LT7381_IsReady(void);
#endif
//*****************************************************************************
//!
//! \brief This function write Register's address to the LT7381
//!
//! \param  cmd     Register's address
//!
//! \return \c      None
//!
//*****************************************************************************
void LCD_CmdWrite(uint8_t cmd);

//*****************************************************************************
//!
//! \brief This function writes data to Register or memory
//!
//! \param  data     Data to write to Register or Memory
//!
//! \return \c      None
//!
//*****************************************************************************
void LCD_DataWrite(uint8_t data);

#if 0
//*****************************************************************************
//!
//! \brief This function writes pixels to the LCD RAM
//!
//! \param  data     Data to write to the LCD RAM
//!
//! \return \c      None
//!
//*****************************************************************************
void LCD_DataWrite_Pixel(uint16_t data);

//*****************************************************************************
//!
//! \brief This function write each line of the window to LCD RAM
//!
//! \param  data     Pointer to the buffer
//!         h        length of the buffer
//!
//! \return \c      None
//!
//*****************************************************************************
void LCD_DataWrite_Line(unsigned char *data, uint32_t h);
#endif
//*****************************************************************************
//!
//! \brief This function read status register
//!
//! \param  None
//!
//! \return \c      Status Register
//!
//*****************************************************************************
uint8_t LCD_StatusRead(void);

//*****************************************************************************
//!
//! \brief This function read Register's Data
//!
//! \param  None
//!
//! \return \c      Register's Data
//!
//*****************************************************************************
uint8_t LCD_DataRead(void);

void SPI_RST(char val);
void SPI_CS(char val);
void SPI_CLK(char val);
void SPI_SDA(char val);
void SPI_WriteComm(unsigned int i);
void SPI_WriteData(unsigned int i);

//*****************************************************************************
//!
//! \brief Delay function in us used by LT738 Library
//!
//! \param  time    Delayed time
//!
//! \return \c      None
//!
//*****************************************************************************
void Delay_us(uint16_t time);

//*****************************************************************************
//!
//! \brief Delay function in ms used by LT738 Library
//!
//! \param  time    Delayed time
//!
//! \return \c      None
//!
//*****************************************************************************
void Delay_ms(uint16_t time);
void LCD_Init(void);
void BSP_TFT_LT7381_Reset(uint8_t sts);
void BSP_TFT_LT7381_BL(uint8_t sts);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* BSP_BSP_TFTLT7381_H_ */
