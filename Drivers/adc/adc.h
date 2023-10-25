/**
 * @file adc.h
 * @brief Implementation of the ADC Module for Kinetis KL05 Family
 * @version 1.0
 * @date 24/05/2023
 * @author Matheus Leitzke Pinto
 *
 * This file contains the implementation of the ADC module for the Kinetis KL05 family of microcontrollers.
 * It provides functionality for configuring and utilizing the analog-to-digital conversion capabilities of the microcontroller.
 */

#ifndef ADC_DRV_H_
#define ADC_DRV_H_

#include <common.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @addtogroup adc driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Flags for channel status. */
typedef enum adcChannelStatusFlag
{
    ADC_CHANNEL_CONVERSION_DONE_FLAG = ADC_SC1_COCO_MASK, /*!< Conversion is complete. */
} adcChannelStatusFlag_t;

/*! @brief Flags for ADC status. */
typedef enum adcStatusFlag
{
    ADC_ACTIVE_FLAG = ADC_SC2_ADACT_MASK, /*!< ADC is active. */
    ADC_CALIBRATION_FAILED_FLAG = ADC_SC3_CALF_MASK, /*!< Calibration failed. */
} adcStatusFlag_t;

/*! @brief Divider of the ADC clock. */
typedef enum adcClockDivider
{
    ADC_CLOCK_DIV_1 = 0U, /* Divide the input clock to the module by 1. */
    ADC_CLOCK_DIV_2 = 1U, /* Divide the input clock to the module by 2. */
    ADC_CLOCK_DIV_4 = 2U, /* Divide the input clock to the module by 4. */
    ADC_CLOCK_DIV_8 = 3U, /* Divide the input clock to the module by 8. */
} adcClockDivider_t;

/*! @brief ADC resolution. */
typedef enum adcResolution
{
    /* Enumeration values for internal use related to register settings. */
    ADC_RESOLUTION_8_BIT = 0U, /*!< Single-Ended 8-bit. */
    ADC_RESOLUTION_12_BIT = 1U, /*!< Single-Ended 12-bit. */
    ADC_RESOLUTION_10_BIT = 2U, /*!< Single-Ended 10-bit. */

} adcResolution_t;

/*! @brief Source of the ADC clock. */
typedef enum adcClockSource
{
    ADC_BUS_CLOCK_SRC = 0U, /* Use the bus clock as the clock source. */
    ADC_BUS_CLOCK_DIV_2_SRC = 1U, /* Use bus clock divided by 2 as the clock source. */
    ADC_ALT_CLOCK_SRC = 2U, /* Use an alternate clock source (OSCERCLK). */
    ADC_ASYNC_CLOCK_SRC = 3U, /* Use the internal asynchronous clock source. */
} adcClockSource_t;

/*! @brief Long sample mode. */
typedef enum adcLongSampleMode
{
    ADC_LONG_SAMPLE_CYCLE_24 = 0U, /* 20 extra ADCK cycles, 24 ADCK cycles in total. */
    ADC_LONG_SAMPLE_CYCLE_16 = 1U, /* 12 extra ADCK cycles, 16 ADCK cycles in total. */
    ADC_LONG_SAMPLE_CYCLE_10 = 2U, /* 6 extra ADCK cycles, 10 ADCK cycles in total. */
    ADC_LONG_SAMPLE_CYCLE_6 = 3U, /* 2 extra ADCK cycles, 6 ADCK cycles in total. */
    ADC_LONG_SAMPLE_DISABLE = 4U, /* Disable the long sample feature. */
} adcLongSampleMode_t;

/*! @brief Hardware averaging mode. */
typedef enum adcHardwareAverageMode
{
    ADC_HARDWARE_AVG_COUNT_4 = 0U, /* Hardware averaging with 4 samples. */
    ADC_HARDWARE_AVG_COUNT_8 = 1U, /* Hardware averaging with 8 samples. */
    ADC_HARDWARE_AVG_COUNT_16 = 2U, /* Hardware averaging with 16 samples. */
    ADC_HARDWARE_AVG_COUNT_32 = 3U, /* Hardware averaging with 32 samples. */
    ADC_HARDWARE_AVG_DISABLE = 4U, /* Disable the hardware averaging feature. */
} adcHardwareAverageMode_t;

/*! @brief Single-ended or differential terminal mode. */
typedef enum adcTerminalMode
{
    ADC_IS_SINGLE_ENDED = 0U, /* Single-ended terminal mode. */
    ADC_IS_DIFFERENTIAL = 1U, /* Differential terminal mode. */
} adcTerminalMode_t;

