/***************************************************************************************
 * @file i2c.h
 * @brief Implementation of the I2C module for the Kinetis KL05 family.
 * @date 19/06/2023
 * @author João Mário C. I. Lago
 ***************************************************************************************/

#ifndef I2C_DRV_H_
#define I2C_DRV_H_

#include <common.h>

/** For c++ compilers */
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @addtogroup uart driver
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Returns the RXAK bit from the status register.
 * 
 * @param base I2C mem map of the I2C module to read RXAK bit.
 * 
 * @return 1 if RXAK is set, otherwise 0.
 */
static inline uint8_t I2C_GetRxAk(I2C_Type *base);

/**
 * @brief Returns the TCF bit from the status register.
 * 
 * @param base I2C mem map of the I2C module to read TCF bit.
 * 
 * @return 1 if TCF is set, otherwise 0.
 */
static inline uint8_t I2C_GetTcf(I2C_Type *base);

/**
 * @brief Returns the ARBL bit from the status register.
 * 
 * @param base I2C mem map of the I2C module to read ARBL bit.
 * 
 * @return 1 if ARBL is set, otherwise 0.
 */
static inline uint8_t I2C_GetArbLost(I2C_Type *base);

/**
 * @brief Returns the IICIF bit from the status register.
 * 
 * @param base I2C mem map of the I2C module to read IICIF bit.
 * 
 * @return 1 if IICIF is set, otherwise 0.
 */
static inline uint8_t I2C_GetInterruptFlag(I2C_Type *base);

/**
 * @brief Enables the acknowledgment signal to be sent onto SDA during acknowledgment cycles.
 * 
 * @param base I2C mem map of the I2C module to have ACK enabled.
 */
static inline void I2C_EnableAck(I2C_Type *base);

/**
 * @brief Disables the acknowledgment signal to be sent onto SDA during acknowledgment cycles.
 * 
 * @param base I2C mem map of the I2C module to have ACK disabled.
 */
static inline void I2C_DisableAck(I2C_Type *base);

/**
 * @brief Sets a given I2C module to operate in receiver mode.
 * 
 * @param base I2C mem map of the I2C module to set as a receiver.
 */
static inline void I2C_SetRxMode(I2C_Type *base);

/**
 * @brief Sets a given I2C module to operate in transmitter mode.
 * 
 * @param base I2C mem map of the I2C module to set as a transmitter.
 */
static inline void I2C_SetTxMode(I2C_Type *base);

/**
 * @brief Generates a Repeated Start condition in a given I2C module.
 * 
 * @param base I2C mem map of the I2C module to generate the Repeated Start condition.
 */
static inline void I2C_RepeatedStart(I2C_Type *base);

/**
 * @brief Generates a start condition in a given I2C module and sets it as a transmitter in master mode.
 * 
 * @param base I2C mem map of the I2C module to generate the start condition.
 */
static inline void I2C_Start(I2C_Type *base);

/**
 * @brief Generates a stop condition in a given I2C module and sets it as a receiver in slave mode.
 * 
 * @param base I2C mem map of the I2C module to generate the stop condition.
 */
static inline void I2C_Stop(I2C_Type *base);

/**
 * @brief Waits for the I2C interrupt flag to be set. Leaves the loop if not set within approximately 17ms.
 * 
 * @param base I2C mem map of the I2C module to await.
 * 
 * @return SYSTEM_STATUS_SUCCESS if the flag was set, or SYSTEM_STATUS_TIMEOUT if it was not set within the timeout.
 */
static inline uint8_t I2C_Wait(I2C_Type *base);

/**
 * @brief Performs a small pause with a dummy loop, used to leave the bus after sending a stop condition.
 */
static inline void I2C_Pause();

/**
 * @brief Writes a byte to the data I/O register of a given I2C module.
 *
 * @param base I2C mem map of the I2C module to write the data.
 * @param data Byte to be written in the data I/O register.
 */
static inline void I2C_WriteByte(I2C_Type *base, uint8_t data);

/**
 * @brief Reads a byte from the data I/O register of a given I2C module.
 *
 * @param base I2C mem map of the I2C module to read the data.
 *
 * @return Data read from the I2C module.
 */
static inline uint8_t I2C_ReadByte(I2C_Type *base);

/**
 * @brief Initializes the I2C module using a given baud rate.
 *
 * @param base I2C mem map of the I2C module to be initialized.
 * @param baud_rate Baud rate to initialize I2C module in [bps].
 *
 * @return SYSTEM_STATUS_SUCCESS if the requested baud rate in [bps] was achievable, otherwise SYSTEM_STATUS_FAIL.
 */
uint8_t I2C_InitWithBaud(I2C_Type *base, uint32_t baud_rate);

/**
 * @brief Initializes the I2C module using the standard mode with a fixed baud rate of 100 [kbps].
 *
 * @param base I2C mem map of the I2C module to be initialized.
 *
 * @return \a SYSTEM_STATUS_SUCCESS if 100 [kbps] baud rate was set successfully.
 *         \a SYSTEM_STATUS_FAIL in case of any error when trying
 *             to set this speed or if this speed was not possible to be set
 *             with current bus clock speed.
 */
