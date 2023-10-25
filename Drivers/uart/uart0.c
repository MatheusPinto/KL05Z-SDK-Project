/***************************************************************************************
 * @file        uart0.c
 * @version     1.0
 * @date        05/24/2023
 * @brief       File with implementations of UART0 Modules for Kinetis KL Family.
 * @remarks     None.
 * @author      Matheus Leitzke Pinto
 ***************************************************************************************/

#include "uart0.h"

/*!
 * @addtogroup uart driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

static systemClock_t g_uartClkSrc;

/*******************************************************************************
 * Code
 ******************************************************************************/

/**********************************************************************************/
uint8_t UART0_SetClkSrc( uart0ClkSrc_t src )
{
    switch(src)
    {
    case UART0_CLOCK_DISABLE:
        g_uartClkSrc = 0U;
        break;
    case UART0_CLOCK_FLL:
        g_uartClkSrc = MCG_FLL_CLK_FREQ;
        break;
    case UART0_CLOCK_IRC:
        g_uartClkSrc = MCG_IRC_CLK_FREQ;
        break;
    case UART0_CLOCK_OSC:
        g_uartClkSrc = OSC_ER_CLK_FREQ;
        break;
    default:
        return SYSTEM_STATUS_FAIL;
        break;
    }

    SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(src);

    return SYSTEM_STATUS_SUCCESS;
}

/**********************************************************************************/
void UART0_Init( uint32_t baudRate,
                uart0TxRxEnabling_t enTxRx,
                uart0Parity_t parity,
                uart0StopBitNum_t stopBitsN)
{
    uint16_t sbr_field;

    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    /* Clear initial settings */
    UART0->C1 = 0x00U;
    UART0->C2 = 0x00U;
    UART0->C3 = 0x00U;
    UART0->S2 = 0x00U;

    UART0->C4 &= ~UART0_C4_OSR_MASK;
    UART0->C4 |= UART0_C4_OSR(UART0_OSR_FIELD_VALUE); /* Set OSR field value */

    sbr_field  = g_uartClkSrc / (baudRate * (UART0_OSR_FIELD_VALUE + 1)); /* Set SBR field value */
    UART0->BDH |= UART0_BDH_SBR((sbr_field >> 8)); 						  /* Put MSB of SBR into UART0_BDH */
    UART0->BDL = UART0_BDL_SBR(sbr_field); 								  /* Put LSB of SBR into UART0_BDL */

    UART0->BDH |= UART0_BDH_SBNS(stopBitsN);

    UART0->C2 |= enTxRx;

    UART0_CleanRxBuffer(); /* Clear Rx initialization flags */

    UART0->C1 &= ~UART0_C1_PT_MASK; /* Clear previous parity settings */
    UART0->C1 |= parity; /* Enable parity */
}

/*! @}*/
