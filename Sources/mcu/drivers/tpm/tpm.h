/***************************************************************************************
 * M�dulo      : tpm.h
 * Revis�o     : 1.1
 * Data        : 24/05/2023
 * Descri��o   : Arquivo com implementa��es dos M�dulos Timer/PWM (TPM) da Familia Kinetis KL05.
 * Coment�rios : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef TPM_DRV_H_
#define TPM_DRV_H_

/*ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

#include <mcu/common.h>

/*FIM: ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

#if defined(__cplusplus)
extern "C" {
#endif

/*TIPOS ENUMERADOS*/
/*=======================================================================================*/

/*Poss�veis modos de opera��o dos m�dulos TPM*/
typedef enum{
	TPM_OUT_COMPARE_MODE, 		/* Modo comparador de sa�da*/
	TPM_IN_COMPARE_MODE	,		/* Modo captura de entrada*/
	TPM_EDGE_PWM_MODE	,		/* Modo PWM alinhado a borda*/
	TPM_CENTER_PWM_MODE			/* Modo PWM alinhado ao centro */
}tpmChMode_t;

/*Poss�veis configura��es do modos de opera��o dos m�dulos TPM*/
typedef enum{
	TPM_OUT_TOGGLE_GONFIG		, 	/*Modo compara��o de sa�da: inverte l�gica de sa�da*/
	TPM_OUT_CLEAR_GONFIG		,  	/*Modo compara��o de sa�da: limpa sa�da*/
	TPM_OUT_SET_GONFIG			,  	/*Modo compara��o de sa�da: seta sa�da*/
	TPM_IN_RISING_EDGE_CONFIG	, 	/*Modo captura de entrada: borda de subida*/
	TPM_IN_FALLING_EDGE_CONFIG	, 	/*Modo captura de entrada: borda de descida*/
	TPM_IN_BOTH_EDGE_CONFIG		, 	/*Modo captura de entrada: borda de subida/descida*/
	TPM_PWM_HIGH_TRUE_CONFIG	,	/*Modos PWM: Pulso � o n�vel*/
	TPM_PWM_LOW_TRUE_CONFIG		,	/*Modos PWM: Pulso � o baixo*/
}tpmChConfig_t;

/*Poss�veis valores de divis�es do prescaler dos m�dulos TPM*/
typedef enum{
	TPM_PRESCALER_DIV_1		=	0x0, 	/*Prescaler do m�dulo TPM divide por 0*/
	TPM_PRESCALER_DIV_2		=	0x1,  	/*Prescaler do m�dulo TPM divide por 1*/
	TPM_PRESCALER_DIV_4		=	0x2,  	/*Prescaler do m�dulo TPM divide por 2*/
	TPM_PRESCALER_DIV_8		=	0x3,  	/*Prescaler do m�dulo TPM divide por 3*/
	TPM_PRESCALER_DIV_16	=	0x4,  	/*Prescaler do m�dulo TPM divide por 4*/
	TPM_PRESCALER_DIV_32	=	0x5,  	/*Prescaler do m�dulo TPM divide por 5*/
	TPM_PRESCALER_DIV_64	=	0x6,  	/*Prescaler do m�dulo TPM divide por 6*/
	TPM_PRESCALER_DIV_128	=	0x7,  	/*Prescaler do m�dulo TPM divide por 7*/
}tpmPrescalerValues_t;

/*Poss�veis fontes de clock*/
typedef enum{
	TPM_CNT_CLOCK_DISABLE = 0U,
	TPM_CNT_CLOCK_FLL = 1U,
	TPM_CNT_CLOCK_OSC = 2U,
	TPM_CNT_CLOCK_IRC = 3U
}tpmClkSrc_t;

/*FIM: TIPOS ENUMERADOS*/
/*=======================================================================================*/


/*PROT�TIPOS - FUN��ES INLINE*/
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

/*FIM: PROT�TIPOS - FUN��ES INLINE*/
/*=======================================================================================*/

