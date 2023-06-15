/***************************************************************************************
 * Módulo      : adc.h
 * Revisão     : 1.1
 * Data        : 24/05/2023
 * Descrição   : Arquivo com implementações do Módulo ADC da Familia Kinetis KL05.
 * Comentários : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef ADC_DRV_H_
#define ADC_DRV_H_

/*ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

#include <mcu/common.h>

/*FIM: ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

#if defined(__cplusplus)
extern "C" {
#endif

/*TIPOS ENUMERADOS*/
/*=======================================================================================*/

/*Flags de status do canal*/
typedef enum adcChannelStatusFlag
{
    ADC_CHANNEL_CONVERSION_DONE_FLAG = ADC_SC1_COCO_MASK, /* Conversão pronta. */
}adcChannelStatusFlag_t;

/*Flags de status do ADC*/
typedef enum adcStatusFlag
{
    ADC_ACTIVE_FLAG = ADC_SC2_ADACT_MASK, /*ADC está ativo. */
    ADC_CALIBRATION_FAILED_FLAG = ADC_SC3_CALF_MASK, /*!< Calibração falhou. */
}adcStatusFlag_t;

/*Divisor do clock do ADC*/
typedef enum adcClockDivider
{
    ADC_CLOCK_DIV_1 = 0U, /* Dividir por 1 a entrada do clock para o módulo. */
    ADC_CLOCK_DIV_2 = 1U, /* Dividir por 2 a entrada do clock para o módulo. */
    ADC_CLOCK_DIV_4 = 2U, /* Dividir por 4 a entrada do clock para o módulo. */
    ADC_CLOCK_DIV_8 = 3U, /* Dividir por 8 a entrada do clock para o módulo. */
} adcClockDivider_t;

/*Resolução do ADC*/
typedef enum adcResolution
{
    ADC_RESOLUTION_8_BIT = 0U,  /* Resolução de 8 bits. */
    ADC_RESOLUTION_12_BIT = 1U, /* Resolução de 12 bits. */
    ADC_RESOLUTION_10_BIT = 2U, /* Resolução de 10 bits. */
} adcResolution_t;

/*Fonte de clock do ADC*/
typedef enum adcClockSource
{
    ADC_BUS_CLOCK_SRC = 0U,       /* Usa clock do barramento como fonte de clock. */
    ADC_BUS_CLOCK_DIV_2_SRC = 1U, /* Usa clock do barramento/2 como fonte de clock. */
    ADC_ALT_CLOCK_SRC   = 2U,     /* Usa fonte de clock alternativa (OSCERCLK). */
	ADC_ASYNC_CLOCK_SRC = 3U      /* Usa fonte de clock assíncrona interna. */
} adcClockSource_t;

/* Modo de longa amostragem. */
typedef enum adcLongSampleMode
{
    ADC_LONG_SAMPLE_CYCLE_24 = 0U,  /* 20 extra ADCK cycles, 24 ADCK cycles total. */
    ADC_LONG_SAMPLE_CYCLE_16 = 1U,  /* 12 extra ADCK cycles, 16 ADCK cycles total. */
    ADC_LONG_SAMPLE_CYCLE_10 = 2U,  /* 6 extra ADCK cycles, 10 ADCK cycles total. */
    ADC_LONG_SAMPLE_CYCLE_6 = 3U,   /* 2 extra ADCK cycles, 6 ADCK cycles total. */
    ADC_LONG_SAMPLE_DISABLE = 4U, /* Desabilita o recurso de longa amostragem. */
} adcLongSampleMode_t;

/* Modo média por hardware. */
typedef enum adcHardwareAverageMode
{
    ADC_HARDWARE_AVG_COUNT_4 = 0U,   /* Média de hardware com 4 amostras. */
    ADC_HARDWARE_AVG_COUNT_8 = 1U,   /* Média de hardware com 8 amostras. */
    ADC_HARDWARE_AVG_COUNT_16 = 2U,  /* Média de hardware com 16 amostras. */
    ADC_HARDWARE_AVG_COUNT_32 = 3U,  /* Média de hardware com 32 amostras. */
    ADC_HARDWARE_AVG_DISABLE = 4U,   /* Desabilita o recurso de média por hardware.*/
} adcHardwareAverageMode_t;

/* Fonte de disparo de ínicio de conversão do ADC. */
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

/* Modo de comparação no hardware. O valor convertido x só estará disponível se
 * a condição escolhida for atendida. Os valores "value1" e "value2" são definidos
 * em  adcHardwareCompareConfig_t */
