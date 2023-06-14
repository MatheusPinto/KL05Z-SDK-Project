/***************************************************************************************
 * Módulo      : port.h
 * Revisão     : 1.1
 * Data        : 24/05/2023
 * Descrição   : Arquivo com implementações dos Módulos PORT da Familia Kinetis KL05.
 * Comentários : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef PORT_DRV_H_
#define PORT_DRV_H_

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

/*Possíveis alternativas de multiplexação dos pinos das PORTs*/
typedef enum{
	PORT_MUX_ALT0 		= 0x0u,
	PORT_MUX_ALT1 		= 0x1u,
	PORT_MUX_ALT2 		= 0x2u,
	PORT_MUX_ALT3 		= 0x3u,
	PORT_MUX_DISABLE 	= PORT_MUX_ALT0
}portMux_t;

/*Possíveis alternativas de interrupção dos pinos das PORTs*/
typedef enum{
	PORT_IRQ_DISABLE 		 	 = 0x0U,
	PORT_IRQ_DMA_ON_RISING_EDGE  = 0x1U,
	PORT_IRQ_DMA_ON_FALLING_EDGE = 0x2U,
	PORT_IRQ_DMA_ON_EITHER_EDGE  = 0x3U,
	PORT_IRQ_ON_lOW_LEVEL		 = 0x8U,
	PORT_IRQ_ON_RISING_EDGE 	 = 0x9U,
	PORT_IRQ_ON_FALLING_EDGE 	 = 0xAU,
	PORT_IRQ_ON_EITHER_EDGE 	 = 0xBU,
	PORT_IRQ_ON_HIGH_LEVEL 	 	 = 0xCU
}portIRQ_t;

/*FIM: TIPOS ENUMERADOS*/
/*=======================================================================================*/

/*PROTOTIPOS - FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Função		:	PORT_Init
 *
 * Descrição	:   Deve ser chamado antes de utilizar um módulo PORT.
 *
 * Entradas		:   *base - registrador base do periférico PORT.
 *
 * Saídas 		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
void PORT_Init(PORT_Type* base);

/**********************************************************************
 * Função		:	PORT_EnableIRQ
 *
 * Descrição	:   Habilita a geração de interrupção pelo pino de
 * 					um módulo PORT.
 *
 * Entradas		:   *base - registrador base do periférico PORT.
 *                  pin	- numeração do pino;
 * 					irq - uma das alternativas de interrupção do pino.
 *
 * Saídas		:  Nenhuma.
 *
 * Comentários 	:  Nenhum.
 * ********************************************************************/
void PORT_EnableIRQ(PORT_Type *base, uint8_t pin, portIRQ_t irq);

/**********************************************************************
 * Função		:	PORT_GetIRQFlag
 *
 * Descrição	:   Retorna se a flag de interrupção de pino do PORT foi setada.
 *
 * Entradas		:   *base - registrador base do periférico PORT.
 *                  pin	- numeração do pino;
 *
 * Saídas		:  0 - se flag não setada;
 *                 !0 - se flag setada.
 *
 * Comentários 	:  Nenhum.
 * ********************************************************************/
uint32_t PORT_GetIRQFlag(PORT_Type *base, uint8_t pin);

/**********************************************************************
 * Função		:	PORT_ClearIRQFlag
 *
 * Descrição	:   Limpa flag de interrupção do pino de PORT.
 *
 * Entradas		:   *base - registrador base do periférico PORT.
 *                  pin	- numeração do pino;
 *
 * Saídas		:  Nenhuma.
 *
 * Comentários 	:  Nenhum.
 * ********************************************************************/
void PORT_ClearIRQFlag(PORT_Type *base, uint8_t pin);

/**********************************************************************
 * Função		:	PORTB_SetMux
 *
 * Descrição	:   Configura a multiplexação no pino do módulo PORTB.
 *
 * Entradas		:   *base - registrador base do periférico PORT.
 *                  pin	- numeração do pino;
 * 					mux - uma das alternativas de multiplexação do pino.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
void PORT_SetMux(PORT_Type *base, uint8_t pin, portMux_t mux);

/**********************************************************************
 * Função		:	PORT_EnablePull
 *
 * Descrição	:   Habilita pullUp no pino do módulo PORT.
 *
 * Entradas		:   *base - registrador base do periférico PORT.
 *                  pin	  - numeração do pino;
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
void PORT_EnablePull(PORT_Type *base, uint8_t pin);

/**********************************************************************
 * Função		:	PORT_EnableHighStrength
 *
 * Descrição	:   Habilita a intensidade da corrente do pino como alta.
 *
 * Entradas		:   *base - registrador base do periférico PORT.
 *                  pin	  - numeração do pino;
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
void PORT_EnableHighStrength(PORT_Type *base, uint8_t pin);

/**********************************************************************
 * Função		:	PORTB_SetMux
 *
 * Descrição	:   Configura a multiplexação no pino do módulo PORTB.
 *
 * Entradas		:   *base - registrador base do periférico PORT.
 *                  pin	- numeração do pino;
 * 					mux - uma das alternativas de multiplexação do pino.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
void PORT_SetMux(PORT_Type *base, uint8_t pin, portMux_t mux);

/*FIM: PROTOTIPOS - FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

#if defined(__cplusplus)
}
#endif

#endif /* PORT_DRV_H_ */

/***************************************************************************************
 * FIM: Módulo - port.h
 ***************************************************************************************/
