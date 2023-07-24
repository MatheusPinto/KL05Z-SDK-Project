# I2C Driver for KL05Z Microcontroller

## Description
The I2C driver is a software module that provides functions to initialize and control the I2C module of the KL05Z microcontroller. The driver allows communication with I2C devices connected to the microcontroller.

## Specifications

### Functions

1. `uint8_t I2C_InitWithBaud(I2C_Type *base, uint32_t baud_rate)`
   - Description: Initializes the I2C module using a given baud rate.
   - Inputs:
     - `base`: I2C mem map of the I2C module to be initialized.
     - `baud_rate`: Baud rate to initialize I2C module in [bps].
   - Outputs:
     - Returns `SYSTEM_STATUS_SUCCESS` if the requested baud rate was achievable; otherwise, returns `SYSTEM_STATUS_FAIL`.

2. `void I2C_InitManual(I2C_Type *base, uint8_t icr, uint8_t mult)`
   - Description: Initializes the I2C module using given ICR and MULT values.
   - Inputs:
     - `base`: I2C mem map of the I2C module to be initialized.
     - `icr`: Valid ICR index to specify the SCL divider.
     - `mult`: Valid MULT value.
   - Outputs: None.

3. `uint8_t I2C_Init(I2C_Type *base)`
   - Description: Initializes the I2C module using the standard mode with a fixed baud rate of 100 [kbps].
   - Inputs:
     - `base`: I2C mem map of the I2C module to be initialized.
   - Outputs:
     - Returns `SYSTEM_STATUS_SUCCESS` if 100 [kbps] baud rate was possible to be set; otherwise, returns `SYSTEM_STATUS_FAIL`.

4. `uint8_t I2C_Init_400kbps(I2C_Type *base)`
   - Description: Initializes the I2C module using the full speed with a fixed baud rate of 400 [kbps].
   - Inputs:
     - `base`: I2C mem map of the I2C module to be initialized.
   - Outputs:
     - Returns `SYSTEM_STATUS_SUCCESS` if 400 [kbps] baud rate was possible to be set; otherwise, returns `SYSTEM_STATUS_FAIL`.

5. `static inline uint8_t I2C_GetRxAk(I2C_Type *base)`
   - Description: Returns the RXAK bit from the status register.
   - Inputs:
     - `base`: I2C mem map of the I2C to read RXAK bit.
   - Outputs:
     - Returns 1 if RXAK is set; otherwise, returns 0.

6. `static inline uint8_t I2C_GetTcf(I2C_Type *base)`
   - Description: Returns the TCF bit from the status register.
   - Inputs:
     - `base`: I2C mem map of the I2C to read TCF bit.
   - Outputs:
     - Returns 1 if TCF is set; otherwise, returns 0.

7. `static inline uint8_t I2C_GetArbLost(I2C_Type *base)`
   - Description: Returns the ARBL bit from the status register.
   - Inputs:
     - `base`: I2C mem map of the I2C to read ARBL bit.
   - Outputs:
     - Returns 1 if ARBL is set; otherwise, returns 0.

8. `static inline uint8_t I2C_GetInterruptFlag(I2C_Type *base)`
   - Description: Returns the IICIF bit from the status

 register.
   - Inputs:
     - `base`: I2C mem map of the I2C to read IICIF bit.
   - Outputs:
     - Returns 1 if IICIF is set; otherwise, returns 0.

9. `static inline void I2C_EnableAck(I2C_Type *base)`
   - Description: Enables the acknowledgment signal to be sent onto SDA during the acknowledgment cycles for both master and slave receivers in a given I2C module.
   - Inputs:
     - `base`: I2C mem map of the I2C module to have ACK enabled.
   - Outputs: None.

10. `static inline void I2C_DisableAck(I2C_Type *base)`
    - Description: Disables the acknowledgment signal to be sent onto SDA during the acknowledgment cycles for both master and slave receivers in a given I2C module.
    - Inputs:
      - `base`: I2C mem map of the I2C module to have ACK disabled.
    - Outputs: None.

11. `static inline void I2C_SetRxMode(I2C_Type *base)`
    - Description: Sets a given I2C module to operate in receiver mode.
    - Inputs:
      - `base`: I2C mem map of the I2C module to set as receiver.
    - Outputs: None.

12. `static inline void I2C_SetTxMode(I2C_Type *base)`
    - Description: Sets a given I2C module to operate in transmitter mode.
    - Inputs:
      - `base`: I2C mem map of the I2C module to set as transmitter.
    - Outputs: None.

