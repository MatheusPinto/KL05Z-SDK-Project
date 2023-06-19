/*******************************************************************************
 * Módulo      : i2c.h
 * Revisão     : 1.0
 * Data        : 19/06/2023
 * Descrição   : Implementação modulo I2C da Familia Kinetis KL05.
 * Comentários : Nenhum.
 * Autor(es)   : João Mário C. I. Lago
 ******************************************************************************/

#ifndef I2C_DRV_H_
#define I2C_DRV_H_

/** Headers */
/*============================================================================*/

/** Libraries */
#include <mcu/common.h>

/** For c++ compilers */
#if defined(__cplusplus)
extern "C" {
#endif

/** Functions Prototypes */
/*============================================================================*/

/**
 * Function: I2C_InitWithBaud
 * 
 * Description: Initializes the I2C module using a given baud rate provided in
 * "baud_rate" parameter as [bps]
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to be initialized
 *  - baud_rate -> Baud rate to initialize I2C module in [bps]
 * 
 * Outputs:
 *  - {uint8_t} -> SYSTEM_STATUS_SUCCESS if requested baud_rate in [bps]
 *                 was achievable otherwise SYSTEM_STATUS_FAIL
 */
uint8_t I2C_InitWithBaud(I2C_Type *base, uint32_t baud_rate);

/**
 * Function: I2C_InitManual
 * 
 * Description: Initializes I2C module using given ICR and MULT values
 * 
 * MULT values are:
 *  - 00 = mult 1
 *  - 01 = mult 2
 *  - 10 = mult 4
 * 
 * baud_rate = bus speed (Hz) / (mult * SCL divider)
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to be initialized
 *  - icr -> Valid ICR index to specify the SCL divider.
 *           All values can be found in page 622/623 of document bellow
 *           https://dl.btc.pl/kamami_wa/kl05_reference_manual.pdf
 *           Last access in April 4 - 2023
 *  - mult -> Valid MULT value
 *            All values can be found in page 607 of document bellow
 *            https://dl.btc.pl/kamami_wa/kl05_reference_manual.pdf
 *            Last access in April 4 - 2023
 * Outputs:
 *  - {void}
 */
void I2C_InitManual(I2C_Type *base, uint8_t icr, uint8_t mult);

/**
 * Function: I2C_Init
 * 
 * Description: Initializes the I2C module using the standard mode with a fixed
 * baud rate of 100 [kbps]
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to be initialized
 * 
 * Outputs:
 *  - {uint8_t}
 *          -> SYSTEM_STATUS_SUCCESS if 100 [kbps] baud rate was possible to be
 *             set otherwise SYSTEM_STATUS_FAIL in case of any error when trying
 *             to set this speed or if this speed was not possible to be set
 *             with current bus clock speed
 */
uint8_t I2C_Init(I2C_Type *base);

/**
 * Function: I2C_Init_400kbps
 * 
 * Description: Initializes the I2C module using the full speed with a fixed
 * baud rate of 400 [kbps]
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to be initialized
 * 
 * Outputs:
 *  - {uint8_t}
 *          -> SYSTEM_STATUS_SUCCESS if 400 [kbps] baud rate was possible to be set
 *             otherwise SYSTEM_STATUS_FAIL in case  of any error when trying to set
 *             this speed or if this speed was not possible to be set with current
 *             bus clock speed
 */
uint8_t I2C_Init_400kbps(I2C_Type *base);

/**
 * Function: I2C_GetRxAk
 * 
 * Description: Returns the RXAK bit from status register
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C to read RXAK bit
 * 
 * Outputs:
 *  - {uint8_t} -> 1 if RXAK is set otherwise 0
 */
static inline uint8_t I2C_GetRxAk(I2C_Type *base)
{
	return (base->S & I2C_S_RXAK_MASK) ? 1 : 0;
}

/**
 * Function: I2C_GetTcf
 * 
 * Description: Returns the TCF bit from status register
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C to read TCF bit
 * 
 * Outputs:
 *  - {uint8_t} -> 1 if TCF is set otherwise 0
 */
static inline uint8_t I2C_GetTcf(I2C_Type *base)
{
	return (base->S & I2C_S_TCF_MASK) ? 1 : 0;
}

/**
 * Function: I2C_GetArbLost
 * 
 * Description: Returns the ARBL bit from status register
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C to read ARBL bit
 * 
 * Outputs:
 *  - {uint8_t} -> 1 if ARBL is set otherwise 0
 */
static inline uint8_t I2C_GetArbLost(I2C_Type *base)
{
	return (base->S & I2C_S_ARBL_MASK) ? 1 : 0;
}

/**
 * Function: I2C_GetInterruptFlag
 * 
 * Description: Returns the IICIF bit from status register
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C to read IICIF bit
 * 
 * Outputs:
 *  - {uint8_t} -> 1 if IICIF is set otherwise 0
 */
static inline uint8_t I2C_GetInterruptFlag(I2C_Type *base)
{
	return (base->S & I2C_S_IICIF_MASK) ? 1 : 0;
}

/**
 * Function: I2C_EnableAck
 * 
 * Description: Enables the acknowledgment signal to be sent onto SDA during the
 * acknowledgment cycles for both master and slave receivers in a given I2C
 * module
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to have ACK enabled
 * 
 * Outputs:
 *  - {void}
 */
static inline void I2C_EnableAck(I2C_Type *base)
{
	base->C1 &= ~I2C_C1_TXAK_MASK;
}

/**
 * Function: I2C_DisableAck
 * 
 * Description: Disables the acknowledgment signal to be sent onto SDA during
 * the acknowledgment cycles for both master and slave receivers in a given I2C
 * module
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to have ACK disabled
 * 
 * Outputs:
 *  - {void}
 */
static inline void I2C_DisableAck(I2C_Type *base)
{
	base->C1 |= I2C_C1_TXAK_MASK;
}

/**
 * Function: I2C_SetRxMode
 * 
 * Description: Sets a given I2C module to operate in receiver mode
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to set as receiver
 * 
 * Outputs:
 *  - {void}
 */
static inline void I2C_SetRxMode(I2C_Type *base)
{
	base->C1 &= ~I2C_C1_TX_MASK;
}

/**
 * Function: I2C_SetTxMode
 * 
 * Description: Sets a given I2C module to operate in transmitter mode
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to set as transmitter
 * 
 * Outputs:
 *  - {void}
 */
static inline void I2C_SetTxMode(I2C_Type *base)
{
	base->C1 |= I2C_C1_TX_MASK;
}

/**
 * Function: I2C_RepeatedStart
 * 
 * Description: Generates a Repeated Start condition in a given I2C module
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to generate the Repeated Start
 *           condition
 * 
 * Outputs:
 *  - {void}
 */
static inline void I2C_RepeatedStart(I2C_Type *base)
{
	base->C1 |= I2C_C1_RSTA_MASK;
}

/**
 * Function: I2C_Start
 * 
 * Description: Generates a start condition in a given I2C module, performs two
 * actions in the module, sets the module as transmitter mode and master.
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to generate the start condition
 * 
 * Outputs:
 *  - {void}
 */
static inline void I2C_Start(I2C_Type *base)
{
	base->C1 |= I2C_C1_TX_MASK;
	base->C1 |= I2C_C1_MST_MASK;
}

/**
 * Function: I2C_Stop
 * 
 * Description: Generates a stop condition in a given I2C module, performs two
 * actions in the module, sets the module as receiver mode and slave.
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to generate the stop condition
 * 
 * Outputs:
 *  - {void}
 */
static inline void I2C_Stop(I2C_Type *base)
{
	base->C1 &= ~I2C_C1_MST_MASK;
	base->C1 &= ~I2C_C1_TX_MASK;
}

/**
 * Function: I2C_Wait
 * 
 * Description: Generates a dummy loop that checks I2C interrupt flag and awaits
 * till the interrupt flag raise or leaves the loop in worst case after about
 * 17ms
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to await
 * 
 * Outputs:
 *  - {i2cStatusCodes_t} -> One of possible scenarios
 *              SYSTEM_STATUS_SUCCESS
 *              SYSTEM_STATUS_TIMEOUT
 */
static inline uint8_t I2C_Wait(I2C_Type *base)
{
	/**
	 * Guess a number of retries based on system core clock to try to match
	 * a delay of about 43ms in worst case
	 * Pre loop and post loop steps are not take in account because are
	 * much smaller compared with the time in loop
	 * 
	 * loop (with conditionals) about: (18|20|22) [cycles]
	 * 
	 * time_taken = (retries * loop_cycles) / (cycles_per_second)
	 * 
	 * Initial time taken is fixed in: 8 / 256 = 31.25ms
	 * loop_cycles is fixed in 2^4: 16
	 * 
	 * retries = (time_taken * cycles_per_second) / loop_cycles
	 * retries = ((8 / 256) * cycles_per_second) / 16
	 * retries = (8 * (cycles_per_second >> 8)) >> 4
	 * retries = ((cycles_per_seconds >> 5)) >> 4
	 * retries = (cycles_per_seconds >> 9)
	 * 
	 * Now using real loop times possibilities due to taken or not in branches:
	 *  - 18 [cycles]: 35.15ms
	 *  - 20 [cycles]: 39.06ms
	 *  - 22 [cycles]: 42.96ms
	 */
	const uint32_t timeout_tries = (SystemCoreClock >> 9);

	for (uint32_t i = 0; (i < timeout_tries); ++i)
	{
		/** Check the interrupt flag */
		if (base->S & I2C_S_IICIF_MASK) {
			/** Clears the interrupt flag */
			base->S |= I2C_S_IICIF_MASK;
			/** Return success */
			return SYSTEM_STATUS_SUCCESS;
		}
	}

	/** Timeout */
	return SYSTEM_STATUS_TIMEOUT;
}

/**
 * Function: I2C_Pause
 * 
 * Description: Performs a small pause with a dummy loop, used to leave the
 * bus after sending a stop condition
 * 
 * Inputs: None
 * 
 * Outputs: {void}
 */
static inline void I2C_Pause()
{
	for (uint8_t i = 0; i < 50; ++i)
	{
		__asm("nop");
	}
}

/**
 * Function: I2C_WriteByte
 * 
 * Description: Writes a byte in data I/O register of a given I2C module
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C to write the data
 *  - data -> Byte tho be written in data I/O register
 * 
 * Outputs:
 *  - {void}
 */
static inline void I2C_WriteByte(I2C_Type *base, uint8_t data)
{
	base->D = (uint8_t)data;
}

/**
 * Function: I2C_ReadByte
 * 
 * Description: Reads a byte from data I/O register of a given I2C module
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C to read the data
 * 
 * Outputs:
 *  - {uint8_t} Data read 
 */
static inline uint8_t I2C_ReadByte(I2C_Type *base)
{
	return (uint8_t)(base->D);
}

/**
 * Function: I2C_WriteSlave
 * 
 * Description: Writes one data byte in a given slave
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to be initialized
 *  - slave_addr -> 7 bits slave address
 *  - data -> 1 data byte to be transferred
 * 
 * Outputs:
 *  - {uint8_t} -> One of possible scenarios
 *              SYSTEM_STATUS_SUCCESS
 *              SYSTEM_STATUS_TIMEOUT
 *              SYSTEM_STATUS_INVALID_ADDRESS
 *              SYSTEM_STATUS_INVALID_REGISTER
 *              SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_WriteSlave(
	I2C_Type *base, uint8_t slave_addr, uint8_t data
);

/**
 * Function: I2C_WriteSlaveChunk
 * 
 * Description: Writes N data byte in a given slave
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to be initialized
 *  - slave_addr -> 7 bits slave address
 *  - data -> N data byte to be transferred
 *  - data_size -> size of the data to be transferred
 * 
 * Outputs:
 *  - {uint8_t} -> One of possible scenarios
 *              SYSTEM_STATUS_SUCCESS
 *              SYSTEM_STATUS_TIMEOUT
 *              SYSTEM_STATUS_INVALID_ADDRESS
 *              SYSTEM_STATUS_INVALID_REGISTER
 *              SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_WriteSlaveChunk(
	I2C_Type *base, uint8_t slave_addr, const uint8_t* const data,
	uint16_t data_size
);

/**
 * Function: I2C_WriteRegister
 * 
 * Description: Writes one data byte in a given register of a slave
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to be initialized
 *  - slave_addr -> 7 bits slave address
 *  - register_addr -> 1 byte slave register address
 *  - data -> 1 data byte to be transferred
 * 
 * Outputs:
 *  - {uint8_t} -> One of possible scenarios
 *              SYSTEM_STATUS_SUCCESS
 *              SYSTEM_STATUS_TIMEOUT
 *              SYSTEM_STATUS_INVALID_ADDRESS
 *              SYSTEM_STATUS_INVALID_REGISTER
 *              SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_WriteRegister(
	I2C_Type *base, uint8_t slave_addr, uint8_t register_addr, uint8_t data
);

/**
 * Function: I2C_WriteRegisterChunk
 * 
 * Description: Writes N data byte in a given register of a slave
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to be initialized
 *  - slave_addr -> 7 bits slave address
 *  - register_addr -> 1 byte slave register address
 *  - data -> N data byte to be transferred
 *  - data_size -> size of the data to be transferred
 * 
 * Outputs:
 *  - {uint8_t} -> One of possible scenarios
 *              SYSTEM_STATUS_SUCCESS
 *              SYSTEM_STATUS_TIMEOUT
 *              SYSTEM_STATUS_INVALID_ADDRESS
 *              SYSTEM_STATUS_INVALID_REGISTER
 *              SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_WriteRegisterChunk(
	I2C_Type *base, uint8_t slave_addr, uint8_t register_addr,
	const uint8_t* const data, uint16_t data_size
);

/**
 * Function: I2C_ReadSlave
 * 
 * Description: Reads a data byte in a given slave
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to be initialized
 *  - slave_addr -> 7 bits slave address
 *  - result -> Pointer to destination where the result data will be stored
 * 
 * Outputs:
 *  - {uint8_t} -> One of possible scenarios
 *              SYSTEM_STATUS_SUCCESS
 *              SYSTEM_STATUS_TIMEOUT
 *              SYSTEM_STATUS_INVALID_ADDRESS
 *              SYSTEM_STATUS_INVALID_REGISTER
 *              SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_ReadSlave(
	I2C_Type *base, uint8_t slave_addr, uint8_t* const result
);

/**
 * Function: I2C_ReadSlaveChunk
 * 
 * Description: Reads N data byte from slave
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to be initialized
 *  - slave_addr -> 7 bits slave address
 *  - result -> Pointer to destination where the result data will be stored
 *  - result_size -> Number of bytes to be read from the slave
 * 
 * Outputs:
 *  - {uint8_t} -> One of possible scenarios
 *              SYSTEM_STATUS_SUCCESS
 *              SYSTEM_STATUS_TIMEOUT
 *              SYSTEM_STATUS_INVALID_ADDRESS
 *              SYSTEM_STATUS_INVALID_REGISTER
 *              SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_ReadSlaveChunk(
	I2C_Type *base, uint8_t slave_addr, uint8_t** const result,
	uint16_t result_size
);

/**
 * Function: I2C_ReadRegister
 * 
 * Description: Reads a data byte in a given register of a slave
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to be initialized
 *  - slave_addr -> 7 bits slave address
 *  - register_addr -> 1 byte slave register address
 *  - result -> Pointer to destination where the result data will be stored
 * 
 * Outputs:
 *  - {uint8_t} -> One of possible scenarios
 *              SYSTEM_STATUS_SUCCESS
 *              SYSTEM_STATUS_TIMEOUT
 *              SYSTEM_STATUS_INVALID_ADDRESS
 *              SYSTEM_STATUS_INVALID_REGISTER
 *              SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_ReadRegister(
	I2C_Type *base, uint8_t slave_addr, uint8_t register_addr,
	uint8_t* const result
);

/**
 * Function: I2C_ReadRegisterChunk
 * 
 * Description: Reads N data byte from registers of a slave
 * 
 * Inputs:
 *  - base -> I2C mem map of the I2C module to be initialized
 *  - slave_addr -> 7 bits slave address
 *  - register_addr -> 1 byte slave register address
 *  - result -> Pointer to destination where the result data will be stored
 *  - result_size -> Number of bytes to be read from the slave
 * 
 * Outputs:
 *  - {uint8_t} -> One of possible scenarios
 *              SYSTEM_STATUS_SUCCESS
 *              SYSTEM_STATUS_TIMEOUT
 *              SYSTEM_STATUS_INVALID_ADDRESS
 *              SYSTEM_STATUS_INVALID_REGISTER
 *              SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_ReadRegisterChunk(
	I2C_Type *base, uint8_t slave_addr, uint8_t register_addr,
	uint8_t** const result, uint16_t result_size
);

/** For c++ compilers */
#if defined(__cplusplus)
}
#endif

#endif /* SOURCES_I2C_H_ */
