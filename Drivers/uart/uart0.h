/***************************************************************************************
 * @file        uart0.h
 * @version     1.0
 * @date        05/24/2023
 * @brief       File with implementations of UART0 Modules for Kinetis KL Family.
 * @remarks     None.
 * @author      Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef UART0_DRV_H_
#define UART0_DRV_H_

#include <common.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @addtogroup uart driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define UART0_OSR_FIELD_VALUE 15


/* @brief Possible combinations to enable Tx and Rx */
typedef enum{
    UART0_TX_RX_DISABLE = 0U, /**< Disable Tx and Rx */
    UART0_TX_ENABLE = UART0_C2_TE_MASK, /**< Enable Tx */
    UART0_RX_ENABLE = UART0_C2_RE_MASK, /**< Enable Rx */
    UART0_TX_RX_ENABLE =
        UART0_C2_TE_MASK | UART0_C2_RE_MASK /**< Enable Tx and Rx */
}uart0TxRxEnabling_t;

/* @brief Possible parity modes */
typedef enum {
    UART0_NO_PARITY = 0U, /**< Disable Parity */
    UART0_PARITY_EVEN =
        UART0_C1_PE_MASK | UART0_C2_TE_MASK, /**< Enable Parity and Tx */
    UART0_PARITY_ODD =
        UART0_C1_PE_MASK | UART0_C2_RE_MASK, /**< Enable Parity and Rx */
} uart0Parity_t;

/* @brief Possible number of stop bits */
typedef enum {
    UART0_ONE_STOP_BIT = 0U, /**< 1 stop bit */
    UART0_TWO_STOP_BIT /**< 2 stop bits */
} uart0StopBitNum_t;

/* @brief Possible clock sources for the UART module */
typedef enum {
    UART0_CLOCK_DISABLE = 0U,
    UART0_CLOCK_FLL = 1U,
    UART0_CLOCK_OSC = 2U,
    UART0_CLOCK_IRC = 3U
} uart0ClkSrc_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Check for Receive Overrun error.
 *
 * This function checks if there was an Overrun error during data reception.
 *
 * @return 0 if there was no Overrun error; 1 if there was an Overrun error.
 */
static inline uint8_t UART0_IsRxOverrun( void );

/**
 * @brief Check for Framing Error.
 *
 * This function checks if there was a Framing error during data reception.
 *
 * @return 0 if there was no error; 1 if there was an error.
 */
static inline uint8_t UART0_IsFramingError( void );

/**
 * @brief Check for Parity Error.
 *
 * This function checks if there was a Parity error during data reception.
 *
 * @return 0 if there was no error; 1 if there was an error.
 */
static inline uint8_t UART0_IsParityError( void );

/**
 * @brief Clear Rx Buffer and Error Flags.
 *
 * This function clears the error flags and the receive buffer.
 */
static inline void UART0_CleanRxBuffer( void );

/**
 * @brief Deinitialize UART0.
 *
 * This function disables the UART0 module.
 */
static inline void UART0_Deinit( void );

/**
 * @brief Check if data is available in the Rx buffer.
 *
 * This function checks if there is data to be read from the Rx buffer.
 *
 * @return 0 if there is no data to be read; 1 if there is unread data in the buffer.
 */
static inline uint8_t UART0_IsRxAvailable( void );

/**
 * @brief Check if the Tx buffer is available for writing.
 *
 * This function checks if the Tx buffer is available for writing data.
 *
 * @return 0 if the Tx buffer is not available; 1 if the Tx buffer is available for writing.
 */
static inline uint8_t UART0_IsTxAvailable( void );

/**
 * @brief Send 8-bit data through UART0.
 *
 * This function sends an 8-bit data through UART0.
 *
 * @param data - The 8-bit data to send.
 */
static inline void UART0_Write( uint8_t data );

/**
 * @brief Read 8-bit data from UART0.
 *
 * This function returns an 8-bit data received through UART0.
 *
 * @return The 8-bit data received.
 *
 * @note If no new data has been sent to RX, it will read the last received value.
 *       Use the UART0_IsRxAvailable function to check if new data has been sent.
 */
static inline uint8_t UART0_Read( void );

