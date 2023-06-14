/***************************************************************************************
 * Módulo      : gpio.c
 * Revisão     : 1.1
 * Data        : 24/05/2023
 * Descrição   : Arquivo com implementações dos Módulos GPIO da Familia Kinetis KL05.
 * Comentários : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

/*ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

#include "gpio.h"

/*FIM: ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

/*FUNÇÕES PRIVADAS*/
/*=======================================================================================*/

/**********************************************************************
 * Função		:	SetPortAsGPIO
 *
 * Descrição	:   Realiza a multiplexação do pino do PORT como GPIO.
 *
 * Entradas		:   *base - registrador base do periférico GPIO;
 *                  pin   - numeração do pino.
 *
 * Saídas		:  	Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static void SetPortAsGPIO(GPIO_Type *base, uint8_t pin)
{
	if ( base == GPIOA )
	{
		PORT_SetMux(PORTA, pin, PORT_MUX_ALT1);
	}
	else
	{
		if ( base == GPIOB )
		{
			PORT_SetMux(PORTB, pin, PORT_MUX_ALT1);
		}
	}
}

/*FIM - FUNÇÕES PRIVADAS*/
/*=======================================================================================*/

/*FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Função		:	GPIO_InitInputPin
 *
 * Descrição	:   Inicializa pino de módulo GPIO como entrada.
 *
 * Entradas		:   *base - registrador base do periférico GPIO;
 *                  pin   - numeração do pino.
 *
 * Saídas		:  	Nenhuma.
 *
 * Comentários 	: 	Essa função também realiza a multiplexação do
 *                  do pino do PORT relacionado ao GPIO.
 * ********************************************************************/
void GPIO_InitInputPin(GPIO_Type *base, uint8_t pin)
{
	SetPortAsGPIO(base, pin);
	base->PDDR &= ~(1 << pin);
}

/**********************************************************************
 * Função		:	GPIO_InitOutputPin
 *
 * Descrição	:   Inicializa pino de módulo GPIO como saída.
 *
 * Entradas		:   *base - registrador base do periférico GPIO;
 *                  pin   - numeração do pino;
 *                  initVal - valor lógico inicial do pino (0 ou !0).
 *
 * Saídas		:  	Nenhuma.
 *
 * Comentários 	: 	Essa função também realiza a multiplexação do
 *                  do pino do PORT relacionado ao GPIO.
 * ********************************************************************/
void GPIO_InitOutputPin(GPIO_Type *base, uint8_t pin, uint8_t initVal)
{
	   SetPortAsGPIO(base, pin);
	   base->PDDR |= (1 << pin);
	   if(initVal == 0)
	   {
		   base->PCOR = 1 << pin;
	   }
	   else
	   {
		   base->PSOR = 1 << pin;
	   }
}

/*FIM: FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/***************************************************************************************
 * FIM: Módulo - gpio.c
 ***************************************************************************************/
