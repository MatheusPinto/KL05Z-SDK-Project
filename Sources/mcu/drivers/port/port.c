/***************************************************************************************
 * Módulo      : port.c
 * Revisão     : 1.1
 * Data        : 24/05/2023
 * Descrição   : Arquivo com implementações dos Módulos PORT da Familia Kinetis KL05.
 * Comentários : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

/*ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

#include "port.h"

/*FIM: ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

/*FUNÇÕES PÚBLICAS*/
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
void PORT_Init(PORT_Type* base)
{
	SYSTEM_ASSERT(base);

	if( PORTA == base )
	{
		   SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; /* Ativa o clock do periférico PORTA*/
	}
	else
	{
		if( PORTB == base )
		{
			SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; /* Ativa o clock do periférico PORTB*/
			/*Desabilita NMI em PTB5 e habilita como GPIO*/
			base->PCR[5] &= ~PORT_PCR_MUX_MASK; /*Limpa configurações anteriores*/
			base->PCR[5] |= PORT_PCR_MUX(PORT_MUX_ALT1);
		}
	}
}

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
void PORT_EnableIRQ(PORT_Type *base, uint8_t pin, portIRQ_t irq)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	base->PCR[pin] &= ~PORT_PCR_IRQC_MASK; /*Limpa configurações anteriores*/
	base->PCR[pin] |= PORT_PCR_IRQC(irq);
}

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
uint32_t PORT_GetIRQFlag(PORT_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	return base->ISFR & ((uint32_t)1 << pin);
}

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
void PORT_ClearIRQFlag(PORT_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	base->ISFR = ( (uint32_t) 1 << pin );
}

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
void PORT_EnablePull(PORT_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	base->PCR[pin] |= PORT_PCR_PE_MASK;
}

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
void PORT_EnableHighStrength(PORT_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	base->PCR[pin] |= PORT_PCR_DSE_MASK;
}

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
void PORT_SetMux(PORT_Type *base, uint8_t pin, portMux_t mux)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	base->PCR[pin] &= ~PORT_PCR_MUX_MASK; /*Limpa configurações anteriores*/
	base->PCR[pin] |= PORT_PCR_MUX(mux);
}

/*FIM: FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/***************************************************************************************
 * FIM: Módulo - port.c
 ***************************************************************************************/