uint8_t I2C_Init(I2C_Type *base);

/**
 * @brief Initializes the I2C module using the full speed with a fixed baud rate of 400 [kbps].
 *
 * @param base I2C mem map of the I2C module to be initialized.
 *
 * @return \a SYSTEM_STATUS_SUCCESS if 400 [kbps] baud rate was set successfully.
 *         \a SYSTEM_STATUS_FAIL in case  of any error when trying to set
 *             this speed or if this speed was not possible to be set with current
 *             bus clock speed.
 */
uint8_t I2C_Init_400kbps(I2C_Type *base);

/**
 * @brief Initializes the I2C module using given ICR and MULT values.
 *
 * @param base I2C mem map of the I2C module to be initialized.
 * @param icr Valid ICR index to specify the SCL divider.
 *           All values can be found in page 622/623 of document bellow
 *           https://dl.btc.pl/kamami_wa/kl05_reference_manual.pdf
 *           Last access in April 4 - 2023.
 * @param mult Valid MULT value
 *            All values can be found in page 607 of document bellow
 *            https://dl.btc.pl/kamami_wa/kl05_reference_manual.pdf
 *            Last access in April 4 - 2023
 *
 * @note MULT values are:
 *         \a 00 = mult 1
 *         \a 01 = mult 2
 *         \a 10 = mult 4
 *
 * @note baud_rate = bus speed (Hz) / (mult * SCL divider)
 *
 */
void I2C_InitManual(I2C_Type *base, uint8_t icr, uint8_t mult);

/**
 *
 * @brief Writes one data byte in a given slave
 *
 * @param base, I2C mem map of the I2C module to be initialized
 * @param slave,_addr 7 bits slave address
 * @param data, byte to be transferred
 *
 * @return
 *          \a SYSTEM_STATUS_SUCCESS
 *          \a SYSTEM_STATUS_TIMEOUT
 *          \a SYSTEM_STATUS_INVALID_ADDRESS
 *          \a SYSTEM_STATUS_INVALID_REGISTER
 *          \a SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_WriteSlave(
	I2C_Type *base, uint8_t slave_addr, uint8_t data
);

/**
 * @brief Writes N data byte in a given slave
 *
 * @param base I2C mem map of the I2C module to be initialized
 * @param slave_addr 7 bits slave address
 * @param data N data byte to be transferred
 * @param data_size size of the data to be transferred
 *
 * @return
 *          \a SYSTEM_STATUS_SUCCESS
 *          \a SYSTEM_STATUS_TIMEOUT
 *          \a SYSTEM_STATUS_INVALID_ADDRESS
 *          \a SYSTEM_STATUS_INVALID_REGISTER
 *          \a SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_WriteSlaveChunk(
	I2C_Type *base, uint8_t slave_addr, const uint8_t* const data,
	uint16_t data_size
);

/**
 * @brief Writes one data byte in a given register of a slave
 *
 * @param base I2C mem map of the I2C module to be initialized
 * @param slave_addr 7 bits slave address
 * @param register_addr 1 byte slave register address
 * @param data 1 data byte to be transferred
 *
 * @return
 *         \a SYSTEM_STATUS_SUCCESS
 *         \a SYSTEM_STATUS_TIMEOUT
 *         \a SYSTEM_STATUS_INVALID_ADDRESS
 *         \a SYSTEM_STATUS_INVALID_REGISTER
 *         \a SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_WriteRegister(
	I2C_Type *base, uint8_t slave_addr, uint8_t register_addr, uint8_t data
);

/**
 * @brief I2C_WriteRegisterChunk
 *
 * @param base I2C mem map of the I2C module to be initialized
 * @param slave_addr 7 bits slave address
 * @param register_addr 1 byte slave register address
 * @param data N data byte to be transferred
 * @param data_size size of the data to be transferred
 *
 * @return
 *         \a SYSTEM_STATUS_SUCCESS
 *         \a SYSTEM_STATUS_TIMEOUT
 *         \a SYSTEM_STATUS_INVALID_ADDRESS
 *         \a SYSTEM_STATUS_INVALID_REGISTER
 *         \a SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_WriteRegisterChunk(
	I2C_Type *base, uint8_t slave_addr, uint8_t register_addr,
	const uint8_t* const data, uint16_t data_size
);

/**
 * @brief Reads a data byte in a given slave
 *
 * @param base I2C mem map of the I2C module to be initialized
 * @param slave_addr 7 bits slave address
 * @param result Pointer to destination where the result data will be stored
 *
 * @return
 *         \a SYSTEM_STATUS_SUCCESS
 *         \a SYSTEM_STATUS_TIMEOUT
 *         \a SYSTEM_STATUS_INVALID_ADDRESS
 *         \a SYSTEM_STATUS_INVALID_REGISTER
 *         \a SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_ReadSlave(
	I2C_Type *base, uint8_t slave_addr, uint8_t* const result
);

/**
 * @brief Reads N data byte from slave
 *
 * @param base I2C mem map of the I2C module to be initialized
 * @param slave_addr 7 bits slave address
 * @param result Pointer to destination where the result data will be stored
 * @param result_size Number of bytes to be read from the slave
 *
 * @return
 *         \a SYSTEM_STATUS_SUCCESS
 *         \a SYSTEM_STATUS_TIMEOUT
 *         \a SYSTEM_STATUS_INVALID_ADDRESS
 *         \a SYSTEM_STATUS_INVALID_REGISTER
 *         \a SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_ReadSlaveChunk(
	I2C_Type *base, uint8_t slave_addr, uint8_t** const result,
	uint16_t result_size
);

/**
 * brief Reads a data byte in a given register of a slave
 *
 * @param base  I2C mem map of the I2C module to be initialized
 * @param slave_addr 7 bits slave address
 * @param register_addr 1 byte slave register address
 * @param result Pointer to destination where the result data will be stored
 *
 * @return
 *         \a SYSTEM_STATUS_SUCCESS
 *         \a SYSTEM_STATUS_TIMEOUT
 *         \a SYSTEM_STATUS_INVALID_ADDRESS
 *         \a SYSTEM_STATUS_INVALID_REGISTER
 *         \a SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_ReadRegister(
	I2C_Type *base, uint8_t slave_addr, uint8_t register_addr,
	uint8_t* const result
);

/**
 * @brief Reads N data byte from registers of a slave
 *
 * @param base I2C mem map of the I2C module to be initialized
 * @param slave_addr 7 bits slave address
 * @param register_addr 1 byte slave register address
 * @param result Pointer to destination where the result data will be stored
 * @param result_size Number of bytes to be read from the slave
 *
 * @return
 *         \a SYSTEM_STATUS_SUCCESS
 *         \a SYSTEM_STATUS_TIMEOUT
 *         \a SYSTEM_STATUS_INVALID_ADDRESS
 *         \a SYSTEM_STATUS_INVALID_REGISTER
 *         \a SYSTEM_STATUS_TRANSFER_FAIL
 */