/*PROT�TIPOS - FUN��ES P�BLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Fun��o		:	TPM_Init
 *
 * Descri��o	:   Inicializa��o do driver do m�dulo TPM.
 *
 * Entradas		:   *base    - registrador base do perif�rico TPM;
 *                  modulo   - valor de final de contagem;
 *                  prescale - tipo enumerado com um dos poss�veis
 *                             valores de prescaler.
 *
 * Sa�das 		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
void TPM_Init(TPM_Type *base, uint16_t modulo, tpmPrescalerValues_t prescale);

/**********************************************************************
 * Fun��o		:	TPM_InitChannel
 *
 * Descri��o	:   Inicializa um canal do m�dulo TPM.
 *
 * Entradas		:   *base  - registrador base do perif�rico TPM;
 *                  chNum  - num�ro do canal;
 *                  mode   - modo de opera��o do canal, sendo
 * 							 definidos pelo tipo TPM_chMode_t.
 * 					config - poss�veisconfigura��es de cada modo de
 * 							 opera��o do canal, sendo definidos pelo
 * 							 tipo TPM_chConfig_t.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
void TPM_InitChannel(TPM_Type *base, uint8_t chNum, tpmChMode_t mode, tpmChConfig_t config);


/*FIM: PROT�TIPOS - FUN��ES P�BLICAS*/
/*=======================================================================================*/

/*FUN��ES INLINE*/
/*=======================================================================================*/

/**********************************************************************
 * Fun��o		:	TPM_InitCounter
 *
 * Descri��o	:   Ativa clock do contador para iniciar a contagem.
 *
 * Entradas		:   *base - registrador base do perif�rico TPM.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_InitCounter(TPM_Type *base)
{
	SYSTEM_ASSERT(base);
	base->SC |= (TPM_SC_CMOD(0x1U));
}

/**********************************************************************
 * Fun��o		:	TPM_StopCounter
 *
 * Descri��o	:   Desativa clock do contador para parar a contagem.
 *
 * Entradas		:   *base - registrador base do perif�rico TPM.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_StopCounter(TPM_Type *base)
{
	SYSTEM_ASSERT(base);
	base->SC &= ~TPM_SC_CMOD_MASK;
}

/**********************************************************************
 * Fun��o		:	TPM_SetCounterClkSrc
 *
 * Descri��o	:   Define a fonte de clock utilizada pelo contador.
 *
 * Entradas		:   *base - registrador base do perif�rico TPM;
 *                  src   - tipo enumerado que define a fonte de clock.
 *
 * Sa�das 		:   STATUS_SUCCESS, se configura��o ocorreu corretamente;
 *                  STATUS_FAIL, caso contr�rio.
 *
 * Coment�rios 	: 	Deve ser chamada antes da fun��o TPM_Init.
 * ********************************************************************/
static inline void TPM_SetCounterClkSrc(TPM_Type *base, tpmClkSrc_t src)
{
	SYSTEM_ASSERT(base);

	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(src);
}

