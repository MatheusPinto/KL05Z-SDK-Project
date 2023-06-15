/***************************************************************************************
 * Módulo      : gpio.h
 * Revisão     : 1.1
 * Data        : 24/05/2023
 * Descrição   : Arquivo com implementações dos Módulos GPIO da Familia Kinetis KL05.
 * Comentários : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef GPIO_DRV_H_
#define GPIO_DRV_H_

/*ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

#include <mcu/common.h>
#include <mcu/drivers/port/port.h>

/*FIM: ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

#if defined(__cplusplus)
extern "C" {
#endif

/*PROTÓTIPOS - FUNÇÕES INLINE*/
/*=======================================================================================*/

static inline void GPIO_SetPin(GPIO_Type *base, uint8_t pin);
static inline void GPIO_ClearPin(GPIO_Type *base, uint8_t pin);
static inline void GPIO_TogglePin(GPIO_Type *base, uint8_t pin);
static inline void GPIO_WritePin(GPIO_Type *base, uint8_t pin, uint8_t value);
static inline void GPIO_SetPinsMask(GPIO_Type *base, uint32_t mask);
static inline void GPIO_ClearPinsMask(GPIO_Type *base, uint32_t mask);
static inline uint8_t GPIO_ReadPin(GPIO_Type *base, uint8_t pin);

/*FIM: PROTÓTIPOS - FUNÇÕES INLINE*/
/*=======================================================================================*/

/*PROTOTIPOS - FUNÇÕES PÚBLICAS*/
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
void GPIO_InitInputPin(GPIO_Type *base, uint8_t pin);

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
void GPIO_InitOutputPin(GPIO_Type *base, uint8_t pin, uint8_t initVal);

/*FIM: PROTOTIPOS - FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/*FUNÇÕES INLINE*/
/*=======================================================================================*/

/**********************************************************************
 * Função		:	GPIO_SetPinValue
 *
 * Descrição	:   Coloca o valor lógico 1 no pino do módulo GPIO.
 *
 * Entradas		:   *base - registrador base do periférico GPIO;
 *                  pin   - numeração do pino.
 *
 * Saídas		:  	Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void GPIO_SetPin(GPIO_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	base->PSOR = (1 << pin);
}

/**********************************************************************
 * Função		:	GPIO_ClearPinValue
 *
 * Descrição	:   Coloca o valor lógico 0 no pino do módulo GPIO.
 *
 * Entradas		:   *base - registrador base do periférico GPIO;
 *                  pin   - numeração do pino.
 *
 * Saídas		:  	Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void GPIO_ClearPin(GPIO_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	base->PCOR = (1 << pin);
}

/**********************************************************************
 * Função		:	GPIO_TogglePin
 *
 * Descrição	:   Inverte o valor lógico no pino do módulo GPIO.
 *
 * Entradas		:   *base - registrador base do periférico GPIO;
 *                  pin   - numeração do pino.
 *
 * Saídas		:  	Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void GPIO_TogglePin(GPIO_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	base->PTOR = (1 << pin);
}

/**********************************************************************
 * Função		:	GPIO_ReadPin
 *
 * Descrição	:   Retorna o valor do pino do módulo GPIO.
 *
 * Entradas		:   *base - registrador base do periférico GPIO;
 *                  pin   - numeração do pino.
 *
 * Saídas		:  	0,  valor lógico 0;
 * 					1,  valor lógico 1.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline uint8_t GPIO_ReadPin(GPIO_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	return (uint8_t)((base->PDIR & (((uint32_t)1) << pin)) != 0);
}

/**********************************************************************
 * Função		:	GPIO_WritePin
 *
 * Descrição	:   Escreve o valor no pino do módulo GPIO.
 *
 * Entradas		:   *base - registrador base do periférico GPIO;
 *                  pin   - numeração do pino;
 *                  value - valor lógico (0 ou 1) para enviar.
 *
 * Saídas		:  	Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void GPIO_WritePin(GPIO_Type *base, uint8_t pin, uint8_t value)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	if(pin)
	{
		base->PDOR |= 1 << pin;
	}
	else
	{
		base->PDOR &= ~(1 << pin);
	}
}

/**********************************************************************
 * Função		:	GPIO_SetPinsMask
 *
 * Descrição	:   Seta os pinos do módulo GPIO correspondentes ao bit 1
 *                  na máscara.
 *
 * Entradas		:   *base - registrador base do periférico GPIO;
 *                  mask - máscara de bits.
 *
 * Saídas		:  	Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void GPIO_SetPinsMask(GPIO_Type *base, uint32_t mask)
{
	SYSTEM_ASSERT(base);

	base->PSOR = mask;
}

/**********************************************************************
 * Função		:	GPIO_ClearPinsMask
 *
 * Descrição	:   Limpa os pinos do módulo GPIO correspondentes ao bit 1
 *                  na máscara.
 *
 * Entradas		:   *base - registrador base do periférico GPIO;
 *                  mask - máscara de bits.
 *
 * Saídas		:  	Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
static inline void GPIO_ClearPinsMask(GPIO_Type *base, uint32_t mask)
{
	SYSTEM_ASSERT(base);

	base->PCOR = mask;
}

/*FIM: FUNÇÕES INLINE*/
/*=======================================================================================*/

#if defined(__cplusplus)
}
#endif

#endif /* GPIO_DRV_H_ */

/***************************************************************************************
 * FIM: Módulo - gpio.h
 ***************************************************************************************/