/*! @brief Hardware trigger source for ADC start of conversion. */
typedef enum adcHardwareTriggerSrc
{
	ADC_EXTRG_IN 	  = 0x0U,
    ADC_CMP0_OUTPUT   = 0x1U,
    ADC_PIT_TRIGGER_0 = 0x4U,
	ADC_PIT_TRIGGER_1 = 0x5U,
    ADC_TPM0_OVERFLOW = 0x8U,
	ADC_TPM1_OVERFLOW = 0x9U,
	ADC_RTC_ALARM 	  = 0xCU,
	ADC_RTC_SECONDS   = 0xDU,
    ADC_LPTMR0_TRIGGER = 0xEU,
} adcHardwareTriggerSrc_t;

/**
 * @brief Hardware comparison mode. The converted value 'x' will only be available if
 *        the chosen condition is met. The values "value1" and "value2" are defined
 *        in adcHardwareCompareConfig_t.
 */
typedef enum adcHardwareCompareMode
{
    ADC_HARDWARE_COMPARE_MODE_0 = 0U, /* x < value1. */
    ADC_HARDWARE_COMPARE_MODE_1 = 1U, /* x > value1. */
    ADC_HARDWARE_COMPARE_MODE_2 = 2U, /* If value1 <= value2, then x < value1 || x > value2;
                                          else, value1 > x > value2. */
    ADC_HARDWARE_COMPARE_MODE_3 = 3U, /* If value1 <= value2, then value1 <= x <= value2;
                                          else x >= value1 || x <= value2. */
} adcHardwareCompareMode_t;


/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Sets the ADC clock divider value.
 *
 * This function sets the input clock divider for the ADC.
 *
 * @param base ADC peripheral base address.
 * @param div The clock divider value, as defined in adcClockDivider_t.
 */
static inline void ADC_SetClockDiv( ADC_Type *base, adcClockDivider_t div );

/*!
 * @brief Sets the ADC resolution.
 *
 * This function sets the resolution (number of bits) of the ADC.
 *
 * @param base ADC peripheral base address.
 * @param res The resolution value, as defined in adcResolution_t.
 */
static inline void ADC_SetResolution( ADC_Type *base, adcResolution_t res );

/*!
 * @brief Sets the ADC in low power mode.
 *
 * This function sets the ADC to low power mode for conversion.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_SetLowPowerMode( ADC_Type *base );

/*!
 * @brief Sets the ADC in normal power mode.
 *
 * This function sets the ADC to normal power mode for conversion.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_SetNormalPowerMode( ADC_Type *base );

/*!
 * @brief Sets the ADC in high-speed mode.
 *
 * This function sets the ADC to high-speed conversion mode.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_SetHighSpeedMode( ADC_Type *base );

/*!
 * @brief Sets the ADC in normal speed mode.
 *
 * This function sets the ADC to normal speed conversion mode.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_SetNormalSpeedMode( ADC_Type *base );

/*!
 * @brief Enables the asynchronous clock output.
 *
 * This function enables the asynchronous clock output for the ADC.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_EnableAsyncClkOut( ADC_Type *base );

/*!
 * @brief Disables the asynchronous clock output.
 *
 * This function disables the asynchronous clock output for the ADC.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_DisableAsyncClkOut( ADC_Type *base );

/*!
 * @brief Sets the ADC reference voltage to VAlt.
 *
 * This function sets the ADC reference voltage to VAlt.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_SetVAltVoltage( ADC_Type *base );

/*!
 * @brief Sets the ADC reference voltage to VRef.
 *
 * This function sets the ADC reference voltage to VRef.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_SetVRefVoltage( ADC_Type *base );

/*!
 * @brief Enables continuous conversion mode.
 *
 * This function enables continuous conversion mode for the ADC.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_EnableContinuousConversion( ADC_Type *base );

/*!
 * @brief Disables continuous conversion mode.
 *
 * This function disables continuous conversion mode for the ADC.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_DisableContinuousConversion( ADC_Type *base );

/*!
 * @brief Sets the input clock source for the ADC.
 *
 * This function sets the input clock source for the ADC.
 *
 * @param base ADC peripheral base address.
 * @param clk The input clock source, as defined in adcClockSource_t.
 */
static inline void ADC_SetInputInternalClock( ADC_Type *base, adcClockSource_t clk );

