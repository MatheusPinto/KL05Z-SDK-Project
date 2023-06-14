/**
 * @file	string.h
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
 * String function implementations optimized for embedded.
 *
 */

#ifndef LIBRARIES_UTIL_STRING_H_
#define LIBRARIES_UTIL_STRING_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>


/*! @}*/

/*!
 * @name String Manipulation
 * @{
 */


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*Comment the macros below to not include the function in the binary file*/
#define STR_COPY_FUNC 	  //StrCopy
#define STR_CAT_FUNC 	  //StrCat
#define CHAR_CAT_FUNC 	  //CharCat
#define REVERSE_FUNC 	  //Reverse
#define STR_FIND_FUNC 	  //StrFind
#define ITOA_FUNC 		  //ItoA
#define ATOI_FUNC 		  //AtoI
#define FTOA_FUNC 		  //FtoA
#define ATOF_FUNC 		  //AtoF
#define ETOA_FUNC 		  //EtoA
#define ATOE_FUNC 		  //AtoE


/*******************************************************************************
 * API
 ******************************************************************************/


 /**
 * @brief Returns the string length.
 *
 * @Note: The null terminated string is not counted.
 * @Precondition: Null terminated string.
 *
 * @param str - The null terminated string.
 *
 * @return The string length.
 *
 */
#define StrLen(str) strlen(str)


 /**
 * @brief Compares two strings.
 * *
 * @param str1, str2 - The strings to compare.
 *
 * @return - If str1 sorts lexicographically after
 *           str2, returns a number greater than zero.
 *         - If the two strings match, returns zero.
 *         - If str1 sorts lexicographically before str2,
 *           returns a number less than zero.
 *
 */
#define StrCmp(str1, str2) strcmp(str1, str2)


 /**
 * @brief Compares up to length characters from the
 *        string at str1 to the string at str2.
 * *
 * @param str1, str2 - The strings to compare.
 *
 * @return - If str1 sorts lexicographically after
 *           str2, returns a number greater than zero.
 *         - If the two strings match, returns zero.
 *         - If str1 sorts lexicographically before str2,
 *           returns a number less than zero.
 *
 */
#define StrNCmp(str1, str2, len) strncmp(str1, str2, len)


#ifdef STR_COPY_FUNC
/**
* @brief Copy the string src into dst. It performs the same task as strncpy,
*        except:
*           - always terminates the result string.
*           - does not zero out the remaining part in dst.
*
* @Note: dstSize is the size of dst INCLUDING zero byte.
* @Precondition: src, dst != NULL
*
* @param dst     - Start of string buffer, where to append the number string.
* @param dstSize - The size of the buffer, including the zero byte.
* @param src - The source string to copy.
*
*/
void Util_StrCopy(uint8_t *dst, size_t dstSize, const char *src);
#endif /* EMB_STR_COPY_FUNC */


#ifdef STR_CAT_FUNC
/**
 * @brief Concat the string src into dst. Always terminates the result string.
 *
 * @Note: count is the size of dst INCLUDING zero byte.
 * @Precondition: src, dst != NULL
 *
 * @param dst     - Start of string buffer, where to append the number string.
 * @param dstSize - The size of the buffer, including the zero byte.
 * @param src - The source string to add.
 *
 */
void Util_StrCat(uint8_t *dst, size_t dstSize, const unsigned char *src);
#endif /* EMB_STR_CAT_FUNC */


#ifdef CHAR_CAT_FUNC
/**
 * @brief Adds a single character to a zero byte terminated string
 *         buffer. It cares about buffer overflow.
 *
 * @Note: count is the size of dst INCLUDING zero byte.
 * @Precondition: src, dst != NULL
 *
 * @param dst     - Start of string buffer, where to append the character.
 * @param dstSize - The size of the buffer, including the zero byte.
 * @param src - The character to append.
 *
 */
void Util_CharCat(uint8_t *dst, size_t dstSize, uint8_t ch);
#endif /* CHAR_CAT_FUNC */

#ifdef REVERSE_FUNC
/**
 * @brief A function to reverse a string.
 *
 * @param str - the string that will be reverted.
 * @param length - the string length.
 *
 */
void Util_ReverseStr(uint8_t *str, size_t length);
#endif


#ifdef STR_FIND_FUNC
/**
 * @brief Searches a substring inside a string and returns the
 *        position.
 *
 * @param str  - the string that will be searched.
 * @param subStr - substring to search inside str.
 *
 * @return -1 if not found,
 *          otherwise the character index.
 *
 */
int16_t Util_StrFind(uint8_t *str, uint8_t *subStr);
#endif /* STR_FIND_FUNC */


#ifdef ITOA_FUNC
 /**
  * @brief Converts integer into null-terminated string.
  *		   It can convert negative numbers too.
  *
  * @param num  - the integer number.
  * @param str  - the string that will represent the number.
  * @param base - the numerical system that the num will be showed:
  * 		      \a 2 - binary;
  * 			  \a 10 - decimal;
  * 			  \a 16 - hex.
  *
  * @return The string length.
  *
  */
int Util_ItoA(int32_t num, uint8_t* str, uint8_t base);
#endif

#ifdef ATOI_FUNC
 /**
  * @brief Converts a null-terminated string representing a number
  *        into an integer value.
  *		   It can convert negative numbers too.
  *
  * @param str  - Pointer to string to scan. Returns until
  *               where it has scanned.
  * @param res  - Pointer to where to store the result
  *
  * @return \a true  - if conversion was ok;
  *         \a false - otherwise.
  *
  */
bool Util_AtoI(const unsigned char** str, int32_t* res);
#endif

#if defined(ITOA_FUNC) && defined(FTOA_FUNC)
/**
 * @brief Converts floating-point number into null-terminated string.
 *
 * @param n - input number.
 * @param res - array where output string to be stored.
 * @param afterPoint - number of digits to be considered after the point.
 *
 * @return The string length.
 *
 */
int Util_FtoA(float n, uint8_t *res, int afterPoint);
#endif

#ifdef ATOF_FUNC
/**
 * @brief Converts a null-terminated string representing a number
 *        into an float value.
 *		   It can convert negative numbers too.
 *
 * @param str  - Pointer to string to scan. Returns until
 *               where it has scanned.
 * @param res  - Pointer to where to store the result
 *
 * @return \a true  - if conversion was ok;
 *         \a false - otherwise.
 *
 */
bool Util_AtoF(const unsigned char** str, float* res);
#endif

/*! @}*/


#endif /* LIBRARIES_UTIL_STRING_H_ */