/**
 * @brief Enable Tx Interrupt.
 *
 * This function enables the interrupt for complete data transmission by UART0.
 */
static inline void UART0_EnableTxIRQ( void );

/**
 * @brief Enable Rx Interrupt.
 *
 * This function enables the interrupt for complete data reception by UART0.
 */
static inline void UART0_EnableRxIRQ( void );

/**
 * @brief Disable Tx Interrupt.
 *
 * This function disables the interrupt for complete data transmission by UART0.
 */
static inline void UART0_DisableTxIRQ( void );

/**
 * @brief Disable Rx Interrupt.
 *
 * This function disables the interrupt for complete data reception by UART0.
 */
static inline void UART0_DisableRxIRQ( void );

/**
 * @brief Set the clock source used by UART.
 *
 * This function defines the clock source used by the UART module.
 *
 * @param src - Clock source defined in an enumerated type.
 *
 * @return STATUS_SUCCESS if the configuration was successful; STATUS_FAIL otherwise.
 *
 * @note This function should be called before UART_Init.
 */
uint8_t UART0_SetClkSrc( uart0ClkSrc_t src );

/**
 * @brief Initialize the UART module pin.
 *
 * This function initializes the UART module pin with the specified parameters.
 *
 * @param baudRate - The baudrate.
 * @param enTxRx - Disable or enable Tx, Rx, or both.
 * @param parity - Even or odd parity.
 * @param stopBitsN - Number of stop bits (1U or 2U).
 *
 * @note This function should be called after the UART_SetClkSrc.
 */
void UART0_Init( uint32_t baudRate,
                uart0TxRxEnabling_t enTxRx,
                uart0Parity_t parity,
                uart0StopBitNum_t stopBitsN);

/*******************************************************************************
 * Code
 ******************************************************************************/

/**********************************************************************************/
static inline uint8_t UART0_IsRxOverrun( void )
{
    return (uint8_t)((UART0->S1 & UART0_S1_OR_MASK) != 0);
}

/**********************************************************************************/
static inline uint8_t UART0_IsFramingError( void )
{
    return (uint8_t)((UART0->S1 & UART0_S1_FE_MASK) != 0);
}

/**********************************************************************************/
static inline uint8_t UART0_IsParityError( void )
{
    return (uint8_t)((UART0->S1 & UART0_S1_PF_MASK) != 0);
}

/**********************************************************************************/
static inline void UART0_CleanRxBuffer( void )
{
    UART0_IsRxAvailable();  /* Reads the UART_S1 register */
    UART0_Read();
}

/**********************************************************************************/
static inline void UART0_Deinit( void )
{
    SIM->SCGC4 &= ~SIM_SCGC4_UART0_MASK;

    NVIC_DisableIRQ( UART0_IRQn );
}

/**********************************************************************************/
static inline uint8_t UART0_IsRxAvailable( void )
{
    return (uint8_t)((UART0->S1 & UART0_S1_RDRF_MASK) != 0);
}

/**********************************************************************************/
static inline uint8_t UART0_IsTxAvailable( void )
{
    return (uint8_t)((UART0->S1 & UART0_S1_TDRE_MASK) != 0);
}

/**********************************************************************************/
static inline void UART0_Write( uint8_t data )
{
    UART0_D = data;
}

/**********************************************************************************/
static inline uint8_t UART0_Read( void )
{
    return (uint8_t)UART0->D;
}

/**********************************************************************************/
static inline void UART0_EnableTxIRQ( void )
{
    UART0->C2 |= UART0_C2_TIE_MASK;
}

/**********************************************************************************/
static inline void UART0_EnableRxIRQ( void )
{
    /* Enable complete reception, overrun, and framing error interrupts */
    UART0->C2 |= UART0_C2_RIE_MASK;
    UART0->C3 |= UART0_C3_ORIE_MASK | UART0_C3_FEIE_MASK;
}

/**********************************************************************************/
static inline void UART0_DisableTxIRQ( void )
{
    UART0->C2 &= ~UART0_C2_TIE_MASK;
}

/**********************************************************************************/
static inline void UART0_DisableRxIRQ( void )
{
    UART0->C2 &= ~UART0_C2_RIE_MASK;
}

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /*UART0_DRV_H_*/
