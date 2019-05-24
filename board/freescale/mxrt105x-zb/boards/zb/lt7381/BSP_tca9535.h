/*! \file BSP_tca9535.h
*
* Module Description:
* -------------------
* This is the board support package interface for TCA9535 IO Expander
*
* $Author: sicris $
* $Date: 2019-01-23 11:58:25 +0800 (Wed, 23 Jan 2019) $
* $Revision: 555 $
*
*/

#ifndef BSP_TCA9535_H
#define BSP_TCA9535_H

//*****************************************************************************
// File dependencies.
//*****************************************************************************
#include "stdint.h"
#if 0
#include "stdbool.h"
#include "BSP_config.h"
#endif
//*****************************************************************************
// Public / Internal definitions.
//*****************************************************************************

//! \ enum BSP_TCA9535_RET_T
typedef enum {
    BSP_TCA9535_NOERROR = 0,            //!< Value: 0, No Error
	BSP_TCA9535_I2C_ERROR,

    N_BSP_TCA9535_RET
} BSP_TCA9535_RET_T;

//! \enum TCA9535_PIN_ID_T
typedef enum {
    TCA9535_P00 = 0,
    TCA9535_P01,
    TCA9535_P02,
    TCA9535_P03,
    TCA9535_P04,
    TCA9535_P05,
    TCA9535_P06,
    TCA9535_P07,
    TCA9535_P10,
    TCA9535_P11,
    TCA9535_P12,
    TCA9535_P13,
    TCA9535_P14,
    TCA9535_P15,
    TCA9535_P16,
    TCA9535_P17,

    N_TCA9535_PIN
} TCA9535_PIN_ID_T;

//*****************************************************************************
// Public function prototypes.
//*****************************************************************************
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/
//*****************************************************************************
//!
//! \brief This function initializes the TCA9535.
//!
//! \param  None
//!
//! \return \c ::BSP_TCA9535_RET_T
//!
//*****************************************************************************
extern BSP_TCA9535_RET_T BSP_TCA9535_Init(void);

//*****************************************************************************
//!
//! \brief This function sets the pin state of TCA9535
//!
//! \param  id      Pin ID (::TCA9535_PIN_ID_T)
//! \param  state   Pin State (true is high, false is low)
//!
//! \return \c ::BSP_TCA9535_RET_T
//!
//*****************************************************************************
extern BSP_TCA9535_RET_T BSP_TCA9535_SetPinState(TCA9535_PIN_ID_T id, uint8_t state);
#if 0
//*****************************************************************************
//!
//! \brief This function returns the pin state of TCA9535
//!
//! \param  id      Pin ID (::TCA9535_PIN_ID_T)
//!
//! \return Pin State
//!
//*****************************************************************************
extern uint8_t BSP_TCA9535_GetPinState(TCA9535_PIN_ID_T id);
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif // End BSP_TCA9535_H
