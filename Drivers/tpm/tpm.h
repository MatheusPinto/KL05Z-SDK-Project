/***************************************************************************************
 * @file tpm.h
 * @version 1.0
 * @date 05/24/2023
 * @brief File with implementations of the Timer/PWM (TPM) Modules for the Kinetis KL05 Family.
 * @details None.
 * @author Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef TPM_DRV_H_
#define TPM_DRV_H_

#include <common.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @addtogroup tpm driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * @enum tpmChMode_t
 * @brief Possible operating modes of the TPM modules.
 */
typedef enum {
    TPM_OUT_COMPARE_MODE,       /**< Output compare mode */
    TPM_IN_COMPARE_MODE,        /**< Input capture mode */
    TPM_EDGE_PWM_MODE,          /**< Edge-aligned PWM mode */
    TPM_CENTER_PWM_MODE         /**< Center-aligned PWM mode */
} tpmChMode_t;

/**
 * @enum tpmChConfig_t
 * @brief Possible configurations for the channel operation modes of the TPM modules.
 */
typedef enum {
    TPM_OUT_TOGGLE_GONFIG,       /**< Output compare mode: toggle output logic */
    TPM_OUT_CLEAR_GONFIG,       /**< Output compare mode: clear output */
    TPM_OUT_SET_GONFIG,         /**< Output compare mode: set output */
    TPM_IN_RISING_EDGE_CONFIG,  /**< Input capture mode: rising edge */
    TPM_IN_FALLING_EDGE_CONFIG, /**< Input capture mode: falling edge */
    TPM_IN_BOTH_EDGE_CONFIG,    /**< Input capture mode: both edges */
    TPM_PWM_HIGH_TRUE_CONFIG,   /**< PWM modes: Pulse is high */
    TPM_PWM_LOW_TRUE_CONFIG     /**< PWM modes: Pulse is low */
} tpmChConfig_t;

/**
 * @enum tpmPrescalerValues_t
 * @brief Possible values for the prescaler division of the TPM modules.
 */
typedef enum {
    TPM_PRESCALER_DIV_1 = 0x0,  /**< TPM module prescaler divides by 0 */
    TPM_PRESCALER_DIV_2 = 0x1,  /**< TPM module prescaler divides by 1 */
    TPM_PRESCALER_DIV_4 = 0x2,  /**< TPM module prescaler divides by 2 */
    TPM_PRESCALER_DIV_8 = 0x3,  /**< TPM module prescaler divides by 3 */
    TPM_PRESCALER_DIV_16 = 0x4, /**< TPM module prescaler divides by 4 */
    TPM_PRESCALER_DIV_32 = 0x5, /**< TPM module prescaler divides by 5 */
    TPM_PRESCALER_DIV_64 = 0x6, /**< TPM module prescaler divides by 6 */
    TPM_PRESCALER_DIV_128 = 0x7 /**< TPM module prescaler divides by 7 */
} tpmPrescalerValues_t;

/**
 * @enum tpmClkSrc_t
 * @brief Possible clock sources for TPM modules.
 */
typedef enum {
    TPM_CNT_CLOCK_DISABLE = 0U,
    TPM_CNT_CLOCK_FLL = 1U,
    TPM_CNT_CLOCK_OSC = 2U,
    TPM_CNT_CLOCK_IRC = 3U
} tpmClkSrc_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Activates the counter clock to start counting.
 * @param base - TPM peripheral base register.
 */
static inline void TPM_InitCounter(TPM_Type *base);

/**
 * @brief Deactivates the counter clock to stop counting.
 * @param base - TPM peripheral base register.
 */
static inline void TPM_StopCounter(TPM_Type *base);

/**
 * @brief Sets the clock source used by the counter.
 * @param base - TPM peripheral base register.
 * @param src - Clock source for the counter.
 */
static inline void TPM_SetCounterClkSrc(TPM_Type *base, tpmClkSrc_t src);

/**
 * @brief Sets the counter to stop counting when an overflow occurs.
 * @param base - TPM peripheral base register.
 */
static inline void TPM_SetStopOnOverflow(TPM_Type *base);

/**
 * @brief Sets the counter to continue counting when an overflow occurs.
 * @param base - TPM peripheral base register.
 */
static inline void TPM_SetContinueOnOverflow(TPM_Type *base);

/**
 * @brief Returns the value of the counter register in "Input Capture" mode or the "match" value in "Output Compare" mode for channel 0 of the TPM module.
 * @param base - TPM peripheral base register.
 * @param channel - TPM channel number.
 * @return The value of the counter register or match value.
 */
static inline uint16_t TPM_GetChValue(TPM_Type *base, uint8_t channel);