13. `static inline void I2C_RepeatedStart(I2C_Type *base)`
    - Description: Generates a Repeated Start condition in a given I2C module.
    - Inputs:
      - `base`: I2C mem map of the I2C module to generate the Repeated Start condition.
    - Outputs: None.

14. `static inline void I2C_Start(I2C_Type *base)`
    - Description: Generates a start condition in a given I2C module.
    - Inputs:
      - `base`: I2C mem map of the I2C module to generate the start condition.
    - Outputs: None.

15. `static inline void I2C_Stop(I2C_Type *base)`
    - Description: Generates a stop condition in a given I2C module.
    - Inputs:
      - `base`: I2C mem map of the I2C module to generate the stop condition.
    - Outputs: None.

16. `static inline uint8_t I2C_Wait(I2C_Type *base)`
    - Description: Generates a dummy loop that checks the I2C interrupt flag and awaits until the interrupt flag is raised or leaves the loop in the worst case after about 17ms.
    - Inputs:
      - `base`: I2C mem map of the I2C module to await.
    - Outputs:
      - Returns `SYSTEM_STATUS_SUCCESS` if the interrupt flag was set; otherwise, returns `SYSTEM_STATUS_TIMEOUT`.

17. `static inline void I2C_Pause()`
    - Description: Performs a small pause with a dummy loop, used to leave the bus after sending a stop condition.
    - Inputs: None.
    - Outputs: None.

18. `static inline void I2C_WriteByte(I2C_Type *base, uint8_t data)`
    - Description: Writes a byte in the data I/O register of a given I2C module.
    - Inputs:
      - `base`: I2C mem map of the I2

C to write the data.
      - `data`: Byte to be written in the data I/O register.
    - Outputs: None.

19. `static inline uint8_t I2C_ReadByte(I2C_Type *base)`
    - Description: Reads a byte from the data I/O register of a given I2C module.
    - Inputs:
      - `base`: I2C mem map of the I2C to read the data.
    - Outputs:
      - Returns the data byte read.

20. `uint8_t I2C_WriteSlave(I2C_Type *base, uint8_t slave_addr, uint8_t data)`
    - Description: Writes one data byte to a given slave.
    - Inputs:
      - `base`: I2C mem map of the I2C module to be initialized.
      - `slave_addr`: 7-bit slave address.
      - `data`: Data byte to be transferred.
    - Outputs:
      - Returns one of the possible scenarios: `SYSTEM_STATUS_SUCCESS`, `SYSTEM_STATUS_TIMEOUT`, `SYSTEM_STATUS_INVALID_ADDRESS`, `SYSTEM_STATUS_INVALID_REGISTER`, or `SYSTEM_STATUS_TRANSFER_FAIL`.

21. `uint8_t I2C_WriteSlaveChunk(I2C_Type *base, uint8_t slave_addr, const uint8_t* const data, uint16_t data_size)`
    - Description: Writes N data bytes to a given slave.
    - Inputs:
      - `base`: I2C mem map of the I2C module to be initialized.
      - `slave_addr`: 7-bit slave address.
      - `data`: N data bytes to be transferred.
      - `data_size`: Size of the data to be transferred.
    - Outputs:
      - Returns one of the possible scenarios: `SYSTEM_STATUS_SUCCESS`, `SYSTEM_STATUS_TIMEOUT`, `SYSTEM_STATUS_INVALID_ADDRESS`, `SYSTEM_STATUS_INVALID_REGISTER`, or `SYSTEM_STATUS_TRANSFER_FAIL`.

22. `uint8_t I2C_WriteRegister(I2C_Type *base, uint8_t slave_addr, uint8_t register_addr, uint8_t data)`
    - Description: Writes one data byte to a given register of a slave.
    - Inputs:
      - `base`: I2C mem map of the I2C module to be initialized.
      - `slave_addr`: 7-bit slave address.
      - `register_addr`: 1-byte slave register address.
      - `data`: Data byte to be transferred.
    - Outputs:
      - Returns one of the possible scenarios: `SYSTEM_STATUS_SUCCESS`, `SYSTEM_STATUS_TIMEOUT`, `SYSTEM_STATUS_INVALID_ADDRESS`, `SYSTEM_STATUS_INVALID_REGISTER`, or `SYSTEM_STATUS_TRANSFER_FAIL`.

