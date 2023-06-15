/***************************************************************************************
 * Módulo      : uart.h
 * Revisão     : 1.1
 * Data        : 24/05/2023
 * Descrição   : Arquivo com implementações dos Módulos UART da Familia Kinetis KL05.
 * Comentários : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef UART_DRV_H_
#define UART_DRV_H_

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

/*Possíveis combinações para habilitar Tx e Rx*/
typedef enum{
	UART_TX_RX_DISABLE = 0U, 					/* Desabilita Tx e Rx*/
	UART_TX_ENABLE = UART0_C2_TE_MASK,			/* Habilita Tx*/
	UART_RX_ENABLE = UART0_C2_RE_MASK,			/* Habilita Rx*/
	UART_TX_RX_ENABLE =
			UART0_C2_TE_MASK | UART0_C2_RE_MASK	/* Habilita Tx e Rx*/
}uartTxRxEnabling_t;

/*Possíveis métodos de paridade*/
typedef enum{
	UART_NO_PARITY = 0U, 						 /* Desabilita Tx e Rx*/
	UART_PARITY_EVEN =
			UART0_C1_PE_MASK | UART0_C2_TE_MASK, /* Habilita Tx*/
	UART_PARITY_ODD =
			UART0_C1_PE_MASK | UART0_C2_RE_MASK, /* Habilita Rx*/
}uartParity_t;

/*Possíveis quantidade de bits de parada*/
typedef enum{
	UART_ONE_STOP_BIT = 0U, 						 /* 1 bit de para*/
	UART_TWO_STOP_BIT 						 /* 12 bits de para*/
}uartStopBitNum_t;


/*Possíveis fontes de clock para o módulo UART*/
typedef enum{
	UART_CLOCK_DISABLE = 0U,
	UART_CLOCK_FLL = 1U,
	UART_CLOCK_OSC = 2U,
	UART_CLOCK_IRC = 3U
}uartClkSrc_t;

/*FIM: TIPOS ENUMERADOS*/
/*=======================================================================================*/


/*PROTOTIPOS - FUNÇÕES INLINE*/
/*=======================================================================================*/

static inline uint8_t UART_IsRxAvailable(UART0_Type * base);
static inline uint8_t UART_IsTxAvailable(UART0_Type * base);
static inline void UART_Write(UART0_Type * base, uint8_t data);
static inline uint8_t UART_Read(UART0_Type * base);
static inline void UART_EnableRxIRQ(UART0_Type * base);
static inline void UART_DisableTxIRQ(UART0_Type * base);
static inline void UART_CleanRxBuffer(UART0_Type * base);
static inline uint8_t UART_IsRxOverrun(UART0_Type * base);
static inline uint8_t UART_IsFramingError(UART0_Type * base);
static inline uint8_t UART_IsParityError(UART0_Type * base);

/*FIM: PROTOTIPOS - FUNÇÕES INLINE*/
/*=======================================================================================*/


/*PROTOTIPOS - FUNÇÕES PÚBLICAS*/
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
uint8_t UART_SetClkSrc(UART0_Type * base, uartClkSrc_t src);

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
				uartStopBitNum_t stopBitsN);

/*FIM: PROTOTIPOS - FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/


/*FUNÇÕES INLINE*/
/*=======================================================================================*/

/**********************************************************************
 * Função		:	UART_IsRxOverrun
 *
 * Descrição	:   Verifica se houve erro de Overrun na recepção dos dados.
 *
 * Entradas		:   *base - registrador base do periférico UART.
 *
 * Saídas		:   0 : não houve Overrun;
 * 					1: houve Overrun.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline uint8_t UART_IsRxOverrun(UART0_Type * base)
{
	SYSTEM_ASSERT(base);
	return (uint8_t)((base->S1 & UART0_S1_OR_MASK) != 0);
}

/**********************************************************************
 * Função		:	UART_IsFramingError
 *
 * Descrição	:   Verifica se houve erro de Framing na recepção dos dados.
 *
 * Entradas		:   *base - registrador base do periférico UART.
 *
 * Saídas		:   0 : não houve erro;
 * 					1 : houve erro.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline uint8_t UART_IsFramingError(UART0_Type * base)
{
	SYSTEM_ASSERT(base);
	return (uint8_t)((UART0_S1 & UART0_S1_FE_MASK) != 0);
}

/**********************************************************************
 * Função		:	UART_IsParityError
 *
 * Descrição	:   Verifica se houve erro de paridade na recepção dos dados.
 *
 * Entradas		:   *base - registrador base do periférico UART.
 *
 * Saídas		:   0 : não houve erro;
 * 					1 : houve erro.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline uint8_t UART_IsParityError(UART0_Type * base)
{
	SYSTEM_ASSERT(base);
	return (uint8_t)((UART0_S1 & UART0_S1_PF_MASK) != 0);
}

/**********************************************************************
 * Função		:	UART_CleanRxBuffer
 *
 * Descrição	:   Limpa flags de erro e buffer de recepção.
 *
 * Entradas		:   *base - registrador base do periférico UART.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_CleanRxBuffer(UART0_Type * base)
{
	SYSTEM_ASSERT(base);
	UART_IsRxAvailable(base);  /*Lê o registrador UART_S1*/
	UART_Read(base);
}