typedef enum adcHardwareCompareMode
{
    ADC_HARDWARE_COMPARE_MODE_0 = 0U, /* x < value1. */
    ADC_HARDWARE_COMPARE_MODE_1 = 1U, /* x > value1. */
    ADC_HARDWARE_COMPARE_MODE_2 = 2U, /* if value1 <= value2, then x < value1 || x > value2;
                                           else, value1 > x > value2. */
    ADC_HARDWARE_COMPARE_MODE_3 = 3U, /* if value1 <= value2, then value1 <= x <= value2;
                                           else x >= value1 || x <= value2. */
} adcHardwareCompareMode_t;

/*FIM: TIPOS ENUMERADOS*/
/*=======================================================================================*/


/*PROTÓTIPOS - FUNÇÕES INLINE*/
/*=======================================================================================*/

static inline void ADC_SetClockDiv( ADC_Type *base, adcClockDivider_t div );
static inline void ADC_SetResolution( ADC_Type *base, adcResolution_t res );
static inline void ADC_SetLowPowerMode( ADC_Type *base );
static inline void ADC_SetNormalPowerMode( ADC_Type *base );
static inline void ADC_SetHighSpeedMode( ADC_Type *base );
static inline void ADC_SetNormalSpeedMode( ADC_Type *base );
static inline void ADC_EnableAsyncClkOut( ADC_Type *base );
static inline void ADC_DisableAsyncClkOut( ADC_Type *base );
static inline void ADC_SetVAltVoltage( ADC_Type *base );
static inline void ADC_SetVRefVoltage( ADC_Type *base );
static inline void ADC_EnableContinuousConversion( ADC_Type *base );
static inline void ADC_DisableContinuousConversion( ADC_Type *base );
static inline void ADC_SetInputInternalClock( ADC_Type *base, adcClockSource_t clk );
static inline void ADC_Init(ADC_Type *base);
static inline void ADC_Deinit(ADC_Type *base);
static inline void ADC_SetOffsetValue(ADC_Type *base, int16_t value);
static inline void ADC_EnableDMA(ADC_Type *base);
static inline void ADC_DisableDMA(ADC_Type *base);
static inline void ADC_EnableHardwareTrigger(ADC_Type *base, adcHardwareTriggerSrc_t src);
static inline void ADC_DisableHardwareTrigger( ADC_Type *base );
static inline uint32_t ADC_GetChConversionValue( ADC_Type *base );

/*FIM: PROTÓTIPOS - FUNÇÕES INLINE*/
/*=======================================================================================*/


/*PROTÓTIPOS - FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Função		:	ADC_SetHardwareCompareConfig
 *
 * Descrição	:   Define o modo de comparação de conversão por
 *                  hardware.
 *
 * Entradas		:   *base - registrador base do periférico ADC;
 *                  hardwareCompareMode - um dos quatro modos de
 *                      comparação, definidos em tipo enumerado;
 *                  value1, value2 - os valores de comparação.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Verificar o tipo adcHardwareCompareMode_t para
 *                  mais detalhes.
 * ********************************************************************/
void ADC_SetHardwareCompareConfig(ADC_Type *base, adcHardwareCompareMode_t hardwareCompareMode, int16_t value1, int16_t value2);

/**********************************************************************
 * Função		:	ADC_SetHardwareAverage
 *
 * Descrição	:   Define o modo de conversão por média de amostras.
 *
 * Entradas		:   *base - registrador base do periférico ADC;
 *                  mode - quantidade de médias, definida por tipo
 *                         enumerado.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
void ADC_SetHardwareAverage(ADC_Type *base, adcHardwareAverageMode_t mode);

/**********************************************************************
 * Função		:	ADC_GetStatusFlags
 *
 * Descrição	:   Retorna a flag de status do ADC.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   - ADC_ACTIVE_FLAG, se ADC está ativo;
 *                  - ADC_CALIBRATION_FAILED_FLAG, se calibração falhou.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
uint32_t ADC_GetStatusFlags(ADC_Type *base);

/**********************************************************************
 * Função		:	ADC_ClearCalibStatusFlags
 *
 * Descrição	:   Limpa a flag de status de calibração do ADC.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
void ADC_ClearCalibStatusFlags(ADC_Type *base);

/**********************************************************************
 * Função		:	ADC_SetChConfig
 *
 * Descrição	:   Configura um canal do ADC.
 *
 * Entradas		:   *base - registrador base do periférico ADC;
 *                  channelNumber - número do canal;
 *                  enableInterruptOnConversionCompleted - true se deve ser
 *                     gerada interrupção no fim da conversão, ou false
 *                     caso contrário.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	O grupo do canal A é usado para conversões disparadas
 *                  por software, enquando o grupo B pode ser usado tanto
 *                  para disparo de software, quanto de hardware.
 * ********************************************************************/