uint8_t I2C_ReadRegisterChunk(
	I2C_Type *base, uint8_t slave_addr, uint8_t register_addr,
	uint8_t** const result, uint16_t result_size
);

/*******************************************************************************
 * Code
 ******************************************************************************/

/**********************************************************************************/
static inline uint8_t I2C_GetRxAk(I2C_Type *base)
{
	return (base->S & I2C_S_RXAK_MASK) ? 1 : 0;
}

/**********************************************************************************/
static inline uint8_t I2C_GetTcf(I2C_Type *base)
{
	return (base->S & I2C_S_TCF_MASK) ? 1 : 0;
}

/**********************************************************************************/
static inline uint8_t I2C_GetArbLost(I2C_Type *base)
{
	return (base->S & I2C_S_ARBL_MASK) ? 1 : 0;
}

/**********************************************************************************/
static inline uint8_t I2C_GetInterruptFlag(I2C_Type *base)
{
	return (base->S & I2C_S_IICIF_MASK) ? 1 : 0;
}

/**********************************************************************************/
static inline void I2C_EnableAck(I2C_Type *base)
{
	base->C1 &= ~I2C_C1_TXAK_MASK;
}

/**********************************************************************************/
static inline void I2C_DisableAck(I2C_Type *base)
{
	base->C1 |= I2C_C1_TXAK_MASK;
}

/**********************************************************************************/
static inline void I2C_SetRxMode(I2C_Type *base)
{
	base->C1 &= ~I2C_C1_TX_MASK;
}

/**********************************************************************************/
static inline void I2C_SetTxMode(I2C_Type *base)
{
	base->C1 |= I2C_C1_TX_MASK;
}

/**********************************************************************************/
static inline void I2C_RepeatedStart(I2C_Type *base)
{
	base->C1 |= I2C_C1_RSTA_MASK;
}

/**********************************************************************************/
static inline void I2C_Start(I2C_Type *base)
{
	base->C1 |= I2C_C1_TX_MASK;
	base->C1 |= I2C_C1_MST_MASK;
}

/**********************************************************************************/
static inline void I2C_Stop(I2C_Type *base)
{
	base->C1 &= ~I2C_C1_MST_MASK;
	base->C1 &= ~I2C_C1_TX_MASK;
}

/**********************************************************************************/
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

/**********************************************************************************/
static inline void I2C_Pause()
{
	for (uint8_t i = 0; i < 50; ++i)
	{
		__asm("nop");
	}
}

/**********************************************************************************/
static inline void I2C_WriteByte(I2C_Type *base, uint8_t data)
{
	base->D = (uint8_t)data;
}

/**********************************************************************************/
static inline uint8_t I2C_ReadByte(I2C_Type *base)
{
	return (uint8_t)(base->D);
}

/*! @}*/

/** For c++ compilers */
#if defined(__cplusplus)
}
#endif

#endif /* I2C_DRV_H_ */