/**
 * @brief Sets the "match" value in the channel of TPM0 module.
 * @param base - TPM peripheral base register.
 * @param channel - TPM channel number.
 * @param match - The "match" value to set.
 */
static inline void TPM_SetChMatch(TPM_Type *base, uint8_t channel, uint16_t match);

/**
 * @brief Enables interrupt generation by the TPM module.
 * @param base - TPM peripheral base register.
 */
static inline void TPM_EnableIRQ(TPM_Type *base);

/**
 * @brief Returns the value of the interrupt flag generated by the TPM module.
 * @param base - TPM peripheral base register.
 * @return 0 if no interrupt occurred, nonzero if an interrupt occurred.
 */
static inline uint32_t TPM_GetIRQFlag(TPM_Type *base);

/**
 * @brief Clears the interrupt flag generated by the TPM module.
 * @param base - TPM peripheral base register.
 */
static inline void TPM_ClearIRQFlag(TPM_Type *base);

/**
 * @brief Sets the end-count value of the TPM module.
 * @param base - TPM peripheral base register.
 * @param modulo - The end-count value.
 */
static inline void TPM_SetModulo(TPM_Type *base, uint16_t modulo);

/**
 * @fn void TPM_Init(TPM_Type *base, uint16_t modulo, tpmPrescalerValues_t prescale)
 * @brief Initializes the TPM driver module.
 * @param base - TPM peripheral base register.
 * @param modulo - The count endpoint value.
 * @param prescale - An enumerated type with one of the possible prescaler values.
 * @return None.
 * @note Must be called after the TPM_SetCounterClkSrc function.
 */
void TPM_Init(TPM_Type *base, uint16_t modulo, tpmPrescalerValues_t prescale);

/**
 * @fn void TPM_InitChannel(TPM_Type *base, uint8_t chNum, tpmChMode_t mode, tpmChConfig_t config)
 * @brief Initializes a channel of the TPM module.
 * @param base - TPM peripheral base register.
 * @param chNum - Channel number.
 * @param mode - Channel operating mode, as defined by the tpmChMode_t type.
 * @param config - Possible configurations for each channel operating mode, as defined by the tpmChConfig_t type.
 * @return None.
 * @note None.
 */
void TPM_InitChannel(TPM_Type *base, uint8_t chNum, tpmChMode_t mode, tpmChConfig_t config);

/**
 * @fn void TPM_SetFrequency(TPM_Type *base, uint32_t freq, uint8_t channel)
 * @brief Configures the operating frequency of the TPM module.
 * @param base - TPM peripheral base register.
 * @param freq - Desired operating frequency.
 * @param channel - TPM channel number.
 * @return None.
 * @note None.
 */
void TPM_SetFrequency(TPM_Type *base, uint32_t freq, uint8_t channel);

/**
 * @fn uint32_t TPM_GetClockFrequency()
 * @brief Returns the frequency used to drive the timer.
 * @param base - TPM peripheral base register.
 * @return The clock frequency.
 * @note None.
 */
uint32_t TPM_GetClockFrequency();

/**
 * @fn static inline void TPM_InitCounter(TPM_Type *base)
 * @brief Activates the counter clock to start counting.
 * @param base - TPM peripheral base register.
 * @return None.
 * @note None.
 */
static inline void TPM_InitCounter(TPM_Type *base)
{
    SYSTEM_ASSERT(base);
    base->SC |= (TPM_SC_CMOD(0x1U));
}

/**
 * @fn static inline void TPM_StopCounter(TPM_Type *base)
 * @brief Deactivates the counter clock to stop counting.
 * @param base - TPM peripheral base register.
 * @return None.
 * @note None.
 */
static inline void TPM_StopCounter(TPM_Type *base)
{
    SYSTEM_ASSERT(base);
    base->SC &= ~TPM_SC_CMOD_MASK;
}

/**
 * @fn static inline void TPM_SetCounterClkSrc(TPM_Type *base, tpmClkSrc_t src)
 * @brief Sets the clock source used by the counter.
 * @param base - TPM peripheral base register.
 * @param src - An enumerated type defining the clock source.
 * @return None.
 * @note Must be called before the TPM_Init function.
 */
static inline void TPM_SetCounterClkSrc(TPM_Type *base, tpmClkSrc_t src)
{
    SYSTEM_ASSERT(base);
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(src);
}

/**
 * @fn static inline void TPM_SetStopOnOverflow(TPM_Type *base)
 * @brief Sets the counter to stop counting when an overflow occurs.
 * @param base - TPM peripheral base register.
 * @return None.
 * @note None.
 */
static inline void TPM_SetStopOnOverflow(TPM_Type *base)
{
    SYSTEM_ASSERT(base);
    base->CONF |= (TPM_CONF_CSOO(0x1U));
}