void ADC_SetChConfig( ADC_Type *base, uint32_t channelNumber, bool enableInterruptOnConversionCompleted );

/**********************************************************************
 * Função		:	ADC_IsConversionDone
 *
 * Descrição	:   Retorna verdadeiro se conversão do ADC está pronta.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   true, se conversão finalizada;
 *                  false, caso contrário.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
bool ADC_IsConversionDone( ADC_Type *base );

/**********************************************************************
 * Função		:	ADC_DoAutoCalibration
 *
 * Descrição	:   Realiza auto-calibragem no ADC.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   STATUS_SUCCESS, calibração ocorreu corretamente;
 *                  STATUS_FAIL, caso contrário.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
uint8_t ADC_DoAutoCalibration(ADC_Type *base);

/*FIM: PROTÓTIPOS - FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/


/*FUNÇÕES INLINE*/
/*=======================================================================================*/

/**********************************************************************
 * Função		:	ADC_SetClockDiv
 *
 * Descrição	:   Define o valor de divisão do clock de entrada do ADC.
 *
 * Entradas		:   *base - registrador base do periférico ADC;
 *                  div   - valor de divisão, limitado por tipo enumerado.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetClockDiv( ADC_Type *base, adcClockDivider_t div )
{
	SYSTEM_ASSERT(base);

	base->CFG1 &= ~ADC_CFG1_ADIV_MASK;
	base->CFG1 |= ADC_CFG1_ADIV(div);
}

/**********************************************************************
 * Função		:	ADC_SetResolution
 *
 * Descrição	:   Define a resolução em bits do ADC.
 *
 * Entradas		:   *base - registrador base do periférico ADC;
 *                  res   - valor de resolução, limitado por tipo enumerado.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetResolution( ADC_Type *base, adcResolution_t res )
{
	SYSTEM_ASSERT(base);

	base->CFG1 &= ~ADC_CFG1_MODE_MASK;
	base->CFG1 |= ADC_CFG1_MODE(res);
}

/**********************************************************************
 * Função		:	ADC_SetLowPowerMode
 *
 * Descrição	:   Define o consumo de potência de conversão como baixa.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetLowPowerMode( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->CFG1 |= ADC_CFG1_ADLPC_MASK;
}

/**********************************************************************
 * Função		:	ADC_SetNormalPowerMode
 *
 * Descrição	:   Define o consumo de potência de conversão como normal.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetNormalPowerMode( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->CFG1 &= ~ADC_CFG1_ADLPC_MASK;
}

/**********************************************************************
 * Função		:	ADC_SetHighSpeedMode
 *
 * Descrição	:   Define a velocidade de conversão como rápida.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetHighSpeedMode( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->CFG2 |= ADC_CFG2_ADLSTS_MASK;
}

/**********************************************************************
 * Função		:	ADC_SetNormalSpeedMode
 *
 * Descrição	:   Define a velocidade de conversão como normal.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetNormalSpeedMode( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->CFG2 &= ~ADC_CFG2_ADLSTS_MASK;
}

/**********************************************************************
 * Função		:	ADC_EnableAsyncClkOut
 *
 * Descrição	:   Habilita saída do clock interno assíncrono.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_EnableAsyncClkOut( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->CFG2 |= ADC_CFG2_ADACKEN_MASK;
}

/**********************************************************************
 * Função		:	ADC_DisableAsyncClkOut
 *
 * Descrição	:   Desabilita saída do clock interno assíncrono.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_DisableAsyncClkOut( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->CFG2 &= ~ADC_CFG2_ADACKEN_MASK;
}

/**********************************************************************
 * Função		:	ADC_SetVAltVoltage
 *
 * Descrição	:   Define e tensão de referência como VAlt.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetVAltVoltage( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->SC2 &= ~ADC_SC2_REFSEL_MASK;
	base->SC2 |= ADC_SC2_REFSEL(1U);
}

/**********************************************************************
 * Função		:	ADC_SetVRefVoltage
 *
 * Descrição	:   Define e tensão de referência como VRef.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetVRefVoltage( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->SC2 &= ~ADC_SC2_REFSEL_MASK;
}

/**********************************************************************
 * Função		:	ADC_EnableContinuousConversion
 *
 * Descrição	:   Habilita o modo de conversão continua do módulo ADC.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_EnableContinuousConversion( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->SC3 |= ADC_SC3_ADCO_MASK;
}

/**********************************************************************
 * Função		:	ADC_DisableContinuousConversion
 *
 * Descrição	:   Desabilita o modo de conversão continua do módulo ADC.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_DisableContinuousConversion( ADC_Type *base )
{
	SYSTEM_ASSERT(base);

	base->SC3 &= ~ADC_SC3_ADCO_MASK;
}

/**********************************************************************
 * Função		:	ADC_SetInputInternalClock
 *
 * Descrição	:   Define a fonte de clock interno do ADC.
 *
 * Entradas		:   *base - registrador base do periférico ADC;
 *                  res   - valor do clock, limitado por tipo enumerado.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetInputInternalClock( ADC_Type *base, adcClockSource_t clk )
{
	SYSTEM_ASSERT(base);

	base->CFG1 &= ~ADC_CFG1_ADICLK_MASK;
	base->CFG1 |= ADC_CFG1_ADICLK(clk);
}

/**********************************************************************
 * Função		:	ADC_Init
 *
 * Descrição	:   Inicializa o módulo ADC.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_Init(ADC_Type *base)
{
	SYSTEM_ASSERT(NULL != base);

    /* Enable the clock. */
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
}

