/**
 * @file	math.cpp
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
 * Mathematical function implementation optimized for embedded.
 *
 */

#include "math.h"


/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef MAP_FUNC
int32_t Util_Map(int32_t x,
		         int32_t in_min,
				 int32_t in_max,
				 int32_t out_min,
				 int32_t out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
#endif

#ifdef FLOOR_SQRT_FUNC
int Util_FloorSqrt(int x)
{
    // Base cases
    if (x == 0 || x == 1)
    return x;

    // Staring from 1, try all numbers until
    // i*i is greater than or equal to x.
    int i = 1, result = 1;
    while (result <= x)
    {
      i++;
      result = i * i;
    }
    return i - 1;
}
#endif

#ifdef IPOW_FUNC
int Util_IntPow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}
#endif

