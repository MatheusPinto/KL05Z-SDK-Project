/***************************************************************************************
 * Módulo      : tpm.h
 * Revisão     : 1.1
 * Data        : 24/05/2023
 * Descrição   : Arquivo com implementações dos Módulos Timer/PWM (TPM) da Familia Kinetis KL05.
 * Comentários : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef TPM_DRV_H_
#define TPM_DRV_H_

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

/*Possíveis modos de operação dos módulos TPM*/
typedef enum{
	TPM_OUT_COMPARE_MODE, 		/* Modo comparador de saída*/
	TPM_IN_COMPARE_MODE	,		/* Modo captura de entrada*/
	TPM_EDGE_PWM_MODE	,		/* Modo PWM alinhado a borda*/
	TPM_CENTER_PWM_MODE			/* Modo PWM alinhado ao centro */
}tpmChMode_t;

/*Possíveis configurações do modos de operação dos módulos TPM*/
typedef enum{
	TPM_OUT_TOGGLE_GONFIG		, 	/*Modo comparação de saída: inverte lógica de saída*/
	TPM_OUT_CLEAR_GONFIG		,  	/*Modo comparação de saída: limpa saída*/
	TPM_OUT_SET_GONFIG			,  	/*Modo comparação de saída: seta saída*/
	TPM_IN_RISING_EDGE_CONFIG	, 	/*Modo captura de entrada: borda de subida*/
	TPM_IN_FALLING_EDGE_CONFIG	, 	/*Modo captura de entrada: borda de descida*/
	TPM_IN_BOTH_EDGE_CONFIG		, 	/*Modo captura de entrada: borda de subida/descida*/
	TPM_PWM_HIGH_TRUE_CONFIG	,	/*Modos PWM: Pulso é o nível*/
	TPM_PWM_LOW_TRUE_CONFIG		,	/*Modos PWM: Pulso é o baixo*/
}tpmChConfig_t;

/*Possíveis valores de divisões do prescaler dos módulos TPM*/
typedef enum{
	TPM_PRESCALER_DIV_1		=	0x0, 	/*Prescaler do módulo TPM divide por 0*/
	TPM_PRESCALER_DIV_2		=	0x1,  	/*Prescaler do módulo TPM divide por 1*/
	TPM_PRESCALER_DIV_4		=	0x2,  	/*Prescaler do módulo TPM divide por 2*/
	TPM_PRESCALER_DIV_8		=	0x3,  	/*Prescaler do módulo TPM divide por 3*/
	TPM_PRESCALER_DIV_16	=	0x4,  	/*Prescaler do módulo TPM divide por 4*/
	TPM_PRESCALER_DIV_32	=	0x5,  	/*Prescaler do módulo TPM divide por 5*/
	TPM_PRESCALER_DIV_64	=	0x6,  	/*Prescaler do módulo TPM divide por 6*/
	TPM_PRESCALER_DIV_128	=	0x7,  	/*Prescaler do módulo TPM divide por 7*/
}tpmPrescalerValues_t;

/*Possíveis fontes de clock*/
typedef enum{
	TPM_CNT_CLOCK_DISABLE = 0U,
	TPM_CNT_CLOCK_FLL = 1U,
	TPM_CNT_CLOCK_OSC = 2U,
	TPM_CNT_CLOCK_IRC = 3U
}tpmClkSrc_t;

/*FIM: TIPOS ENUMERADOS*/
/*=======================================================================================*/


/*PROTÓTIPOS - FUNÇÕES INLINE*/
/*=======================================================================================*/

static inline void TPM_InitCounter(TPM_Type *base);
static inline void TPM_StopCounter(TPM_Type *base);
static inline void TPM_SetStopOnOverflow(TPM_Type *base);
static inline void TPM_SetContinueOnOverflow(TPM_Type *base);
static inline void TPM_SetCounterClkSrc(TPM_Type *base, tpmClkSrc_t src);
static inline uint16_t TPM_GetChValue(TPM_Type *base, uint8_t channel);
static inline void TPM_SetChMatch(TPM_Type *base, uint8_t channel, uint16_t match);
static inline void TPM_EnableIRQ(TPM_Type *base);
static inline uint32_t TPM_GetIRQFlag(TPM_Type *base);
static inline void TPM_ClearIRQFlag(TPM_Type *base);
static inline void TPM_SetModulo(TPM_Type *base, uint16_t modulo);

/*FIM: PROTÓTIPOS - FUNÇÕES INLINE*/
/*=======================================================================================*/

/*PROTÓTIPOS - FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Função		:	TPM_Init
 *
 * Descrição	:   Inicialização do driver do módulo TPM.
 *
 * Entradas		:   *base    - registrador base do periférico TPM;
 *                  modulo   - valor de final de contagem;
 *                  prescale - tipo enumerado com um dos possíveis
 *                             valores de prescaler.
 *
 * Saídas 		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
void TPM_Init(TPM_Type *base, uint16_t modulo, tpmPrescalerValues_t prescale);

/**********************************************************************
 * Função		:	TPM_InitChannel
 *
 * Descrição	:   Inicializa um canal do módulo TPM.
 *
 * Entradas		:   *base  - registrador base do periférico TPM;
 *                  chNum  - numéro do canal;
 *                  mode   - modo de operação do canal, sendo
 * 							 definidos pelo tipo TPM_chMode_t.
 * 					config - possíveisconfigurações de cada modo de
 * 							 operação do canal, sendo definidos pelo
 * 							 tipo TPM_chConfig_t.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
void TPM_InitChannel(TPM_Type *base, uint8_t chNum, tpmChMode_t mode, tpmChConfig_t config);


/*FIM: PROTÓTIPOS - FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/*FUNÇÕES INLINE*/
/*=======================================================================================*/

