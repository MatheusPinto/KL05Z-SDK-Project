/***************************************************************************************
 * @file i2c.c
 * @brief Implementation of the I2C module for the Kinetis KL05 family.
 * @date 19/06/2023
 * @author João Mário C. I. Lago
 ***************************************************************************************/

#include "i2c.h"

/** @addtogroup i2c driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * @def I2C_PRE_INIT
 * @brief Execute pre-initialization steps to start the I2C module.
 */
#define I2C_PRE_INIT						\
{											\
	/** Activates the clock on PTB5 */		\
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;		\
	/** Changes PTB3 to  ALT2 [I2C_SCL] */	\
	PORTB_PCR3 = PORT_PCR_MUX(0x2);			\
	/** Changes PtB4 to ALT2 [I2C_SDA] */	\
	PORTB_PCR4 = PORT_PCR_MUX(0x2);			\
	/** Activates clock on I2C module */	\
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;		\
}

/**
 * @def I2C_POST_INIT
 * @brief Execute post-initialization steps to start the I2C module.
 */
#define I2C_POST_INIT						\
{											\
	/** Activates I2C module */				\
	base->C1 = 1 << 7;						\
}

/**
 * @def I2C_START_SLAVE_TRANSMISSION(op)
 * @brief Start a transmission to a slave with a given operation.
 * @param op 0 if starting a write operation, 1 if starting a read operation.
 */
#define I2C_START_SLAVE_TRANSMISSION(op)									\
{																			\
	/**
	 * Puts the I2C module in transmitter and master mode and puts the Start
	 * condition on the bus
	 */																		\
	I2C_Start(base);														\
	/**
	 * Sends the 7 bits slave address and last bit as 0 signaling a write op
	 */																		\
	I2C_WriteByte(base, (uint8_t)((slave_addr << 1) | op));					\
	/** Waits and checks for timeout */										\
	if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;						\
	/** Checks for invalid address */										\
	if (I2C_GetRxAk(base)) return SYSTEM_STATUS_INVALID_ADDRESS;			\
}

/**
 * @def I2C_START_ADDRESS_TRANSMISSION
 * @brief Send a given register address after sending the slave address.
 */
#define I2C_START_REGISTER_TRANSMISSION										\
{																			\
	/** Sends the register address byte */									\
	I2C_WriteByte(base, register_addr);										\
	/** Waits and checks for timeout */										\
	if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;						\
	/** Checks for invalid register */										\
	if (I2C_GetRxAk(base)) return SYSTEM_STATUS_INVALID_REGISTER;			\
}

/**
 * @def I2C_REPEATED_START_CONDITION
 * @brief Execute all steps to put the I2C bus into a repeated start condition.
 */
#define I2C_REPEATED_START_CONDITION										\
{																			\
	/**
	 * Initiates the repeated start condition to avoid other master to get
	 * the bus and specify the slave that wants to be read
	 */																		\
	I2C_RepeatedStart(base);												\
	/**
	 * Sends the 7 bits slave address and last bit as 1 signaling a read op
	 */																		\
	I2C_WriteByte(base, (uint8_t)((slave_addr << 1) | 1));					\
	/** Waits and checks for timeout */										\
	if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;						\
	/** Checks for invalid address */										\
	if (I2C_GetRxAk(base)) return SYSTEM_STATUS_INVALID_ADDRESS;			\
}

/**
 * @brief Initializes the I2C module using a given baud rate.
 * 
 * @param base I2C mem map of the I2C module to be initialized.
 * @param baud_rate Baud rate to initialize I2C module in [bps].
 * 
 * @return SYSTEM_STATUS_SUCCESS if the requested baud rate in [bps] was achievable, otherwise SYSTEM_STATUS_FAIL.
 */
