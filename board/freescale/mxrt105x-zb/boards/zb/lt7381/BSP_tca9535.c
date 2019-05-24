//*****************************************************************************
//!
//! \file BSP_tca9535.c
//!
//! \brief This is the implementation of TCA9535 Board Support Package.
//!
//! $Author: sicris $
//! $Date: 2019-01-23 11:58:25 +0800 (Wed, 23 Jan 2019) $
//! $Revision: 555 $
//!
//*****************************************************************************

//*****************************************************************************
// File dependencies.
//*****************************************************************************
#include "BSP_tca9535.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_lpi2c.h"

#define TCA9535_INT_MUX                         IOMUXC_GPIO_AD_B0_00_GPIO1_IO00
#define TCA9535_INT_GPIO                        GPIO1
#define TCA9535_INT_GPIO_PIN                    0U

#define TCA9535_I2C_DEV_ADDR                    (0x27)

#define TCA9535_REG_INPUT_PORT0                 (0x00)
#define TCA9535_REG_INPUT_PORT1                 (0x01)
#define TCA9535_REG_OUTPUT_PORT0                (0x02)
#define TCA9535_REG_OUTPUT_PORT1                (0x03)
#define TCA9535_REG_POL_INV_PORT0               (0x04)
#define TCA9535_REG_POL_INV_PORT1               (0x05)
#define TCA9535_REG_CONFIG_PORT0                (0x06)
#define TCA9535_REG_CONFIG_PORT1                (0x07)

#define TCA9535_OUTPUT_PIN                      (0)
#define TCA9535_INPUT_PIN                       (1)

static uint16_t const PIN_CONFIG_DEFAULT[N_TCA9535_PIN] = {
        TCA9535_OUTPUT_PIN,         /* LED_KN -->   */
        TCA9535_INPUT_PIN,          /* <-- HALL_GR1 */
        TCA9535_INPUT_PIN,          /* <-- HALL_GR2 */
        TCA9535_OUTPUT_PIN,         /* FAN1 -->     */
        TCA9535_OUTPUT_PIN,         /* FAN2 -->     */
        TCA9535_OUTPUT_PIN,         /* FAN3 -->     */
        TCA9535_OUTPUT_PIN,         /* CASS_OUT --> */
        TCA9535_OUTPUT_PIN,         /* GHEE_OUT --> */
        TCA9535_OUTPUT_PIN,         /* LCD_BL -->   */
        TCA9535_OUTPUT_PIN,         /* SENSOR_SD--> */
        TCA9535_INPUT_PIN,          /* <-- ROTI DOOR*/
        TCA9535_INPUT_PIN,          /* <-- FR DOOR  */
        TCA9535_INPUT_PIN,          /* <-- HALL_KR3 */
        TCA9535_INPUT_PIN,          /* <-- HALL_KR2 */
        TCA9535_INPUT_PIN,          /* <-- HALL_KR1 */
        TCA9535_INPUT_PIN           /* <-- HALL_GR3 */
};
#if 0
#define TCA9535_ISR_BIT                         DIG_IN_ISR_NOTIFY_BIT
#define TCA9535_UPDATE_LATCH_BIT                (0x02)
#endif
typedef union {
    uint16_t all;
    struct {
        uint8_t byte0;
        uint8_t byte1;
    } byteField;
} REG16;
#if 0
//*****************************************************************************
// Private member declarations.
//*****************************************************************************
static bool bInit = false;
static bool bReady = false;

/* TCA9535 Task */
static StaticTask_t xTaskTCA9535;
static StackType_t xTaskStackTCA9535[configTASK_STACK_TCA9535];
static TaskHandle_t xTaskHandleTCA9535 = NULL;
/* Semaphore for I2C transaction Signal */
static SemaphoreHandle_t semHdli2cDone = NULL;
static StaticSemaphore_t semBuffi2cDone;
#endif
static REG16 pinState;
static REG16 outLatchState;
#if 0
//*****************************************************************************
// Public / Internal member external declarations.
//*****************************************************************************

//*****************************************************************************
// Private function prototypes.
//*****************************************************************************
static void _tca9535_Task(void *pxParam);

//*****************************************************************************
// Public function implementations
//*****************************************************************************
#endif

#define I2C_BASE            	   ((LPI2C_Type *) LPI2C1_BASE)
#define LPI2C_CLOCK_SOURCE_SELECT  (0U)	/* Select USB1 PLL */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
#define LPI2C_CLOCK_FREQUENCY      ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
#define I2C_SCL_MUX        		   IOMUXC_GPIO_SD_B1_04_LPI2C1_SCL
#define I2C_SDA_MUX        	       IOMUXC_GPIO_SD_B1_05_LPI2C1_SDA

