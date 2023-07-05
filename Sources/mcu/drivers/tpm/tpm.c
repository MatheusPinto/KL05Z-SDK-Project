/***************************************************************************************
 * M�dulo      : tpm.c
 * Revis�o     : 1.1
 * Data        : 24/05/2023
 * Descri��o   : Arquivo com implementa��es dos M�dulos Timer/PWM (TPM) da Familia Kinetis KL05.
 * Coment�rios : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

/*ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

#include "tpm.h"

/*FIM: ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

/*FUN��ES P�BLICAS*/
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
 * Coment�rios 	: 	Deve ser chamada ap�s a fun��o TPM_SetCounterClkSrc.
 * ********************************************************************/
void TPM_Init(TPM_Type *base, uint16_t modulo, tpmPrescalerValues_t prescale)
{
	SYSTEM_ASSERT(base);

	if (TPM0 == base)
	{
		SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; /* Ativa o clock do m�dulo TPM0 */
	}
	else
	{
		if (TPM1 == base)
		{
			SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK; /* Ativa o clock do m�dulo TPM1 */
		}
	}

	base->MOD = modulo;

    base->SC &= ~TPM_SC_PS_MASK;
    base->SC |= TPM_SC_PS(prescale);

    base->CNT = 0x00U;					/* Reseta registrador contador */
}


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
void TPM_InitChannel(TPM_Type *base, uint8_t chNum, tpmChMode_t mode, tpmChConfig_t config){

	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( chNum >=0 ) && (chNum < 7) );

	switch(mode)
	{
		case(TPM_OUT_COMPARE_MODE):
			base->SC &= ~TPM_SC_CPWMS_MASK; /*N�o � Center PWM*/
			if(config == TPM_OUT_TOGGLE_GONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | TPM_CnSC_ELSA_MASK);
			else if(config == TPM_OUT_CLEAR_GONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK);
			else if(config == TPM_OUT_SET_GONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | (TPM_CnSC_ELSA_MASK) | TPM_CnSC_ELSA_MASK);
			break;
		case(TPM_IN_COMPARE_MODE):
				base->SC &= ~TPM_SC_CPWMS_MASK; /*N�o � Center PWM*/
			if(config == TPM_IN_RISING_EDGE_CONFIG)
				base->CONTROLS[chNum].CnSC = TPM_CnSC_ELSA_MASK;
			else if(config == TPM_IN_FALLING_EDGE_CONFIG)
				base->CONTROLS[chNum].CnSC = TPM_CnSC_ELSB_MASK;
			else
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSA_MASK);
			break;
		case(TPM_EDGE_PWM_MODE):
		base->SC &= ~TPM_SC_CPWMS_MASK; /*N�o � Center PWM*/
			if(config == TPM_PWM_HIGH_TRUE_CONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
			else
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
			break;
		case(TPM_CENTER_PWM_MODE):
				base->SC |= TPM_SC_CPWMS_MASK; /*� Center PWM*/
			if(config == TPM_PWM_HIGH_TRUE_CONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
			else
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
			break;
		default:
			break;
	}
}

/**********************************************************************
 * Fun��o		:	TPM_SetFrequency
 * Descri��o	:   Configura a frequ�ncia de opera��o do m�dulo TPM.
 * Entradas		:   *base  - registrador base do perif�rico TPM;
 * 				freq   - frequ�ncia de opera��o desejada.
 * Sa�das		:   Nenhuma.
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
void TPM_SetFrequency(TPM_Type *base, uint32_t freq, uint8_t channel)
{
	assert(base);
	assert(freq >= 0);

	/*!< Recebe a frequencia do clock base do timer.*/
	uint32_t clock_freq = TPM_GetClockFrequency();

	/*!< Calcula o valor do valor maximo do timer.*/
	uint32_t modulo = (uint32_t)(((clock_freq / (freq))) - 1);

	/*!< Divide o modulo pelo prescaler enquanto o modulo é maior que o valor maximo do timer */
	uint8_t prescaler = 0;
	while (modulo > 0xFFFF)
	{
		modulo = modulo >> 1;
		prescaler++;
		
		/*!< Verifica se o prescaler tem mais de 3bits */
		if (prescaler >> 4)
		{
			modulo = 0xFFFF;
			prescaler = 0b111;
			break;
		}
	}

	uint16_t old_module = TPM_GetModulo(base);
	uint16_t ch_value = TPM_GetChValue(base, 2);

	/*!< Configura o modulo e o prescaler do timer */
	TPM_SetModulo(base, modulo);
	TPM_SetPrescaler(base, prescaler);

	/*!< Atualiza o valor do pwm do canal 2 */
	uint16_t new_ch_value = (uint16_t)((ch_value * modulo) / old_module);
	TPM_SetChMatch(base, channel, new_ch_value);
}

/**********************************************************************
 * Fun��o		:	TPM_GetClockFrequency
 * Descri��o	:   Retorna a frequencia utilizada para alimentar o timer.
 * Entradas		:   *base  - registrador base do perif�rico TPM;
 * Sa�das		:   Nenhuma.
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
uint32_t TPM_GetClockFrequency()
{
	uint32_t value = ((SIM->SOPT2 & SIM_SOPT2_TPMSRC_MASK) >> SIM_SOPT2_TPMSRC_SHIFT);

	switch(value)
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

/*FIM: FUN��ES P�BLICAS*/
/*=======================================================================================*/

/***************************************************************************************
 * FIM: M�dulo - tpm.c
 ***************************************************************************************/
