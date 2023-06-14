/***************************************************************************************
 * Módulo      : uart.c
 * Revisão     : 1.1
 * Data        : 24/05/2023
 * Descrição   : Arquivo com implementações dos Módulos UART da Familia Kinetis KL05.
 * Comentários : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/


/*ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

#include "uart.h"

/*FIM: ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

/*VÁRIAVEIS GLOBAIS PRIVADAS*/
/*=======================================================================================*/

static mcuClock_t g_uartClkSrc;

/*FIM: VÁRIAVEIS GLOBAIS PRIVADAS*/
/*=======================================================================================*/


/*FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Função		:	UART_SetClkSrc
 *
 * Descrição	:   Define a fonte de clock utilizada pelo UART.
 *
 * Entradas		:   *base - registrador base do periférico UART;
 * 					src   - fonte do clock definido em tipo
 * 					        enumerado.
 *
 * Saídas		:   STATUS_SUCCESS, se configuração ocorreu corretamente;
 *                  STATUS_FAIL, caso contrário.
 *
 * Comentários 	: 	Deve ser chamada antes da função UART_Init.
 * ********************************************************************/
uint8_t UART_SetClkSrc(UART0_Type * base, uartClkSrc_t src)
{
	switch(src)
	{
	case UART_CLOCK_DISABLE:
		g_uartClkSrc = 0U;
		break;
	case UART_CLOCK_FLL:
		g_uartClkSrc = MCG_FLL_CLK_FREQ;
		break;
	case UART_CLOCK_IRC:
		g_uartClkSrc = MCG_IRC_CLK_FREQ;
		break;
	case UART_CLOCK_OSC:
		g_uartClkSrc = OSC_ER_CLK_FREQ;
		break;
	default:
		return SYSTEM_STATUS_FAIL;
		break;
	}

	SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(src);

	return SYSTEM_STATUS_SUCCESS;
}

/**********************************************************************
 * Função		:	UART_Init
 *
 * Descrição	:   Inicializa o pino do módulo UART.
 *
 * Entradas		:   *base - registrador base do periférico UART;
 * 					baudrate - o baudarate;
 * 					enTxRx - Desabilita ou habilita Tx, Rx ou ambos;
 * 					parity - paridade par ou ímpar;
 * 					stopBitsN - número de bits de parada (1U ou 2U).
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Deve ser chamada após a função UART_SetClkSrc.
 * ********************************************************************/
void UART_Init( UART0_Type * base,
		        uint32_t baudRate,
				uartTxRxEnabling_t enTxRx,
				uartParity_t parity,
				uartStopBitNum_t stopBitsN)
{
	uint16_t sbr_field;

	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	/*Limpa configurações iniciais*/
	base->C1 = 0x00U;
	base->C2 = 0x00U;
	base->C3 = 0x00U;
	base->S2 = 0x00U;

	sbr_field  = g_uartClkSrc/(baudRate*16); 		/*Define o valor do campo SBR*/
	base->BDH |= UART0_BDH_SBR((sbr_field >> 8)); 		/*Coloca os MSB de SBR em UART0_BDH*/
	base->BDL  = UART0_BDL_SBR(sbr_field); 				/*Coloca os LSB de SBR em UART0_BDL*/

	base->BDH |= UART0_BDH_SBNS(stopBitsN);

	base->C2 |= enTxRx;

	UART_CleanRxBuffer(base); /*Limpa flags de inicialização de Rx*/

	base->C1 &= ~UART0_C1_PT_MASK; /*limpa configuração anterior de paridade*/
	base->C1 |= parity; /*Habilita paridade*/
}

/*FIM: FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/


/***************************************************************************************
 * FIM: Módulo - uart.c
 ***************************************************************************************/
