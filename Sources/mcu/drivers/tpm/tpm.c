/***************************************************************************************
 * Módulo      : tpm.c
 * Revisão     : 1.1
 * Data        : 24/05/2023
 * Descrição   : Arquivo com implementações dos Módulos Timer/PWM (TPM) da Familia Kinetis KL05.
 * Comentários : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

/*ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

#include "tpm.h"

/*FIM: ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

/*FUNÇÕES PÚBLICAS*/
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
 * Comentários 	: 	Deve ser chamada após a função TPM_SetCounterClkSrc.
 * ********************************************************************/
void TPM_Init(TPM_Type *base, uint16_t modulo, tpmPrescalerValues_t prescale)
{
	assert(base);

	if (TPM0 == base)
	{
		SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; /* Ativa o clock do módulo TPM0 */
	}
	else
	{
		if (TPM1 == base)
		{
			SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK; /* Ativa o clock do módulo TPM1 */
		}
	}

	base->MOD = modulo;

    base->SC &= ~TPM_SC_PS_MASK;
    base->SC |= TPM_SC_PS(prescale);

    base->CNT = 0x00U;					/* Reseta registrador contador */
}


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
void TPM_InitChannel(TPM_Type *base, uint8_t chNum, tpmChMode_t mode, tpmChConfig_t config){

	assert(base);
	assert( ( chNum >=0 ) && (chNum < 7) );

	switch(mode)
	{
		case(TPM_OUT_COMPARE_MODE):
			base->SC &= ~TPM_SC_CPWMS_MASK; /*Não é Center PWM*/
			if(config == TPM_OUT_TOGGLE_GONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | TPM_CnSC_ELSA_MASK);
			else if(config == TPM_OUT_CLEAR_GONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK);
			else if(config == TPM_OUT_SET_GONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | (TPM_CnSC_ELSA_MASK) | TPM_CnSC_ELSA_MASK);
			break;
		case(TPM_IN_COMPARE_MODE):
				base->SC &= ~TPM_SC_CPWMS_MASK; /*Não é Center PWM*/
			if(config == TPM_IN_RISING_EDGE_CONFIG)
				base->CONTROLS[chNum].CnSC = TPM_CnSC_ELSA_MASK;
			else if(config == TPM_IN_FALLING_EDGE_CONFIG)
				base->CONTROLS[chNum].CnSC = TPM_CnSC_ELSB_MASK;
			else
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSA_MASK);
			break;
		case(TPM_EDGE_PWM_MODE):
		base->SC &= ~TPM_SC_CPWMS_MASK; /*Não é Center PWM*/
			if(config == TPM_PWM_HIGH_TRUE_CONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
			else
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
			break;
		case(TPM_CENTER_PWM_MODE):
				base->SC |= TPM_SC_CPWMS_MASK; /*É Center PWM*/
			if(config == TPM_PWM_HIGH_TRUE_CONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
			else
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
			break;
		default:
			break;
	}
}

/*FIM: FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/***************************************************************************************
 * FIM: Módulo - tpm.c
 ***************************************************************************************/