static void _TCA9535_I2C_Init(void)
{
	lpi2c_master_config_t masterConfig;

	CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
	CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);

	CLOCK_EnableClock(kCLOCK_Iomuxc);

	/* Configure I2C Pin routing and pad */
	IOMUXC_SetPinMux(I2C_SCL_MUX, 1U);
	IOMUXC_SetPinMux(I2C_SDA_MUX, 1U);
	IOMUXC_SetPinConfig(I2C_SCL_MUX, 0x08B0u);
	IOMUXC_SetPinConfig(I2C_SDA_MUX, 0x08B0u);

	LPI2C_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate_Hz = 100000U;
	LPI2C_MasterInit(I2C_BASE, &masterConfig, LPI2C_CLOCK_FREQUENCY);
}
//*****************************************************************************
//!
//! \brief This function initializes the TCA9535.
//!
//! \param  None
//!
//! \return \c ::BSP_TCA9535_RET_T
//!
//*****************************************************************************
#define TCA9535_INT_MUX                         IOMUXC_GPIO_AD_B0_00_GPIO1_IO00
#define TCA9535_INT_GPIO                        GPIO1
#define TCA9535_INT_GPIO_PIN                    0U

static status_t _I2C_WriteRequest(uint16_t devAddr,
							  uint32_t regAddr,
							  uint32_t regAddrWidth,
							  uint8_t *pData,
							  uint32_t nData)
{
	status_t retval = kStatus_Success;
	uint32_t flags = 0U;

	/* Master Start and Send Address */
	retval = LPI2C_MasterStart(I2C_BASE, TCA9535_I2C_DEV_ADDR, kLPI2C_Write);
	if(retval != kStatus_Success) {
		return(retval);
	}
	do {
		flags = LPI2C_MasterGetStatusFlags(I2C_BASE);
	} while((flags & kLPI2C_MasterTxReadyFlag) == 0);

	/* Send Register Address */
	retval = LPI2C_MasterSend(I2C_BASE, (void *)&regAddr, regAddrWidth);
	if(retval != kStatus_Success) {
		goto i2c_tx_out;
	}
	do {
		flags = LPI2C_MasterGetStatusFlags(I2C_BASE);
	} while((flags & kLPI2C_MasterTxReadyFlag) == 0);

	/* Send Data */
	retval = LPI2C_MasterSend(I2C_BASE, (void *)pData, nData);
	if(retval != kStatus_Success) {
		goto i2c_tx_out;
	}
	do {
		flags = LPI2C_MasterGetStatusFlags(I2C_BASE);
	} while((flags & kLPI2C_MasterTxReadyFlag) == 0);

i2c_tx_out:
	/* Master Send Stop Command */
	retval = LPI2C_MasterStop(I2C_BASE);

	return(retval);
}

static status_t _I2C_ReadRequest(
										uint16_t devAddr,
		                                uint32_t regAddr,
		                                size_t regAddrWidth,
		                                uint8_t* pData,
		                                size_t nData)
{
	status_t retval = kStatus_Success;
	uint32_t flags = 0U;

	/* Master Start and Send Address */
	retval = LPI2C_MasterStart(I2C_BASE, TCA9535_I2C_DEV_ADDR, kLPI2C_Write);
	if(retval != kStatus_Success) {
		return(retval);
	}
	do {
		flags = LPI2C_MasterGetStatusFlags(I2C_BASE);
	} while((flags & kLPI2C_MasterTxReadyFlag) == 0);

	/* Send Register Address */
	retval = LPI2C_MasterSend(I2C_BASE, (void *)&regAddr, regAddrWidth);
	if(retval != kStatus_Success) {
		goto i2c_rx_out;
	}
	do {
		flags = LPI2C_MasterGetStatusFlags(I2C_BASE);
	} while((flags & kLPI2C_MasterTxReadyFlag) == 0);

	/* Restart and Read */
	retval = LPI2C_MasterStart(I2C_BASE, TCA9535_I2C_DEV_ADDR, kLPI2C_Read);
	if(retval != kStatus_Success) {
		goto i2c_rx_out;
	}
	do {
		flags = LPI2C_MasterGetStatusFlags(I2C_BASE);
	} while((flags & kLPI2C_MasterTxReadyFlag) == 0);

	/* Read Data */
	retval = LPI2C_MasterReceive(I2C_BASE, (void *)pData, nData);

i2c_rx_out:
	/* Master Send Stop Command */
	LPI2C_MasterStop(I2C_BASE);

	return(retval);
}