/**********************************************************************
 * Função		:	ADC_Deinit
 *
 * Descrição	:   Desabilita o módulo ADC.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_Deinit(ADC_Type *base)
{
	SYSTEM_ASSERT(NULL != base);

    /* Disable the clock. */
	SIM->SCGC6 &= ~SIM_SCGC6_ADC0_MASK;
}

/**********************************************************************
 * Função		:	ADC_SetOffsetValue
 *
 * Descrição	:   Esse valor de offset entra em vigor no
 *                  resultado da conversão. Se o valor do offset não
 *                  for zero, o resultado da leitura é subtraído por ele.
 *                  Observe que a calibração do hardware preenche o
 *                  valor de deslocamento automaticamente.
 *
 * Entradas		:   *base - registrador base do periférico ADC;
 *                  value - valor de offset.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetOffsetValue(ADC_Type *base, int16_t value)
{
	SYSTEM_ASSERT(NULL != base);

    base->OFS = (uint32_t)(value);
}

/**********************************************************************
 * Função		:	ADC_EnableDMA
 *
 * Descrição	:   Habilita geração de gatilho do DMA quando conversão
 *                  do ADC estiver completa.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_EnableDMA(ADC_Type *base)
{
	SYSTEM_ASSERT(NULL != base);

	base->SC2 |= ADC_SC2_DMAEN_MASK;
}

/**********************************************************************
 * Função		:	ADC_DisableDMA
 *
 * Descrição	:   Desabilita geração de gatilho do DMA quando conversão
 *                  do ADC estiver completa.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_DisableDMA(ADC_Type *base)
{
	SYSTEM_ASSERT(NULL != base);

	base->SC2 &= ~ADC_SC2_DMAEN_MASK;
}

/**********************************************************************
 * Função		:	ADC_EnableHardwareTrigger
 *
 * Descrição	:   Habilita e seleciona fonte de disparo de inicio
 *                  de conversão no ADC por hardware.
 *
 * Entradas		:   *base - registrador base do periférico ADC;
 * 					src   - uma das possíveis fontes de conversão
 * 					        definido em tipo enumerado.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_EnableHardwareTrigger(ADC_Type *base, adcHardwareTriggerSrc_t src)
{
	SYSTEM_ASSERT(NULL != base);

	/* Configure SIM for ADC hw trigger source selection */
	SIM->SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN_MASK;
	SIM->SOPT7 |= SIM_SOPT7_ADC0TRGSEL(src);
	base->SC2 |= ADC_SC2_ADTRG_MASK;
}

/**********************************************************************
 * Função		:	ADC_DisableHardwareTrigger
 *
 * Descrição	:   Desabilita a fonte de disparo de inicio
 *                  de conversão no ADC por hardware.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_DisableHardwareTrigger(ADC_Type *base)
{
	SYSTEM_ASSERT(NULL != base);

	base->SC2 &= ~ADC_SC2_ADTRG_MASK;
}

/**********************************************************************
 * Função		:	ADC_GetChConversionValue
 *
 * Descrição	:   Retorna o valor de conversão do ADC pelo canal configurado.
 *
 * Entradas		:   *base - registrador base do periférico ADC.
 *
 * Saídas		:   Valor da conversão.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline uint32_t ADC_GetChConversionValue( ADC_Type *base )
{
	SYSTEM_ASSERT(NULL != base);

    return base->R[0];
}

/*FIM: FUNÇÕES INLINE*/
/*=======================================================================================*/

#if defined(__cplusplus)
}
#endif

#endif /* ADC_DRV_H_ */

/***************************************************************************************
 * FIM: Módulo - adc.h
 ***************************************************************************************/