/**********************************************************************
 * Função		:	TPM_InitCounter
 *
 * Descrição	:   Ativa clock do contador para iniciar a contagem.
 *
 * Entradas		:   *base - registrador base do periférico TPM.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_InitCounter(TPM_Type *base)
{
	assert(base);
	base->SC |= (TPM_SC_CMOD(0x1U));
}

/**********************************************************************
 * Função		:	TPM_StopCounter
 *
 * Descrição	:   Desativa clock do contador para parar a contagem.
 *
 * Entradas		:   *base - registrador base do periférico TPM.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_StopCounter(TPM_Type *base)
{
	assert(base);
	base->SC &= ~(TPM_SC_CMOD(0x1U));
}

/**********************************************************************
 * Função		:	TPM_SetCounterClkSrc
 *
 * Descrição	:   Define a fonte de clock utilizada pelo contador.
 *
 * Entradas		:   *base - registrador base do periférico TPM;
 *                  src   - tipo enumerado que define a fonte de clock.
 *
 * Saídas 		:   STATUS_SUCCESS, se configuração ocorreu corretamente;
 *                  STATUS_FAIL, caso contrário.
 *
 * Comentários 	: 	Deve ser chamada antes da função TPM_Init.
 * ********************************************************************/
static inline void TPM_SetCounterClkSrc(TPM_Type *base, tpmClkSrc_t src)
{
	assert(base);

	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(src);
}

/**********************************************************************
 * Função		:	TPM_SetStopOnOverflow
 *
 * Descrição	:   Define o contador para parar a contagem quando ocorrer
 *                  estouro de contagem.
 *
 * Entradas		:   *base - registrador base do periférico TPM.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_SetStopOnOverflow(TPM_Type *base)
{
	assert(base);
	base->CONF |= (TPM_CONF_CSOO(0x1U));
}

/**********************************************************************
 * Função		:	TPM_SetContinueOnOverflow
 *
 * Descrição	:   Define o contador não parar a contagem quando ocorrer
 *                  estouro de contagem.
 *
 * Entradas		:   *base - registrador base do periférico TPM.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_SetContinueOnOverflow(TPM_Type *base)
{
	assert(base);
	base->CONF &= ~(TPM_CONF_CSOO(0x1U));
}

/**********************************************************************
 * Função		:	TPM_GetChValue
 *
 * Descrição	:   Retorna o valor do registrador contador no modo "Captura
 * 					de Entrada" ou o valor de "match" no modo "Comparação de Saída"
 * 					do canal 0 módulo TPM.
 *
 * Entradas		:   *base - registrador base do periférico TPM;
 *                  channel - valor do canal do TPM.
 *
 * Saídas		:   modulo: valor de 1 à 65535.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline uint16_t TPM_GetChValue(TPM_Type *base, uint8_t channel)
{
	assert(base);

	return (uint16_t)base->CONTROLS[channel].CnV;
}


/**********************************************************************
 * Função		:	TPM_SetChMatch
 *
 * Descrição	:    Define valor de "match" em canal do módulo TPM0.
 *
 * Entradas		:   *base - registrador base do periférico TPM;
 *                  channel - valor do canal do TPM;
 *                  match: valor de 2 à 65535.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_SetChMatch(TPM_Type *base, uint8_t channel, uint16_t match)
{
	assert(base);

	base->CONTROLS[channel].CnV = match;
}

/**********************************************************************
 * Função		:	TPM_EnableIRQ
 *
 * Descrição	:   Habilita a geração de interrupção pelo módulo TPM.
 *
 * Entradas		:   *base - registrador base do periférico TPM.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_EnableIRQ(TPM_Type *base)
{
   base->SC |= TPM_SC_TOIE_MASK;
}

/**********************************************************************
 * Função		:	TPM_GetIRQFlag
 *
 * Descrição	:   Retorna o valor da flag de interrupção gerada
 * 					pelo módulo TPM.
 *
 * Entradas		:   *base - registrador base do periférico TPM.
 *
 * Saídas		:   0	,  se não houve interrupção;
 * 					!0	,  se houve interrupção.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline uint32_t TPM_GetIRQFlag(TPM_Type *base)
{
   return base->SC & TPM_SC_TOF_MASK;
}

/**********************************************************************
 * Função		:	TPM_ClearIRQFlag
 *
 * Descrição	:   Limpa a flag de interrupção gerada pelo módulo TPM.
 *
 * Entradas		:   *base - registrador base do periférico TPM.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_ClearIRQFlag(TPM_Type *base)
{
	base->SC |= TPM_SC_TOF_MASK;
}

/**********************************************************************
 * Função		:	TPM_SetModulo
 *
 * Descrição	:   Define valor de fim de contagem do módulo do TPM.
 *
 * Entradas		:   *base - registrador base do periférico TPM;
 * 					modulo - valor de fim de contagem.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_SetModulo(TPM_Type *base, uint16_t modulo)
{
	assert(base);

	base->MOD = modulo;
}

/*FIM: FUNÇÕES INLINE*/
/*=======================================================================================*/

#if defined(__cplusplus)
}
#endif

#endif /* TPM_DRV_H_ */

/***************************************************************************************
 * FIM: Módulo - tpm.h
 ***************************************************************************************/

