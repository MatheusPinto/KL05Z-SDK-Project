/**
 * @file	bitwise.c
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
 * It contains API util for bitwise operations.
 *
 */

#include "bitwise.h"


/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef GETVALUE_16LE
uint16_t Util_GetValue16LE(uint8_t *dataP)
{
	return (uint16_t)((dataP[1]<<8)+(dataP[0]));
}
#endif /* GETVALUE_16LE */

#ifdef GETVALUE_24LE
uint32_t Util_GetValue24LE(uint8_t *dataP)
{
	return (uint32_t)(((uint32_t)dataP[2])<<16)+(dataP[1]<<8)+(dataP[0]);
}
#endif /* GETVALUE_24LE */

#ifdef GETVALUE_32LE
uint32_t Util_GetValue32LE(uint8_t *dataP)
{
	return (uint32_t)(((uint32_t)dataP[3])<<24)+(((uint32_t)dataP[2])<<16)+(dataP[1]<<8)+(dataP[0]);
}
#endif /* GETVALUE_32LE */

#ifdef SETVALUE_16LE
void Util_SetValue16LE(uint16_t data, uint8_t *dataP)
{
	dataP[0] = (uint8_t)(data&0xff); /* LSB */
	dataP[1] = (uint8_t)((data>>8)&0xff); /* MSB */
}
#endif /* SETVALUE_16LE */

#ifdef SETVALUE_24LE
void Util_SetValue24LE(uint32_t data, uint8_t *dataP)
{
	dataP[0] = (uint8_t)(data&0xff); /* LSB */
	dataP[1] = (uint8_t)((data>>8)&0xff);
	dataP[2] = (uint8_t)((data>>16)&0xff);
}
#endif /* SETVALUE_24LE */

#ifdef SETVALUE_32LE
void Util_SetValue32LE(uint32_t data, uint8_t *dataP)
{
	dataP[0] = (uint8_t)(data&0xff); /* LSB */
	dataP[1] = (uint8_t)((data>>8)&0xff);
	dataP[2] = (uint8_t)((data>>16)&0xff);
	dataP[3] = (uint8_t)((data>>24)&0xff);
}
#endif /* SETVALUE_32LE */
