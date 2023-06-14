/***************************************************************************************
 * Módulo      : adc.c
 * Revisão     : 1.1
 * Data        : 24/05/2023
 * Descrição   : Arquivo com implementações do Módulo ADC da Familia Kinetis KL05.
 * Comentários : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

/*ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

#include "adc.h"

/*FIM: ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

/*FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

void ADC_SetLongSampleTimeMode( ADC_Type *base, adcLongSampleMode_t mode )
{
	assert(base);

	base->CFG1 &= ~ADC_CFG1_ADLSMP_MASK; /*Limpa configurações anteriores*/
	if ( mode != ADC_LONG_SAMPLE_DISABLE )
	{
		base->CFG1 |= ADC_CFG1_ADLSMP(1U);

		base->CFG2 &= ~ADC_CFG2_ADLSTS_MASK;
		base->CFG2 |= ADC_CFG2_ADLSTS(mode);
	}
}

uint8_t ADC_DoAutoCalibration(ADC_Type *base)
{
    bool bHWTrigger = false;
    volatile uint32_t tmp32; /* 'volatile' here is for the dummy read of ADCx_R[0] register. */
    uint8_t status = SYSTEM_STATUS_SUCCESS;

    /* The calibration would be failed when in hardware mode.
     * Remember the hardware trigger state here and restore it later if the hardware trigger is enabled.*/
    if (0U != (ADC_SC2_ADTRG_MASK & base->SC2))
    {
        bHWTrigger = true;
        base->SC2 &= ~ADC_SC2_ADTRG_MASK;
    }

    /* Clear the CALF and launch the calibration. */
    base->SC3 |= ADC_SC3_CAL_MASK | ADC_SC3_CALF_MASK;

    while (false == ADC_IsConversionDone(base))
    {
        /* Check the CALF when the calibration is active. */
        if ( (ADC_CALIBRATION_FAILED_FLAG & ADC_GetStatusFlags(base)) != 0U )
        {
            status = SYSTEM_STATUS_FAIL;
            break;
        }
    }
    tmp32 = base->R[0]; /* Dummy read to clear COCO caused by calibration. */

    /* Restore the hardware trigger setting if it was enabled before. */
    if (bHWTrigger)
    {
        base->SC2 |= ADC_SC2_ADTRG_MASK;
    }
    /* Check the CALF at the end of calibration. */
    if ( (ADC_CALIBRATION_FAILED_FLAG & ADC_GetStatusFlags(base)) != 0U )
    {
        status = SYSTEM_STATUS_FAIL;
    }
    if ( status!= SYSTEM_STATUS_SUCCESS ) /* Check if the calibration process is succeed. */
    {
        return status;
    }

    /* Calculate the calibration values. */
    tmp32 = base->CLP0 + base->CLP1 + base->CLP2 + base->CLP3 + base->CLP4 + base->CLPS;
    tmp32 = 0x8000U | (tmp32 >> 1U);
    base->PG = tmp32;

    return SYSTEM_STATUS_SUCCESS;
}

void ADC_SetHardwareCompareConfig(ADC_Type *base, adcHardwareCompareMode_t hardwareCompareMode, int16_t value1, int16_t value2)
{
    uint32_t tmp32 = base->SC2 & ~(ADC_SC2_ACFE_MASK | ADC_SC2_ACFGT_MASK | ADC_SC2_ACREN_MASK);

    /* Enable the feature. */
    tmp32 |= ADC_SC2_ACFE_MASK;

    /* Select the hardware compare working mode. */
    switch (hardwareCompareMode)
    {
        case ADC_HARDWARE_COMPARE_MODE_0:
            break;
        case ADC_HARDWARE_COMPARE_MODE_1:
            tmp32 |= ADC_SC2_ACFGT_MASK;
            break;
        case ADC_HARDWARE_COMPARE_MODE_2:
            tmp32 |= ADC_SC2_ACREN_MASK;
            break;
        case ADC_HARDWARE_COMPARE_MODE_3:
            tmp32 |= ADC_SC2_ACFGT_MASK | ADC_SC2_ACREN_MASK;
            break;
        default:
            break;
    }
    base->SC2 = tmp32;

    /* Load the compare values. */
    base->CV1 = ADC_CV1_CV(value1);
    base->CV2 = ADC_CV2_CV(value2);
}


void ADC_SetHardwareAverage(ADC_Type *base, adcHardwareAverageMode_t mode)
{
    uint32_t tmp32 = base->SC3 & ~(ADC_SC3_AVGE_MASK | ADC_SC3_AVGS_MASK);

    if (mode != ADC_HARDWARE_AVG_DISABLE)
    {
        tmp32 |= ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(mode);
    }
    base->SC3 = tmp32;
}


uint32_t ADC_GetStatusFlags(ADC_Type *base)
{
    uint32_t ret = 0;

    if (0U != (base->SC2 & ADC_SC2_ADACT_MASK))
    {
        ret |= ADC_ACTIVE_FLAG;
    }
    if (0U != (base->SC3 & ADC_SC3_CALF_MASK))
    {
        ret |= ADC_CALIBRATION_FAILED_FLAG;
    }
    return ret;
}

void ADC_ClearCalibStatusFlags(ADC_Type *base)
{
	base->SC3 |= ADC_SC3_CALF_MASK;
}

void ADC_SetChConfig(ADC_Type *base, uint32_t channelNumber, bool enableIRQ)
{
    assert(base);

    uint32_t sc1 = ADC_SC1_ADCH(channelNumber); /* Set the channel number. */

    /* Enable the interrupt when the conversion is done. */
    if (enableIRQ)
    {
        sc1 |= ADC_SC1_AIEN_MASK;
    }

    /* In Software Trigger mode, when SC2->ADTRG=0, writes to SC1[0] (channel A)
    subsequently initiate a new conversion, if SC1->ADCH contains a value other than all 1s.*/
    base->SC1[0] = sc1;
}

bool ADC_IsConversionDone( ADC_Type *base )
{
    assert(base);

    bool ret = false;

    if (0U != (base->SC1[0] & ADC_SC1_COCO_MASK))
    {
        ret = true;
    }
    return ret;
}

/*FIM: FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/***************************************************************************************
 * FIM: Módulo - adc.c
 ***************************************************************************************/

