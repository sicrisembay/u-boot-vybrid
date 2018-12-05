#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_WriteU32(uint32_t addr, uint32_t val);
uint32_t BOARD_ReadU32(uint32_t addr);
void BOARD_InitPins(void);
void BOARD_SetLedState(uint8_t state);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