BSP_TCA9535_RET_T BSP_TCA9535_Init(void)
{
    BSP_TCA9535_RET_T retval = BSP_TCA9535_I2C_ERROR;
    gpio_pin_config_t gpioConfigINT;
    uint32_t idx = 0;
	REG16 tca9535_reg;

    _TCA9535_I2C_Init();

    /* Configure TCA9535 INT */
	CLOCK_EnableClock(kCLOCK_Iomuxc);
	IOMUXC_SetPinMux(TCA9535_INT_MUX, 0U);
	IOMUXC_SetPinConfig(TCA9535_INT_MUX, 0xF8B0u);

	/* GPIO configuration */
	gpioConfigINT.direction = kGPIO_DigitalInput;
	gpioConfigINT.outputLogic = 1U;
	gpioConfigINT.interruptMode = kGPIO_NoIntmode;
	GPIO_PinInit(TCA9535_INT_GPIO, TCA9535_INT_GPIO_PIN, &gpioConfigINT);

    /* Write TCA9535 Pin Configuration */
    tca9535_reg.all = 0;
    for(idx = 0; idx < N_TCA9535_PIN; idx++) {
        tca9535_reg.all |= (uint16_t)(PIN_CONFIG_DEFAULT[idx] << idx);
    }

  	if(kStatus_Success != _I2C_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_CONFIG_PORT0, 1, (uint8_t*)&(tca9535_reg.byteField.byte0), 1)) return(retval);
  	if(kStatus_Success != _I2C_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_CONFIG_PORT1, 1, (uint8_t*)&(tca9535_reg.byteField.byte1), 1))  return(retval);

    /* Read Input Pin */
  	if(kStatus_Success != _I2C_ReadRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_INPUT_PORT0, 1, (uint8_t*)&(pinState.byteField.byte0), 1)) return(retval);
  	if(kStatus_Success != _I2C_ReadRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_INPUT_PORT1, 1, (uint8_t*)&(pinState.byteField.byte1), 1)) return(retval);

    /* Update Output Latch */
  	if(kStatus_Success != _I2C_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_OUTPUT_PORT0, 1, (uint8_t*)&(outLatchState.byteField.byte0), 1)) return(retval);
  	if(kStatus_Success != _I2C_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_OUTPUT_PORT1, 1, (uint8_t*)&(outLatchState.byteField.byte1), 1)) return(retval);

    /// Set Sensor Shutdown pin High (Temporary location)
    /// TODO: This should be done in the higher layer as this BSP level has no idea on pin map.
    outLatchState.all |= (1U << TCA9535_P11);
    /* Update Output Latch */
    if(kStatus_Success != _I2C_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_OUTPUT_PORT0, 1, (uint8_t*)&(outLatchState.byteField.byte0), 1)) return(retval);
    if(kStatus_Success != _I2C_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_OUTPUT_PORT1, 1, (uint8_t*)&(outLatchState.byteField.byte1), 1)) return(retval);

    retval = BSP_TCA9535_NOERROR;
    return (retval);
}

//*****************************************************************************
//!
//! \brief This function sets the pin state of TCA9535
//!
//! \param  id      Pin ID (::TCA9535_PIN_ID_T)
//! \param  state   Pin State (true is high, false is low)
//!
//! \return \c BSP_TCA9535_RET_T
//!
//*****************************************************************************
BSP_TCA9535_RET_T BSP_TCA9535_SetPinState(TCA9535_PIN_ID_T id, uint8_t state)
{
    BSP_TCA9535_RET_T retval = BSP_TCA9535_I2C_ERROR;

    if((id < N_TCA9535_PIN)) {
        if(state) {
            outLatchState.all |= (1U << id);
        } else {
            outLatchState.all &= ~(1U << id);
        }

        /* Update Output Latch */
        if(kStatus_Success != _I2C_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_OUTPUT_PORT0, 1, (uint8_t*)&(outLatchState.byteField.byte0), 1)) return(retval);
        if(kStatus_Success != _I2C_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_OUTPUT_PORT1, 1, (uint8_t*)&(outLatchState.byteField.byte1), 1)) return(retval);

        retval = BSP_TCA9535_NOERROR;
    }
    return (retval);
}

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
uint8_t BSP_TCA9535_GetPinState(TCA9535_PIN_ID_T id)
{
    uint8_t retval = 0;

    if((id < N_TCA9535_PIN) && (true == bReady)) {
        if(pinState.all & (1U << id)) {
            retval = 1;
        } else {
            retval = 0;
        }
    }
    return (retval);
}


//*****************************************************************************
// Private function implementations.
//*****************************************************************************

