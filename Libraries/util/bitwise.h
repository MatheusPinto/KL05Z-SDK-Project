/**
 * @file	bitwise.h
 * @author  Matheus Leitzke Pinto <matheus.pinto@ifsc.edu.br>
 * @version 1.0
 * @date    2023
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * It contains utility API for bitwise operations.
 *
 */

#ifndef LIBRARIES_UTIL_BITWISE_H_
#define LIBRARIES_UTIL_BITWISE_H_

#include <stdint.h>


/*******************************************************************************
 * Definitions
 ******************************************************************************/

// Comment this macros if not using these functions
#define GETVALUE_16LE // GetValue16LE
#define GETVALUE_24LE // GetValue24LE
#define GETVALUE_32LE // GetValue32LE
#define SETVALUE_16LE // SetValue16LE
#define SETVALUE_24LE // SetValue24LE
#define SETVALUE_32LE // SetValue32LE


/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @name Bitwise Operations
 * @{
 */

/**
 * @brief Put the bit 1 (set) in the variable specific bit.
 *
 *
 * @param x - variable.
 * @param i - position of the bit to be set (0 = LSb).
 *
 */
#define Util_SetBit(x, i) x |= ( (sizeof( x )1 << i )

/**
 * @brief Put the bit 0 (clear) in the variable specific bit.
 *
 *
 * @param x - variable.
 * @param i - position of the bit to be clear (0 = LSb).
 *
 */
#define Util_ClrBit(x, i) x &= ((typeof( x ))0xFFFFFF ^ ( (sizeof( x )1 << i ) )

/**
 * @brief Return the bit value from the variable.
 *
 * @param x - variable.
 * @param i - position of the bit whose value will be returned (0 = LSb).
 *
 * @return non-zero value (with same size as x) if bit is one;
 *         zero if bit is zero.
 *
 */
#define Util_GetBit(x, i) (x & (sizeof( x )(1 << i)))

/**
 * @brief It makes the corresponding bit of value 1 in mask, being 1
 * 		  in the variable.
 * *
 * @param x - variable.
 * @param m - mask.
 *
 */
#define Util_MaskSet(x, m) x |= m

/**
 * @brief It makes the corresponding bit of value 1 in mask, being 0
 * 		  in the variable.
 * *
 * @param x - variable;
 * @param m - mask.
 *
 */
#define Util_MaskClr(x, m) x &= ~m

/**
 * @brief Returns the value of x variable masked with 1's bits,
 *        where the bits of mask is set as 1.
 *
 * @param x - variable;
 * @param m - mask.
 *
 * @return the value of x masked.
 *
 */
#define Util_MaskReturnSet(x, m) (x | m)

/**
 * @brief Returns the value of x variable masked with 0's bits,
 *        where the bits of mask is set as 1.
 *
 * @param x - variable;
 * @param m - mask.
 *
 * @return the value of x masked.
 *
 */
#define Util_MaskReturnClr(x, m) (x & ~m)

/**
 * @brief Returns a value that have all bits 0, except the same
 * 		  bits from the variable x that have the same position
 * 		  of the bits with value 1 from the mask.
 *
 * @param x - variable;
 * @param m - mask.
 *
 * @return the value of x masked.
 *
 */
#define Util_MaskGet(x, m) (x & m)

#ifdef GETVALUE_16LE
/**
 * @brief Returns a 16bit Little Endian value from memory.
 *
 * @param dataP - Pointer to memory
 *
 * @return The Little Endian.
 *
 */
uint16_t Util_GetValue16LE(uint8_t *dataP);
#endif /* EMBUTIL_GETVALUE_16LE */

#ifdef GETVALUE_24LE
 /**
  * @brief Returns a 24bit Little Endian value from memory.
  *
  * @param dataP - Pointer to memory
  *
  * @return The Little Endian.
  *
  */
 uint32_t Util_GetValue24LE(uint8_t *dataP);
#endif /* EMBUTIL_GETVALUE_24LE */

#ifdef GETVALUE_32LE
/**
 * @brief Returns a 32bit Little Endian value from memory.
 *
 * @param dataP - Pointer to memory
 *
 * @return The Little Endian.
 *
 */
 uint32_t Util_GetValue32LE(uint8_t *dataP);
#endif /* EMBUTIL_GETVALUE_32LE */

#ifdef SETVALUE_16LE
/**
 * @brief Stores a 16bit value in memory as Little Endian.
 *
 * @data  - Value to store
 * @dataP - Pointer to memory
 *
 */
void Util_SetValue16LE(uint16_t data, uint8_t *dataP);
#endif /* EMBUTIL_SETVALUE_16LE */

#ifdef SETVALUE_24LE
/**
 * @brief Stores a 24bit value in memory as Little Endian.
 *
 * @data  - Value to store
 * @dataP - Pointer to memory
 *
 */
void Util_SetValue24LE(uint32_t data, uint8_t *dataP);
#endif /* EMBUTIL_SETVALUE_24LE */

#ifdef SETVALUE_32LE
/**
 * @brief Stores a 32bit value in memory as Little Endian.
 *
 * @data  - Value to store
 * @dataP - Pointer to memory
 *
 */
void Util_SetValue32LE(uint32_t data, uint8_t *dataP);
#endif /* EMBUTIL_SETVALUE_32LE */

/*! @}*/


#endif /* SOURCES_LIBRARIES_UTIL_BITWISE_H_ */