/**********************************************************************
 * Fun��o		:	TPM_SetStopOnOverflow
 *
 * Descri��o	:   Define o contador para parar a contagem quando ocorrer
 *                  estouro de contagem.
 *
 * Entradas		:   *base - registrador base do perif�rico TPM.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_SetStopOnOverflow(TPM_Type *base)
{
	SYSTEM_ASSERT(base);
	base->CONF |= (TPM_CONF_CSOO(0x1U));
}

/**********************************************************************
 * Fun��o		:	TPM_SetContinueOnOverflow
 *
 * Descri��o	:   Define o contador n�o parar a contagem quando ocorrer
 *                  estouro de contagem.
 *
 * Entradas		:   *base - registrador base do perif�rico TPM.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_SetContinueOnOverflow(TPM_Type *base)
{
	SYSTEM_ASSERT(base);
	base->CONF &= ~(TPM_CONF_CSOO(0x1U));
}

/**********************************************************************
 * Fun��o		:	TPM_GetChValue
 *
 * Descri��o	:   Retorna o valor do registrador contador no modo "Captura
 * 					de Entrada" ou o valor de "match" no modo "Compara��o de Sa�da"
 * 					do canal 0 m�dulo TPM.
 *
 * Entradas		:   *base - registrador base do perif�rico TPM;
 *                  channel - valor do canal do TPM.
 *
 * Sa�das		:   modulo: valor de 1 � 65535.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline uint16_t TPM_GetChValue(TPM_Type *base, uint8_t channel)
{
	SYSTEM_ASSERT(base);

	return (uint16_t)base->CONTROLS[channel].CnV;
}


/**********************************************************************
 * Fun��o		:	TPM_SetChMatch
 *
 * Descri��o	:    Define valor de "match" em canal do m�dulo TPM0.
 *
 * Entradas		:   *base - registrador base do perif�rico TPM;
 *                  channel - valor do canal do TPM;
 *                  match: valor de 2 � 65535.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_SetChMatch(TPM_Type *base, uint8_t channel, uint16_t match)
{
	SYSTEM_ASSERT(base);

	base->CONTROLS[channel].CnV = match;
}

/**********************************************************************
 * Fun��o		:	TPM_EnableIRQ
 *
 * Descri��o	:   Habilita a gera��o de interrup��o pelo m�dulo TPM.
 *
 * Entradas		:   *base - registrador base do perif�rico TPM.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_EnableIRQ(TPM_Type *base)
{
	SYSTEM_ASSERT(base);
	base->SC |= TPM_SC_TOIE_MASK;
}

/**********************************************************************
 * Fun��o		:	TPM_GetIRQFlag
 *
 * Descri��o	:   Retorna o valor da flag de interrup��o gerada
 * 					pelo m�dulo TPM.
 *
 * Entradas		:   *base - registrador base do perif�rico TPM.
 *
 * Sa�das		:   0	,  se n�o houve interrup��o;
 * 					!0	,  se houve interrup��o.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline uint32_t TPM_GetIRQFlag(TPM_Type *base)
{
	SYSTEM_ASSERT(base);
	return base->SC & TPM_SC_TOF_MASK;
}

/**********************************************************************
 * Fun��o		:	TPM_ClearIRQFlag
 *
 * Descri��o	:   Limpa a flag de interrup��o gerada pelo m�dulo TPM.
 *
 * Entradas		:   *base - registrador base do perif�rico TPM.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_ClearIRQFlag(TPM_Type *base)
{
	SYSTEM_ASSERT(base);
	base->SC |= TPM_SC_TOF_MASK;
}

/**********************************************************************
 * Fun��o		:	TPM_SetModulo
 *
 * Descri��o	:   Define valor de fim de contagem do m�dulo do TPM.
 *
 * Entradas		:   *base - registrador base do perif�rico TPM;
 * 					modulo - valor de fim de contagem.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_SetModulo(TPM_Type *base, uint16_t modulo)
{
	SYSTEM_ASSERT(base);

	base->MOD = modulo;
}

/**********************************************************************
 * Fun��o		:	TPM_GetModulo
 * 
 * Descri��o	:   Retorna o valor de final de contagem
 * 					do m�dulo TPM0.
 * Entradas		:   *base - registrador base do perif�rico TPM.
 * 
 * Sa�das		:   Valor de final de contagem 16bits
 * 
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline uint16_t TPM_GetModulo(TPM_Type *base)
{
	assert(base);
	return (uint16_t)base->MOD;
}

/**********************************************************************
 * Fun��o		:	TPM_SetFrequency
 * Descri��o	:   Configura a frequ�ncia de opera��o do m�dulo TPM.
 * Entradas		:   *base  - registrador base do perif�rico TPM;
 * 				freq   - frequ�ncia de opera��o desejada.
 * Sa�das		:   Nenhuma.
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
void TPM_SetFrequency(TPM_Type *base, uint32_t freq, uint8_t channel);

/**********************************************************************
 * Fun��o		:	TPM_GetClockFrequency
 * Descri��o	:   Retorna a frequencia utilizada para alimentar o timer.
 * Entradas		:   *base  - registrador base do perif�rico TPM;
 * Sa�das		:   Nenhuma.
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
uint32_t TPM_GetClockFrequency();

/**********************************************************************
 * Fun��o		:	TPM_SetPrescaler
 * 
 * Descri��o	:   Define o valor do prescaler do m�dulo TPM0.
 * 
 * Entradas		:   *base - registrador base do perif�rico TPM.
 * 					prescaler do tipo tpmPrescalerValues_t 
 * Sa�das		:   Nenhuma.
 * 
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void TPM_SetPrescaler(TPM_Type *base, tpmPrescalerValues_t prescaler)
{
	assert(base);

	/** Para o contador porque PS é somente leitura enquanto counter esta ligado */
	TPM_StopCounter(base);

	/*<! Limpa os bits de prescaler */
	base->SC &= ~TPM_SC_PS_MASK;
	/*<! Define o prescaler */
    base->SC |= TPM_SC_PS(prescaler);
	/*<! Limpa o contador */
	base->CNT = 0x00U;

	TPM_InitCounter(base);
}

/*FIM: FUN��ES INLINE*/
/*=======================================================================================*/

#if defined(__cplusplus)
}
#endif

#endif /* TPM_DRV_H_ */

/***************************************************************************************
 * FIM: M�dulo - tpm.h
 ***************************************************************************************/