/**
 * @fn static inline void TPM_SetContinueOnOverflow(TPM_Type *base)
 * @brief Sets the counter to continue counting when an overflow occurs.
 * @param base - TPM peripheral base register.
 * @return None.
 * @note None.
 */
static inline void TPM_SetContinueOnOverflow(TPM_Type *base)
{
    SYSTEM_ASSERT(base);
    base->CONF &= ~(TPM_CONF_CSOO(0x1U));
}

/**
 * @fn static inline uint16_t TPM_GetChValue(TPM_Type *base, uint8_t channel)
 * @brief Returns the value of the counter register in "Input Capture" mode or the "match" value in "Output Compare" mode for channel 0 of the TPM module.
 * @param base - TPM peripheral base register.
 * @param channel - TPM channel number.
 * @return The value of the counter register or match value.
 * @note None.
 */
static inline uint16_t TPM_GetChValue(TPM_Type *base, uint8_t channel)
{
    SYSTEM_ASSERT(base);
    return (uint16_t)base->CONTROLS[channel].CnV;
}

/**
 * @fn static inline void TPM_SetChMatch(TPM_Type *base, uint8_t channel, uint16_t match)
 * @brief Sets the "match" value in the channel of TPM0 module.
 * @param base - TPM peripheral base register.
 * @param channel - TPM channel number.
 * @param match - The "match" value to set.
 * @return None.
 * @note None.
 */
static inline void TPM_SetChMatch(TPM_Type *base, uint8_t channel, uint16_t match)
{
    SYSTEM_ASSERT(base);
    base->CONTROLS[channel].CnV = match;
}

/**
 * @fn static inline void TPM_EnableIRQ(TPM_Type *base)
 * @brief Enables interrupt generation by the TPM module.
 * @param base - TPM peripheral base register.
 * @return None.
 * @note None.
 */
static inline void TPM_EnableIRQ(TPM_Type *base)
{
    SYSTEM_ASSERT(base);
    base->SC |= TPM_SC_TOIE_MASK;
}

/**
 * @fn static inline uint32_t TPM_GetIRQFlag(TPM_Type *base)
 * @brief Returns the value of the interrupt flag generated by the TPM module.
 * @param base - TPM peripheral base register.
 * @return 0 if no interrupt occurred, nonzero if an interrupt occurred.
 * @note None.
 */
static inline uint32_t TPM_GetIRQFlag(TPM_Type *base)
{
    SYSTEM_ASSERT(base);
    return base->SC & TPM_SC_TOF_MASK;
}

/**
 * @fn static inline void TPM_ClearIRQFlag(TPM_Type *base)
 * @brief Clears the interrupt flag generated by the TPM module.
 * @param base - TPM peripheral base register.
 * @return None.
 * @note None.
 */
static inline void TPM_ClearIRQFlag(TPM_Type *base)
{
    SYSTEM_ASSERT(base);
    base->SC |= TPM_SC_TOF_MASK;
}

/**
 * @fn static inline void TPM_SetModulo(TPM_Type *base, uint16_t modulo)
 * @brief Sets the end-count value of the TPM module.
 * @param base - TPM peripheral base register.
 * @param modulo - The end-count value.
 * @return None.
 * @note None.
 */
static inline void TPM_SetModulo(TPM_Type *base, uint16_t modulo)
{
    SYSTEM_ASSERT(base);
    base->MOD = modulo;
}

/**
 * @fn static inline uint16_t TPM_GetModulo(TPM_Type *base)
 * @brief Returns the end-count value of TPM0 module.
 * @param base - TPM peripheral base register.
 * @return 16-bit end-count value.
 * @note None.
 */
static inline uint16_t TPM_GetModulo(TPM_Type *base)
{
    assert(base);
    return (uint16_t)base->MOD;
}

/**
 * @fn void TPM_SetPrescaler(TPM_Type *base, tpmPrescalerValues_t prescaler)
 * @brief Sets the prescaler value for the TPM0 module.
 * @param base - TPM peripheral base register.
 * @param prescaler - An enumerated type for the prescaler value.
 * @return None.
 * @note None.
 */
static inline void TPM_SetPrescaler(TPM_Type *base, tpmPrescalerValues_t prescaler)
{
    assert(base);
    /** Stop the counter because PS is read-only while the counter is on */
    TPM_StopCounter(base);
    /* Clear the prescaler bits */
    base->SC &= ~TPM_SC_PS_MASK;
    /* Set the prescaler */
    base->SC |= TPM_SC_PS(prescaler);
    /* Clear the counter */
    base->CNT = 0x00U;
    TPM_InitCounter(base);
}

/*! @}*/

#if defined(__cplusplus)
}
#endif

#endif /* TPM_DRV_H_ */