/**********************************************************************
 * Função		:	UART_Deinit
 *
 * Descrição	:   Desabilita o módulo UART.
 *
 * Entradas		:   *base - registrador base do periférico UART.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_Deinit(UART0_Type * base)
{
	SYSTEM_ASSERT(base);

	SIM_SCGC4 &= ~SIM_SCGC4_UART0_MASK;

	NVIC_DisableIRQ(UART0_IRQn);
}

/**********************************************************************
 * Função		:	UART_IsRxAvailable
 *
 * Descrição	:   Verifica se há dados para serem lidos do buffer Rx.
 *
 * Entradas		:   *base - registrador base do periférico UART.
 *
 * Saídas		:   0: não há dados para serem lidos;
 * 					1: existem dados não lidos no buffer.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline uint8_t UART_IsRxAvailable(UART0_Type * base)
{
	SYSTEM_ASSERT(base);
	return (uint8_t)((UART0_S1 & UART0_S1_RDRF_MASK) != 0);
}

/**********************************************************************
 * Função		:	UART_IsTxAvailable
 *
 * Descrição	:   Verifica se o buffer Tx está disponível para escrita.
 *
 * Entradas		:   *base - registrador base do periférico UART.
 *
 * Saídas		:   0: buffer Tx não disponível;
 * 					1: buffer Tx disponível para escrita.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline uint8_t UART_IsTxAvailable(UART0_Type * base)
{
	SYSTEM_ASSERT(base);
	return (uint8_t)((UART0_S1 & UART0_S1_TDRE_MASK) != 0);
}

/**********************************************************************
 * Função		:	UART_Write
 *
 * Descrição	:   Envia um dado de 8 bits pelo módulo UART0.
 *
 * Entradas		:   *base - registrador base do periférico UART;
 * 					data  - valor de 8 bits.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_Write(UART0_Type * base, uint8_t data)
{
	SYSTEM_ASSERT(base);
	UART0_D = data;
}

/**********************************************************************
 * Função		:	UART_Read
 *
 * Descrição	:   Retorna um dado de 8 bits pelo módulo UART.
 *
 * Entradas		:   *base - registrador base do periférico UART.
 *
 * Saídas		:   valor de 8 bits.
 *
 * Comentários 	: 	- Se não foi enviado valor novo para o RX, será lido o último
 * 					  valor recebido.
 * 					- Utilizar a função UART0_IsRxAvailable para
 * 					  verificar se foi enviado novo dado.
 * ********************************************************************/
static inline uint8_t UART_Read(UART0_Type * base)
{
	SYSTEM_ASSERT(base);
	return (uint8_t)UART0_D;
}

/**********************************************************************
 * Função		:	UART_EnableTxIRQ
 *
 * Descrição	:   Habilita interrupção de envio de dado completo
 * 					pelo módulo UART.
 *
 * Entradas		:   *base - registrador base do periférico UART.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_EnableTxIRQ(UART0_Type * base)
{
	SYSTEM_ASSERT(base);
	UART0_C2 |= UART0_C2_TIE_MASK;
}

/**********************************************************************
 * Função		:	UART_EnableRxIRQ
 *
 * Descrição	:   Habilita interrupção de recepção de dado completo
 * 					pelo módulo UART.
 *
 * Entradas		:   *base - registrador base do periférico UART.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_EnableRxIRQ(UART0_Type * base)
{
	SYSTEM_ASSERT(base);
	/*Habilita interrupção de recepção completa, overrun e framing error*/
	UART0_C2 |= UART0_C2_RIE_MASK;
	UART0_C3 |= UART0_C3_ORIE_MASK | UART0_C3_FEIE_MASK;
}

/**********************************************************************
 * Função		:	UART_DisableTxIRQ
 *
 * Descrição	:   Desabilita interrupção de envio de dado completo
 * 					pelo módulo UART.
 *
 * Entradas		:   *base - registrador base do periférico UART.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_DisableTxIRQ(UART0_Type * base)
{
	SYSTEM_ASSERT(base);
	UART0_C2 &= ~UART0_C2_TIE_MASK;
}

/**********************************************************************
 * Função		:	UART_DisableRxIRQ
 *
 * Descrição	:   Desabilita interrupção de recepção de dado completo
 * 					pelo módulo UART.
 *
 * Entradas		:   *base - registrador base do periférico UART.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_DisableRxIRQ(UART0_Type * base)
{
	SYSTEM_ASSERT(base);
	UART0_C2 &= ~UART0_C2_RIE_MASK;
}

/*FIM: FUNÇÕES INLINE*/
/*=======================================================================================*/

#if defined(__cplusplus)
}
#endif

#endif /*UART_DRV_H_*/

/***************************************************************************************
 * FIM: Módulo - uart.h
 ***************************************************************************************/