/*!
 * @brief Initializes the ADC.
 *
 * This function initializes the ADC module.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_Init(ADC_Type *base);

/*!
 * @brief Deinitializes the ADC.
 *
 * This function deinitializes the ADC module.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_Deinit(ADC_Type *base);

/*!
 * @brief Sets the ADC offset value.
 *
 * This function sets the offset value for the ADC.
 *
 * @param base ADC peripheral base address.
 * @param value The offset value to be set.
 */
static inline void ADC_SetOffsetValue(ADC_Type *base, int16_t value);

/*!
 * @brief Enables DMA trigger for the ADC.
 *
 * This function enables DMA triggering when ADC conversion is complete.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_EnableDMA(ADC_Type *base);

/*!
 * @brief Disables DMA trigger for the ADC.
 *
 * This function disables DMA triggering when ADC conversion is complete.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_DisableDMA(ADC_Type *base);

/*!
 * @brief Enables hardware trigger for the ADC.
 *
 * This function enables and selects the hardware trigger source for starting ADC conversion.
 *
 * @param base ADC peripheral base address.
 * @param src The hardware trigger source, as defined in adcHardwareTriggerSrc_t.
 */
static inline void ADC_EnableHardwareTrigger(ADC_Type *base, adcHardwareTriggerSrc_t src);

/*!
 * @brief Disables hardware trigger for the ADC.
 *
 * This function disables the hardware trigger source for starting ADC conversion.
 *
 * @param base ADC peripheral base address.
 */
static inline void ADC_DisableHardwareTrigger(ADC_Type *base);

/*!
 * @brief Gets the conversion value from the ADC channel.
 *
 * This function returns the conversion value from the configured ADC channel.
 *
 * @param base ADC peripheral base address.
 * @return The conversion value.
 */
static inline uint32_t ADC_GetChConversionValue( ADC_Type *base );

/*!
 * @brief Sets the hardware compare configuration.
 *
 * This function sets the hardware compare mode and comparison values for the ADC.
 *
 * @param base ADC peripheral base address.
 * @param hardwareCompareMode One of the four comparison modes defined in the enumerated type.
 * @param value1 The first comparison value.
 * @param value2 The second comparison value.
 */
void ADC_SetHardwareCompareConfig(ADC_Type *base, adcHardwareCompareMode_t hardwareCompareMode, int16_t value1, int16_t value2);

/*!
 * @brief Sets the hardware average mode for the ADC.
 *
 * This function configures the hardware averaging mode for the ADC.
 *
 * @param base ADC peripheral base address.
 * @param mode The number of samples to average, defined in the enumerated type.
 */
void ADC_SetHardwareAverage(ADC_Type *base, adcHardwareAverageMode_t mode);

/*!
 * @brief Gets the status flags of the ADC.
 *
 * @param base ADC peripheral base address.
 * @return The status flags, including ADC_ACTIVE_FLAG and ADC_CALIBRATION_FAILED_FLAG.
 */
uint32_t ADC_GetStatusFlags(ADC_Type *base);

/*!
 * @brief Clears the calibration status flag of the ADC.
 *
 * @param base ADC peripheral base address.
 */
void ADC_ClearCalibStatusFlags(ADC_Type *base);

/*!
 * @brief Configures an ADC channel.
 *
 * This function configures an ADC channel with the specified channel number, terminal mode, and interrupt setting.
 *
 * @param base ADC peripheral base address.
 * @param channelNumber The channel number.
 * @param enableIRQ Set to true to generate an interrupt at the end of conversion; set to false otherwise.
 */
void ADC_SetChConfig(ADC_Type *base, uint32_t channelNumber, bool enableIRQ);

/*!
 * @brief Checks if an ADC conversion is done.
 *
 * This function checks if the ADC conversion is complete.
 *
 * @param base ADC peripheral base address.
 * @return true if the conversion is finished, false otherwise.
 */
bool ADC_IsConversionDone( ADC_Type *base );

/*!
 * @brief Performs an auto-calibration for the ADC.
 *
 * @param base ADC peripheral base address.
 * @return STATUS_SUCCESS if calibration succeeded; STATUS_FAIL otherwise.
 */
uint8_t ADC_DoAutoCalibration(ADC_Type *base);


/*******************************************************************************
 * Code
 ******************************************************************************/

/**********************************************************************************/
static inline void ADC_SetClockDiv( ADC_Type *base, adcClockDivider_t div )
{
	SYSTEM_ASSERT(base);

	base->CFG1 &= ~ADC_CFG1_ADIV_MASK;
	base->CFG1 |= ADC_CFG1_ADIV(div);
}

