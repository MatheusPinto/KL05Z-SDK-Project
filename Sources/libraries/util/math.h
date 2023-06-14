/**
 * @file	math.h
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

#ifndef LIBRARIES_UTIL_MATH_H_
#define LIBRARIES_UTIL_MATH_H_

#include <math.h>
#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< The pi number.*/
#define PI_NUMBER	3.1415926535897932384626433832795
/*!< The pi number times 2.*/
#define TWO_PI_NUMBER 6.283185307179586476925286766559

/*Comment the macros below to not include the function in the binary file*/
#define MAP_FUNC 		  //Map
#define FLOOR_SQRT_FUNC   //FloorSqrt
#define IPOW_FUNC 		  //IntPow


/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @name Mathematics
 * @{
 */

/**
 * @brief Compute the cosine of the argument x.
 *        Angles are specified in radians.
 *
 *
 * @param x - a float value.
 *
 * @return the cosine of x in radians.
 */
static inline float Util_Cos(float x)
{
	return cosf(x);
}

/**
 * @brief Compute the sine of the argument x.
 *        Angles are specified in radians.
 *
 *
 * @param x - a float value.
 *
 * @return the sine of x in radians.
 */
static inline float Util_Sin(float x)
{
	return sinf(x);
}

/**
 * @brief Computes the inverse tangent (arc tangent) of the input value.
 *
 * @param x - a float value.
 *
 * @return the arc tangent of x in radians, in the range of -pi/2 to pi/2.
 */
static inline float Util_Atan(float x)
{
	return atanf(x);
}

/**
 * @brief Computes the inverse tangent (arc tangent) of y/x.
 *
 * @note Produces the correct result even for angles near
 *       pi/2 or -pi/2 (that is, when x is near 0).
 *
 * @param x - a float value.
 *
 * @return the arc tangent of x in radians, in the range of -pi to pi.
 */
static inline float Util_Atan2(float x, float y)
{
	return atan2f(x, y);
}


#ifdef MAP_FUNC
/**
 * @brief Re-maps an integer number from one range to another.
 *        That is, a value of fromLow would get mapped to toLow,
 *        a value of fromHigh to toHigh, values in-between to
 *        values in-between, etc.
 *
 * @param x	      - The value that will be mapped.
 * @param in_min  - The min value that x originally assumes.
 * @param in_max  - The max value that x originally assumes.
 * @param out_min - The min value that x will assumes.
 * @param out_min - The max value that x will assumes.
 *
 * @return The x value mapped.
 *
 */
int32_t Util_Map(int32_t x,
		         int32_t in_min,
				 int32_t in_max,
				 int32_t out_min,
				 int32_t out_max);
#endif

#ifdef FLOOR_SQRT_FUNC
/**
 * @brief Gets the floor square root of an integer.
 *
 * @param x	- The operand value.
 *
 * @return The x floor square root.
 *
 */
int Util_FloorSqrt(int x);
#endif

#ifdef IPOW_FUNC
/**
 * @brief Calculates the power of a integer number.
 *
 * @param base - The base operand.
 * @param exp  - The exponent operand.
 *
 * @return The base to the power of exp.
 *
 */
int Util_IntPow(int base, int exp);
#endif

/**
 * @brief Calculates the absolute value of a number.
 *
 * @param x - the value that will be calculated.
 *
 * @return The number absolute value.
 *
 */
#define Util_Abs( x ) (((x) > 0) ? (x) : (-1*(x)))

/**
 * @brief Returns the minimum value between two numbers.
 *
 * @param a, b - the numbers.
 *
 * @return The minimum value.
 *
 */
#define Util_Min(a,b) ((a < b) ? a : b)

/*! @}*/

#endif /* LIBRARIES_UTIL_MATH_H_ */