uint8_t I2C_InitWithBaud(I2C_Type *base, uint32_t baud_rate)
{
	/** Pre initialization steps */
	I2C_PRE_INIT

	/** ICR values from reference manual */
	const static uint8_t icr_indexes[] = {
		0x10U, 0x11U, 0x12U, 0x13U, 0x14U, 0x15U,
		0x19U, 0x16U, 0x1AU, 0x17U, 0x1CU, 0x1DU,
		0x1EU, 0x22U, 0x1FU, 0x23U, 0x24U, 0x25U,
		0x26U, 0x27U, 0x28U, 0x29U, 0x2AU, 0x2BU,
		0x2CU, 0x2DU, 0x2EU, 0x2FU, 0x30U, 0x31U,
		0x32U, 0x33U, 0x34U, 0x35U, 0x36U, 0x3AU,
		0x37U, 0x3BU, 0x3CU, 0x3DU, 0x3EU, 0x3FU
	};

	/** SCL divider for the aligned icr_index */
	const static uint16_t scl_dividers[] = {
		48U, 56U, 64U, 72U, 80U, 88U,
		96U, 104U, 112U, 128U, 144U, 160U,
		192U, 224U, 240U, 256U, 288U, 320U,
		384U, 480U, 320U, 384U, 448U, 512U,
		576U, 640U, 768U, 960U, 640U, 768U,
		896U, 1024U, 1152U, 1280U, 1536U, 1792U,
		1920U, 2048U, 2304U, 2560U, 3072U, 3840U
	};

	/**
	 * baud_rate = bus speed (Hz) / (mult * SCL divider)
	 * SCL divider = bus speed (Hz) / (mult * baud_rate)
	 */

	/** For multipliers 1, 2, 4 */
	for (uint8_t mult = 1; mult <= 4; mult *= 2)
	{
		const uint32_t scl_divider = SystemCoreClock / (baud_rate * mult);

		/** scl_divider must be greater of equal to minimum available divider */
		if (scl_divider < scl_dividers[0]) return SYSTEM_STATUS_FAIL;

		/** scl_divider is greater than all available dividers, increase mult */
		if (
			scl_divider > scl_dividers[
				(sizeof(scl_dividers) / sizeof(scl_dividers[0])) - 1
			]
		) continue;

		/** Searches for a valid SCL divider fit */
		for (uint8_t j = 0; j < sizeof(scl_dividers) / sizeof(scl_dividers[0]); ++j)
		{
			/** Valid configuration */
			if (scl_dividers[j] == scl_divider)
			{
				/**
				 * Applies the selected ICR and MULT
				 * 
				 * MULT values are:
				 *  - mask(00) = mult(1) = mult(0b001) = mask(mult(0b001) >> 1)
				 *  - mask(01) = mult(2) = mult(0b010) = mask(mult(0b010) >> 1)
				 *  - mask(10) = mult(4) = mult(0b100) = mask(mult(0b100) >> 1)
				 */
				base->F = I2C_F_ICR(icr_indexes[j]) | I2C_F_MULT(mult >> 1);

				/** Post initialization steps */
				I2C_POST_INIT

				return SYSTEM_STATUS_SUCCESS;
			}
		}
	}

	/** No configuration found */
	return SYSTEM_STATUS_FAIL;
}


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
uint8_t I2C_Init(I2C_Type *base)
{
	return I2C_InitWithBaud(base, 100000);
}


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
uint8_t I2C_Init_400kbps(I2C_Type *base)
{
	return I2C_InitWithBaud(base, 400000);
}


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
void I2C_InitManual(I2C_Type *base, uint8_t icr, uint8_t mult)
{
	/** Pre initialization steps */
	I2C_PRE_INIT

	/** Applies the selected ICR and MULT */
	base->F = I2C_F_ICR(icr) | I2C_F_MULT(mult);

	/** Post initialization steps */
	I2C_POST_INIT
}

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
)
{
	/** Starts Write operation */
	I2C_START_SLAVE_TRANSMISSION(0)

	/** Transmit the data */
	I2C_WriteByte(base, data);
	/** Waits and checks for timeout */
	if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;
	/** Checks transfer error */
	if (I2C_GetRxAk(base) || !I2C_GetTcf(base)) return SYSTEM_STATUS_TRANSFER_FAIL;

	/**
	 * Puts the I2C module in receiver and slave mode and puts the Stop
	 * condition on the bus
	 */
	I2C_Stop(base);

	/** Performs a small pause */
	I2C_Pause();

	return SYSTEM_STATUS_SUCCESS;
}

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
)
{
	/** Starts Write operation */
	I2C_START_SLAVE_TRANSMISSION(0)

	for (uint16_t i = 0; i < data_size; ++i)
	{
		/** Transmit the data */
		I2C_WriteByte(base, data[i]);
		/** Waits and checks for timeout */
		if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;
		/** Checks transfer error */
		if (I2C_GetRxAk(base) || !I2C_GetTcf(base)) return SYSTEM_STATUS_TRANSFER_FAIL;
	}

	/**
	 * Puts the I2C module in receiver and slave mode and puts the Stop
	 * condition on the bus
	 */
	I2C_Stop(base);

	/** Performs a small pause */
	I2C_Pause();

	return SYSTEM_STATUS_SUCCESS;
}

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
)
{
	/** Starts Write operation */
	I2C_START_SLAVE_TRANSMISSION(0)
	I2C_START_REGISTER_TRANSMISSION

	/** Transmit the data */
	I2C_WriteByte(base, data);
	/** Waits and checks for timeout */
	if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;
	/** Checks transfer error */
	if (I2C_GetRxAk(base) || !I2C_GetTcf(base)) return SYSTEM_STATUS_TRANSFER_FAIL;

	/**
	 * Puts the I2C module in receiver and slave mode and puts the Stop
	 * condition on the bus
	 */
	I2C_Stop(base);

	/** Performs a small pause */
	I2C_Pause();

	return SYSTEM_STATUS_SUCCESS;
}

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
)
{
	/** Starts Write operation */
	I2C_START_SLAVE_TRANSMISSION(0)
	I2C_START_REGISTER_TRANSMISSION

	for (uint16_t i = 0; i < data_size; ++i)
	{
		/** Transmit the data */
		I2C_WriteByte(base, data[i]);
		/** Waits and checks for timeout */
		if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;
		/** Checks transfer error */
		if (I2C_GetRxAk(base) || !I2C_GetTcf(base)) return SYSTEM_STATUS_TRANSFER_FAIL;
	}

	/**
	 * Puts the I2C module in receiver and slave mode and puts the Stop
	 * condition on the bus
	 */
	I2C_Stop(base);

	/** Performs a small pause */
	I2C_Pause();

	return SYSTEM_STATUS_SUCCESS;
}

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
)
{
		/** Starts Write operation */
	I2C_START_SLAVE_TRANSMISSION(1)

	/** Initiates the repeated start condition */
	I2C_REPEATED_START_CONDITION

	/** Changes to receiver mode */
	I2C_SetRxMode(base);
	/**
	 * Disable ACK bit signaling to slave to not send more data after this
	 * byte be received
	 */
	I2C_DisableAck(base);

	/** Performs the dummy read */
	*result = I2C_ReadByte(base);
	/** Waits and checks for timeout */
	if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;

	/** Sends a I2C stop condition onto the bus */
	I2C_Stop(base);
	/** Performs actual read of the byte */
	*result = I2C_ReadByte(base);

	return SYSTEM_STATUS_SUCCESS;
}

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
)
{
	/** Starts Write operation */
	I2C_START_SLAVE_TRANSMISSION(1)

	/** Initiates the repeated start condition */
	I2C_REPEATED_START_CONDITION

	/** Changes to receiver mode */
	I2C_SetRxMode(base);
	/**
	 * Enable ACK bit signaling to slave to send more data after this byte
	 * be received
	 */
	I2C_EnableAck(base);

	/** Performs the dummy read */
	*result[0] = I2C_ReadByte(base);
	/** Waits and checks for timeout */
	if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;

	uint16_t i;
	for (i = 0; i < result_size - 2; ++i)
	{
		/** Reads one byte */
		*result[i] = I2C_ReadByte(base);
		/** Waits and checks for timeout */
		if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;
	}
	/**
	 * Disable ACK bit signaling to slave to not send more data after this
	 * byte be received
	 */
	I2C_DisableAck(base);
	/** Reads one byte */
	*result[i++] = I2C_ReadByte(base);
	/** Waits and checks for timeout */
	if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;

	/** Sends a I2C stop condition onto the bus */
	I2C_Stop(base);
	/** Read last byte */
	*result[i] = I2C_ReadByte(base);

	return SYSTEM_STATUS_SUCCESS;
}

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
)
{
	/** Starts Write operation */
	I2C_START_SLAVE_TRANSMISSION(1)
	I2C_START_REGISTER_TRANSMISSION

	/** Initiates the repeated start condition */
	I2C_REPEATED_START_CONDITION

	/** Changes to receiver mode */
	I2C_SetRxMode(base);
	/**
	 * Disable ACK bit signaling to slave to not send more data after this
	 * byte be received
	 */
	I2C_DisableAck(base);

	/** Performs the dummy read */
	*result = I2C_ReadByte(base);
	/** Waits and checks for timeout */
	if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;

	/** Sends a I2C stop condition onto the bus */
	I2C_Stop(base);
	/** Performs actual read of the byte */
	*result = I2C_ReadByte(base);

	return SYSTEM_STATUS_SUCCESS;
}

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
)
{
	/** Starts Write operation */
	I2C_START_SLAVE_TRANSMISSION(1)
	I2C_START_REGISTER_TRANSMISSION

	/** Initiates the repeated start condition */
	I2C_REPEATED_START_CONDITION

	/** Changes to receiver mode */
	I2C_SetRxMode(base);
	/**
	 * Enable ACK bit signaling to slave to send more data after this byte
	 * be received
	 */
	I2C_EnableAck(base);

	/** Performs the dummy read */
	*result[0] = I2C_ReadByte(base);
	/** Waits and checks for timeout */
	if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;

	uint16_t i;
	for (i = 0; i < result_size - 2; ++i)
	{
		/** Reads one byte */
		*result[i] = I2C_ReadByte(base);
		/** Waits and checks for timeout */
		if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;
	}
	/**
	 * Disable ACK bit signaling to slave to not send more data after this
	 * byte be received
	 */
	I2C_DisableAck(base);
	/** Reads one byte */
	*result[i++] = I2C_ReadByte(base);
	/** Waits and checks for timeout */
	if (I2C_Wait(base)) return SYSTEM_STATUS_TIMEOUT;

	/** Sends a I2C stop condition onto the bus */
	I2C_Stop(base);
	/** Read last byte */
	*result[i] = I2C_ReadByte(base);

	return SYSTEM_STATUS_SUCCESS;
}

/*! @}*/