23. `uint8_t I2C_WriteRegisterChunk(I2C_Type *base, uint8_t slave_addr, uint8_t register_addr, const uint8_t* const data, uint16_t data_size)`
    - Description: Writes N data bytes to a given register of a slave.
    - Inputs:
      - `base`: I2C mem map of the I2C module to be initialized.
      - `slave_addr`: 7-bit slave address.
      - `register_addr`: 1-byte slave register address.
      - `data`: N data bytes to be transferred.
      - `data_size`: Size of the data to be transferred.
    - Outputs:
      - Returns one of the possible scenarios: `SYSTEM_STATUS_SUCCESS`, `SYSTEM_STATUS_TIMEOUT`, `SYSTEM_STATUS_INVALID_ADDRESS`, `SYSTEM_STATUS_INVALID_REGISTER`, or `SYSTEM_STATUS_TRANSFER_FAIL`.

24. `uint8_t I2C_ReadSlave(I2C_Type *base, uint8_t slave_addr, uint8_t* const result)`
    - Description: Reads a data byte from a given slave.
   

 - Inputs:
      - `base`: I2C mem map of the I2C module to be initialized.
      - `slave_addr`: 7-bit slave address.
      - `result`: Pointer to the destination where the result data will be stored.
    - Outputs:
      - Returns one of the possible scenarios: `SYSTEM_STATUS_SUCCESS`, `SYSTEM_STATUS_TIMEOUT`, `SYSTEM_STATUS_INVALID_ADDRESS`, `SYSTEM_STATUS_INVALID_REGISTER`, or `SYSTEM_STATUS_TRANSFER_FAIL`.

25. `uint8_t I2C_ReadSlaveChunk(I2C_Type *base, uint8_t slave_addr, uint8_t** const result, uint16_t result_size)`
    - Description: Reads N data bytes from a slave.
    - Inputs:
      - `base`: I2C mem map of the I2C module to be initialized.
      - `slave_addr`: 7-bit slave address.
      - `result`: Pointer to the destination where the result data will be stored.
      - `result_size`: Number of bytes to be read from the slave.
    - Outputs:
      - Returns one of the possible scenarios: `SYSTEM_STATUS_SUCCESS`, `SYSTEM_STATUS_TIMEOUT`, `SYSTEM_STATUS_INVALID_ADDRESS`, `SYSTEM_STATUS_INVALID_REGISTER`, or `SYSTEM_STATUS_TRANSFER_FAIL`.

26. `uint8_t I2C_ReadRegister(I2C_Type *base, uint8_t slave_addr, uint8_t register_addr, uint8_t* const result)`
    - Description: Reads a data byte from a given register of a slave.
    - Inputs:
      - `base`: I2C mem map of the I2C module to be initialized.
      - `slave_addr`: 7-bit slave address.
      - `register_addr`: 1-byte slave register address.
      - `result`: Pointer to the destination where the result data will be stored.
    - Outputs:
      - Returns one of the possible scenarios: `SYSTEM_STATUS_SUCCESS`, `SYSTEM_STATUS_TIMEOUT`, `SYSTEM_STATUS_INVALID_ADDRESS`, `SYSTEM_STATUS_INVALID_REGISTER`, or `SYSTEM_STATUS_TRANSFER_FAIL`.

27. `uint8_t I2C_ReadRegisterChunk(I2C_Type *base, uint8_t slave_addr, uint8_t register_addr, uint8_t** const result, uint16_t result_size)`
    - Description: Reads N data bytes from registers of a slave.
    - Inputs:
      - `base`: I2C mem map of the I2C module to be initialized.
      - `slave_addr`: 7-bit slave address.
      - `register_addr`: 1-byte slave register address.
      - `result`: Pointer to the destination where the result data will be stored.
      - `result_size`: Number of bytes to be read from the slave.
    - Outputs:
      - Returns one of the possible scenarios: `SYSTEM_STATUS_SUCCESS`, `SYSTEM_STATUS_TIMEOUT`, `SYSTEM_STATUS_INVALID_ADDRESS`, `SYSTEM_STATUS_INVALID_REGISTER`, or `SYSTEM_STATUS_TRANSFER_FAIL`.

## Usage Example

```
#include "i2c.h"

int main()
{
	// Initialize I2C module
	uint8_t initResult = I2C_Init(I2C0);

	// Validates initialization
	if (initResult != SYSTEM_STATUS_SUCCESS) {
		printf("I2C initialization failed\n");
		return 1;
	}

	// Random slave address and data
	uint8_t slaveAddress = 0x50;  // Replace with your desired slave address
	uint8_t sendData = 0xAB;  // Replace with your data to send

	// Write data to the slave
	uint8_t writeResult = I2C_WriteSlave(I2C0, slaveAddress, sendData);

	// Validates write
	if (writeResult != SYSTEM_STATUS_SUCCESS) {
		printf("Failed to write data to the slave\n");
		return 1;
	}

	printf("Data sent to slave successfully\n");

	return 0;
}
```