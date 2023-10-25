/***************************************************************************************
 * @file tpm.c
 * @version 1.1
 * @date 05/24/2023
 * @brief File with implementations of the Timer/PWM (TPM) Modules for the Kinetis KL05 Family.
 * @details None.
 * @author Matheus Leitzke Pinto
 ***************************************************************************************/

/* HEADER FILES */
/*=======================================================================================*/

#include "tpm.h"

/* END: HEADER FILES */
/*=======================================================================================*/

/* PUBLIC FUNCTIONS */
/*=======================================================================================*/

/**********************************************************************
 * @fn void TPM_Init(TPM_Type *base, uint16_t modulo, tpmPrescalerValues_t prescale)
 * @brief Initializes the TPM driver module.
 * @param base - TPM peripheral base register.
 * @param modulo - the count endpoint value.
 * @param prescale - an enumerated type with one of the possible prescaler values.
 * @return None.
 * @note Must be called after the TPM_SetCounterClkSrc function.
 ********************************************************************/
void TPM_Init(TPM_Type *base, uint16_t modulo, tpmPrescalerValues_t prescale)
{
    SYSTEM_ASSERT(base);

    if ( base == TPM0 )
    {
    	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; /* Activate TPM0 module clock */
    }

    if ( base == TPM1 )
    {
    	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK; /* Activate TPM1 module clock */
    }

    base->MOD = modulo;

    base->SC &= ~TPM_SC_PS_MASK;
    base->SC |= TPM_SC_PS(prescale);

    base->CNT = 0x00U; /* Reset the counter register */
}

/**********************************************************************
 * @fn void TPM_InitChannel(TPM_Type *base, uint8_t chNum, tpmChMode_t mode, tpmChConfig_t config)
 * @brief Initializes a channel of the TPM module.
 * @param base - TPM peripheral base register.
 * @param chNum - channel number.
 * @param mode - channel operating mode, as defined by the tpmChMode_t type.
 * @param config - possible configurations for each channel operating mode, as defined by the tpmChConfig_t type.
 * @return None.
 * @note None.
 ********************************************************************/
void TPM_InitChannel(TPM_Type *base, uint8_t chNum, tpmChMode_t mode, tpmChConfig_t config)
{
    SYSTEM_ASSERT(base);
    SYSTEM_ASSERT((chNum >= 0) && (chNum < 7));

    switch (mode)
    {
    case (TPM_OUT_COMPARE_MODE):
        base->SC &= ~TPM_SC_CPWMS_MASK; /* Not Center PWM */
        if (config == TPM_OUT_TOGGLE_GONFIG)
            base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | TPM_CnSC_ELSA_MASK);
        else if (config == TPM_OUT_CLEAR_GONFIG)
            base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK);
        else if (config == TPM_OUT_SET_GONFIG)
            base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | (TPM_CnSC_ELSA_MASK) | TPM_CnSC_ELSA_MASK);
        break;
    case (TPM_IN_COMPARE_MODE):
        base->SC &= ~TPM_SC_CPWMS_MASK; /* Not Center PWM */
        if (config == TPM_IN_RISING_EDGE_CONFIG)
            base->CONTROLS[chNum].CnSC = TPM_CnSC_ELSA_MASK;
        else if (config == TPM_IN_FALLING_EDGE_CONFIG)
            base->CONTROLS[chNum].CnSC = TPM_CnSC_ELSB_MASK;
        else
            base->CONTROLS[chNum].CnSC = (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSA_MASK);
        break;
    case (TPM_EDGE_PWM_MODE):
        base->SC &= ~TPM_SC_CPWMS_MASK; /* Not Center PWM */
        if (config == TPM_PWM_HIGH_TRUE_CONFIG)
            base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
        else
            base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
        break;
    case (TPM_CENTER_PWM_MODE):
        base->SC |= TPM_SC_CPWMS_MASK; /* Is Center PWM */
        if (config == TPM_PWM_HIGH_TRUE_CONFIG)
            base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
        else
            base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
        break;
    default:
        break;
    }
}

/**********************************************************************
 * @fn void TPM_SetFrequency(TPM_Type *base, uint32_t freq, uint8_t channel)
 * @brief Configures the operating frequency of the TPM module.
 * @param base - TPM peripheral base register.
 * @param freq - desired operating frequency.
 * @param channel - TPM channel number.
 * @return None.
 * @note None.
 ********************************************************************/
void TPM_SetFrequency(TPM_Type *base, uint32_t freq, uint8_t channel)
{
    assert(base);
    assert(freq >= 0);

    /*!< Receive the clock frequency of the timer's base clock. */
    uint32_t clock_freq = TPM_GetClockFrequency();

    /*!< Calculate the value of the maximum timer value. */
    uint32_t modulo = (uint32_t)(((clock_freq / (freq))) - 1);

    /*!< Divide the modulo by the prescaler while the modulo is greater than the maximum timer value. */
    uint8_t prescaler = 0;
    while (modulo > 0xFFFF)
    {
        modulo = modulo >> 1;
        prescaler++;

        /*!< Check if the prescaler has more than 3 bits. */
        if (prescaler >> 4)
        {
            modulo = 0xFFFF;
            prescaler = 0b111;
            break;
        }
    }

    uint16_t old_module = TPM_GetModulo(base);
    uint16_t ch_value = TPM_GetChValue(base, channel);

    /*!< Configure the timer's modulo and prescaler. */
    TPM_SetModulo(base, modulo);
    TPM_SetPrescaler(base, prescaler);

    /*!< Update the PWM value for channel 2. */
    uint16_t new_ch_value = (uint16_t)((ch_value * modulo) / old_module);
    TPM_SetChMatch(base, channel, new_ch_value);
}

/**********************************************************************
 * @fn uint32_t TPM_GetClockFrequency()
 * @brief Returns the frequency used to drive the timer.
 * @param None.
 * @return Frequency of the timer's base clock.
 * @note None.
 ********************************************************************/
uint32_t TPM_GetClockFrequency()
{
    uint32_t value = ((SIM->SOPT2 & SIM_SOPT2_TPMSRC_MASK) >> SIM_SOPT2_TPMSRC_SHIFT);

    switch (value)
    {
    case TPM_CNT_CLOCK_DISABLE:
        return 0;
    case TPM_CNT_CLOCK_FLL:
        return MCG_FLL_CLK_FREQ;
    case TPM_CNT_CLOCK_IRC:
        return MCG_IRC_CLK_FREQ;
    case TPM_CNT_CLOCK_OSC:
        return OSC_ER_CLK_FREQ;
    default:
        return 0;
    }
}

/* END: PUBLIC FUNCTIONS */
/*=======================================================================================*/

/***************************************************************************************
 * END: Module - tpm.c
 ***************************************************************************************/