static void _tca9535_Task(void *pxParam)
{
    TickType_t xLastWakeTime;
    BaseType_t xResult;
    uint32_t idx = 0;
    uint32_t ulNotifiedValue = 0;
    REG16 tca9535_reg;


    xLastWakeTime = xTaskGetTickCount();

    while(!BSP_I2C_MANAGER_isReady()) {
        /* Wait until I2C manager is ready */
        vTaskDelayUntil(&xLastWakeTime, 1);
    }
    /* Write TCA9535 Pin Configuration */
    tca9535_reg.all = 0;
    for(idx = 0; idx < N_TCA9535_PIN; idx++) {
        tca9535_reg.all |= (uint16_t)(PIN_CONFIG_DEFAULT[idx] << idx);
    }
    BSP_I2C_MANAGER_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_CONFIG_PORT0, 1, (uint8_t*)&(tca9535_reg.byteField.byte0), 1, NULL);
    BSP_I2C_MANAGER_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_CONFIG_PORT1, 1, (uint8_t*)&(tca9535_reg.byteField.byte1), 1, semHdli2cDone);
    xSemaphoreTake(semHdli2cDone, portMAX_DELAY);

    /* Read Input Pin */
    BSP_I2C_MANAGER_ReadRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_INPUT_PORT0, 1, (uint8_t*)&(pinState.byteField.byte0), 1, NULL);
    BSP_I2C_MANAGER_ReadRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_INPUT_PORT1, 1, (uint8_t*)&(pinState.byteField.byte1), 1, semHdli2cDone);
    xSemaphoreTake(semHdli2cDone, portMAX_DELAY);

    /* Update Output Latch */
    BSP_I2C_MANAGER_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_OUTPUT_PORT0, 1, (uint8_t*)&(outLatchState.byteField.byte0), 1, NULL);
    BSP_I2C_MANAGER_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_OUTPUT_PORT1, 1, (uint8_t*)&(outLatchState.byteField.byte1), 1, semHdli2cDone);
    xSemaphoreTake(semHdli2cDone, portMAX_DELAY);

    /* Register and Enable TCA9535 Interrupt */
    BSP_DIN_RegisterIntTask(TCA9535_INT_GPIO, TCA9535_INT_GPIO_PIN, xTaskHandleTCA9535);

    bReady = true;

    /// Set Sensor Shutdown pin High (Temporary location)
    /// TODO: This should be done in the higher layer as this BSP level has no idea on pin map.
    outLatchState.all |= (1U << TCA9535_P11);
    /* Update Output Latch */
    BSP_I2C_MANAGER_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_OUTPUT_PORT0, 1, (uint8_t*)&(outLatchState.byteField.byte0), 1, NULL);
    BSP_I2C_MANAGER_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_OUTPUT_PORT1, 1, (uint8_t*)&(outLatchState.byteField.byte1), 1, semHdli2cDone);
    xSemaphoreTake(semHdli2cDone, portMAX_DELAY);


    while(1) {
        /* wait for task notification */
        xResult = xTaskNotifyWait(pdFALSE, UINT32_MAX, &ulNotifiedValue, 100);
        if(pdTRUE == xResult) {
            /* Notification Received */
            if(ulNotifiedValue & TCA9535_ISR_BIT) {
                /* Input change interrupt */
                /* Read Input Pin */
                BSP_I2C_MANAGER_ReadRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_INPUT_PORT0, 1, (uint8_t*)&(pinState.byteField.byte0), 1, NULL);
                BSP_I2C_MANAGER_ReadRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_INPUT_PORT1, 1, (uint8_t*)&(pinState.byteField.byte1), 1, semHdli2cDone);
                xSemaphoreTake(semHdli2cDone, portMAX_DELAY);
#if(BSP_TEST_TCA9535_INT == 1)
                {
                    /* Use oscilloscope to verify the Sampling Rate */
                    static uint32_t dbgPinState = 0;
                    if(dbgPinState == 0) {
                        BSP_DOUT_SetState(DIG_OUT_DEBUG_LED, 1);
                        dbgPinState = 1;
                    } else {
                        BSP_DOUT_SetState(DIG_OUT_DEBUG_LED, 0);
                        dbgPinState = 0;
                    }
                }
#endif /* #if(BSP_TEST_TCA9535_INT == 1) */
            }

            if(ulNotifiedValue & TCA9535_UPDATE_LATCH_BIT) {
                /* Update Output Latch */
                BSP_I2C_MANAGER_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_OUTPUT_PORT0, 1, (uint8_t*)&(outLatchState.byteField.byte0), 1, NULL);
                BSP_I2C_MANAGER_WriteRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_OUTPUT_PORT1, 1, (uint8_t*)&(outLatchState.byteField.byte1), 1, semHdli2cDone);
                xSemaphoreTake(semHdli2cDone, portMAX_DELAY);
            }
        } else {
            /* Timeout! Update Input state anyway */
            BSP_I2C_MANAGER_ReadRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_INPUT_PORT0, 1, (uint8_t*)&(pinState.byteField.byte0), 1, NULL);
            BSP_I2C_MANAGER_ReadRequest(TCA9535_I2C_DEV_ADDR, TCA9535_REG_INPUT_PORT1, 1, (uint8_t*)&(pinState.byteField.byte1), 1, semHdli2cDone);
            xSemaphoreTake(semHdli2cDone, portMAX_DELAY);
        }
    }
}

#endif