/**********************************************************************************/
static inline void ADC_SetResolution( ADC_Type *base, adcResolution_t res )
{
	SYSTEM_ASSERT(base);

	base->CFG1 &= ~ADC_CFG1_MODE_MASK;
	base->CFG1 |= ADC_CFG1_MODE(res);
}

/**********************************************************************************/
static inline void ADC_SetLowPowerMode( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->CFG1 |= ADC_CFG1_ADLPC_MASK;
}

/**********************************************************************************/
static inline void ADC_SetNormalPowerMode( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->CFG1 &= ~ADC_CFG1_ADLPC_MASK;
}

/**********************************************************************************/
static inline void ADC_SetHighSpeedMode( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->CFG2 |= ADC_CFG2_ADLSTS_MASK;
}

/**********************************************************************************/
static inline void ADC_SetNormalSpeedMode( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->CFG2 &= ~ADC_CFG2_ADLSTS_MASK;
}

/**********************************************************************************/
static inline void ADC_EnableAsyncClkOut( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->CFG2 |= ADC_CFG2_ADACKEN_MASK;
}

/**********************************************************************************/
static inline void ADC_DisableAsyncClkOut( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->CFG2 &= ~ADC_CFG2_ADACKEN_MASK;
}

/**********************************************************************************/
static inline void ADC_SetVAltVoltage( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->SC2 &= ~ADC_SC2_REFSEL_MASK;
	base->SC2 |= ADC_SC2_REFSEL(1U);
}

/**********************************************************************************/
static inline void ADC_SetVRefVoltage( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->SC2 &= ~ADC_SC2_REFSEL_MASK;
}

/**********************************************************************************/
static inline void ADC_EnableContinuousConversion( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->SC3 |= ADC_SC3_ADCO_MASK;
}

/**********************************************************************************/
static inline void ADC_DisableContinuousConversion( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->SC3 &= ~ADC_SC3_ADCO_MASK;
}

/**********************************************************************************/
static inline void ADC_SetInputInternalClock( ADC_Type *base, adcClockSource_t clk )
{
	SYSTEM_ASSERT(base);

	base->CFG1 &= ~ADC_CFG1_ADICLK_MASK;
	base->CFG1 |= ADC_CFG1_ADICLK(clk);
}

/**********************************************************************************/
static inline void ADC_Init(ADC_Type *base)
{
	SYSTEM_ASSERT(NULL != base);

    /* Enable the clock. */
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
}

/**********************************************************************************/
static inline void ADC_Deinit(ADC_Type *base)
{
	SYSTEM_ASSERT(NULL != base);

    /* Disable the clock. */
	SIM->SCGC6 &= ~SIM_SCGC6_ADC0_MASK;
}

/**********************************************************************************/
static inline void ADC_SetOffsetValue(ADC_Type *base, int16_t value)
{
	SYSTEM_ASSERT(NULL != base);

    base->OFS = (uint32_t)(value);
}

/**********************************************************************************/
static inline void ADC_EnableDMA(ADC_Type *base)
{
	SYSTEM_ASSERT(NULL != base);

	base->SC2 |= ADC_SC2_DMAEN_MASK;
}

/**********************************************************************************/
static inline void ADC_DisableDMA(ADC_Type *base)
{
	SYSTEM_ASSERT(NULL != base);

	base->SC2 &= ~ADC_SC2_DMAEN_MASK;
}

/**********************************************************************************/
static inline void ADC_EnableHardwareTrigger(ADC_Type *base, adcHardwareTriggerSrc_t src)
{
	SYSTEM_ASSERT(NULL != base);

	/* Configure SIM for ADC hw trigger source selection */
	SIM->SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN_MASK;
	SIM->SOPT7 |= SIM_SOPT7_ADC0TRGSEL(src);
	base->SC2 |= ADC_SC2_ADTRG_MASK;
}

/**********************************************************************************/
static inline void ADC_DisableHardwareTrigger(ADC_Type *base)
{
	SYSTEM_ASSERT(NULL != base);

	base->SC2 &= ~ADC_SC2_ADTRG_MASK;
}

/**********************************************************************************/
static inline uint32_t ADC_GetChConversionValue( ADC_Type *base )
{
	SYSTEM_ASSERT(NULL != base);

    return base->R[0];
}

/*! @}*/

#if defined(__cplusplus)
}
#endif

#endif /* ADC_DRV_H_ */
