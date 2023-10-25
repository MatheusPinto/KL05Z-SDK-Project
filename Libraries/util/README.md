# Util

This module contains utility software libraries for embedded systems.
All the functions in this module have the "Util_" prefix.

Various parts of this module are taken from contributions by different developers, and are referenced in specific parts of the code.


# Libraries

- `bitwise` - Contains a API to facilitates bitwise operations.
- `math` - Implementation of various mathematical functions optimazed for embedded systems.
- `string` - Implementation of various functions for strings service optimazed for embedded systems.
- `swap` - Contains a swap function implementation.

Do not exclude any library from this directory, because some libraries have dependencies on each other.
The only external dependency (not C standart) in this module is the ["system/assert.h"](/Sources/system). 

In the following will be presented the API of each library.


# Bitwise

Contains a API to facilitates bitwise operations.

The following functions are available:

---
## Util_SetBit

Put the bit 1 (set) in the variable specific bit.

**Parameters:**
- `x`: Variable.
- `i`: Position of the bit to be set (0 = LSb).

---
## Util_ClrBit

Put the bit 0 (clear) in the variable specific bit.

**Parameters:**
- `x`: Variable.
- `i`: Position of the bit to be clear (0 = LSb).

---
## Util_GetBit

Return the bit value from the variable.

**Parameters:**
- `x`: Variable.
- `i`: Position of the bit whose value will be returned (0 = LSb).

**Return:**
- Non-zero value (with the same size as `x`) if the bit is one.
- Zero if the bit is zero.

---
## Util_MaskSet

Make the corresponding bit of value 1 in the mask, being 1 in the variable.

**Parameters:**
- `x`: Variable.
- `m`: Mask.

---
## Util_MaskClr

Make the corresponding bit of value 1 in the mask, being 0 in the variable.

**Parameters:**
- `x`: Variable.
- `m`: Mask.

---
## Util_MaskReturnSet

Return the value of the `x` variable masked with 1's bits, where the bits of the mask are set as 1.

**Parameters:**
- `x`: Variable.
- `m`: Mask.

**Return:**
- The value of `x` masked.

---
## Util_MaskReturnClr

Return the value of the `x` variable masked with 0's bits, where the bits of the mask are set as 1.

**Parameters:**
- `x`: Variable.
- `m`: Mask.

**Return:**
- The value of `x` masked.

---
## Util_MaskGet

Return a value that has all bits 0, except the same bits from the variable `x` that have the same position of the bits with value 1 from the mask.

**Parameters:**
- `x`: Variable.
- `m`: Mask.

**Return:**
- The value of `x` masked.

---
## Util_GetValue16LE

Returns a 16-bit Little Endian value from memory.

**Parameters:**
- `dataP`: Pointer to memory.

**Return:**
- The Little Endian value.

---
## Util_GetValue24LE

Returns a 24-bit Little Endian value from memory.

**Parameters:**
- `dataP`: Pointer to memory.

**Return:**
- The Little Endian value.

---
## Util_GetValue32LE

Returns a 32-bit Little Endian value from memory.

**Parameters:**
- `dataP`: Pointer to memory.

**Return:**
- The Little Endian value.

---
## Util_SetValue16LE

Stores a 16-bit value in memory as Little Endian.

**Parameters:**
- `data`: Value to store.
- `dataP`: Pointer to memory.

---
## Util_SetValue24LE

Stores a 24-bit value in memory as Little Endian.

**Parameters:**
- `data`: Value to store.
- `dataP`: Pointer to memory.

---
## Util_SetValue32LE

Stores a 32-bit value in memory as Little Endian.

**Parameters:**
- `data`: Value to store.
- `dataP`: Pointer to memory.
---


# Math

Implementation of various mathematical functions optimazed for embedded systems.

The following functions are available:

## Util_Cos

Compute the cosine of the argument x. Angles are specified in radians.

**Parameters:**
- `x`: A float value.

**Return:**
- The cosine of x in radians.

---

## Util_Sin

Compute the sine of the argument x. Angles are specified in radians.

**Parameters:**
- `x`: A float value.

**Return:**
- The sine of x in radians.

---

## Util_Atan

Computes the inverse tangent (arc tangent) of the input value.

**Parameters:**
- `x`: A float value.

**Return:**
- The arc tangent of x in radians, in the range of -pi/2 to pi/2.

---

## Util_Atan2

Computes the inverse tangent (arc tangent) of y/x.

**Note:**
Produces the correct result even for angles near pi/2 or -pi/2 (that is, when x is near 0).

**Parameters:**
- `x`: A float value.
- `y`: A float value.

**Return:**
- The arc tangent of x in radians, in the range of -pi to pi.

---

## Util_Map

Re-maps an integer number from one range to another. That is, a value of fromLow would get mapped to toLow, a value of fromHigh to toHigh, values in-between to values in-between, etc.

**Parameters:**
- `x`: The value that will be mapped.
- `in_min`: The min value that x originally assumes.
- `in_max`: The max value that x originally assumes.
- `out_min`: The min value that x will assume.
- `out_max`: The max value that x will assume.

**Return:**
- The x value mapped.

---

## Util_FloorSqrt

Gets the floor square root of an integer.

**Parameters:**
- `x`: The operand value.

**Return:**
- The x floor square root.

---

## Util_IntPow

Calculates the power of an integer number.

**Parameters:**
- `base`: The base operand.
- `exp`: The exponent operand.

**Return:**
- The base to the power of exp.

---

## Util_Abs

Calculates the absolute value of a number.

**Parameters:**
- `x`: The value that will be calculated.

**Return:**
- The number's absolute value.

---

## Util_Min

Returns the minimum value between two numbers.

**Parameters:**
- `a`, `b`: The numbers.

**Return:**
- The minimum value.


# String

Implementation of various functions for strings service optimazed for embedded systems.

The following functions are available:

## Util_StrLen

Returns the string length.

**Note:**
The null-terminated string is not counted.
Precondition: Null-terminated string.

**Parameters:**
- `str`: The null-terminated string.

**Return:**
- The string length.

---

## Util_StrCmp

Compares two strings.

**Parameters:**
- `str1`, `str2`: The strings to compare.

**Return:**
- If `str1` sorts lexicographically after `str2`, returns a number greater than zero.
- If the two strings match, returns zero.
- If `str1` sorts lexicographically before `str2`, returns a number less than zero.

---

## Util_StrNCmp

Compares up to `length` characters from the string at `str1` to the string at `str2`.

**Parameters:**
- `str1`, `str2`: The strings to compare.
- `len`: The maximum number of characters to compare.

**Return:**
- If `str1` sorts lexicographically after `str2`, returns a number greater than zero.
- If the two strings match, returns zero.
- If `str1` sorts lexicographically before `str2`, returns a number less than zero.

---

## Util_StrCopy

Copy the string `src` into `dst`. It performs the same task as `strncpy`, except:
- Always terminates the result string.
- Does not zero out the remaining part in `dst`.

**Note:**
`dstSize` is the size of `dst` INCLUDING the zero byte.
Precondition: `src`, `dst` != NULL.

**Parameters:**
- `dst`: Start of the string buffer, where to append the number string.
- `dstSize`: The size of the buffer, including the zero byte.
- `src`: The source string to copy.

---

## Util_StrCat

Concat the string `src` into `dst`. Always terminates the result string.

**Note:**
`dstSize` is the size of `dst` INCLUDING the zero byte.
Precondition: `src`, `dst` != NULL.

**Parameters:**
- `dst`: Start of the string buffer, where to append the number string.
- `dstSize`: The size of the buffer, including the zero byte.
- `src`: The source string to add.

---

## Util_CharCat

Adds a single character to a zero byte terminated string buffer. It cares about buffer overflow.

**Note:**
`dstSize` is the size of `dst` INCLUDING the zero byte.
Precondition: `src`, `dst` != NULL.

**Parameters:**
- `dst`: Start of the string buffer, where to append the character.
- `dstSize`: The size of the buffer, including the zero byte.
- `src`: The character to append.

---

## Util_ReverseStr

A function to reverse a string.

**Parameters:**
- `str`: The string that will be reversed.
- `length`: The string length.

---

## Util_StrFind

Searches a substring inside a string and returns the position.

**Parameters:**
- `str`: The string that will be searched.
- `subStr`: The substring to search inside `str`.

**Return:**
- -1 if not found,
- Otherwise, the character index.

---

## Util_IntToStr

Converts an integer into a null-terminated string. It can convert negative numbers too.

**Parameters:**
- `num`: The integer number.
- `str`: The string that will represent the number.
- `base`: The numerical system that the `num` will be shown in (2 = binary, 10 = decimal, 16 = hex).

**Return:**
- The string length.

---

## Util_StrToInt

Converts a null-terminated string representing a number into an integer value. It can convert negative numbers too.

**Parameters:**
- `str`: Pointer to the string to scan. Returns until where it has scanned.
- `res`: Pointer to where to store the result.

**Return:**
- `true` if conversion was successful.
- `false` otherwise.

---

## Util_FloatToStr

Converts a floating-point number into a null-terminated string.

**Parameters:**
- `n`: Input number.
- `res`: Array where the output string will be stored.
- `afterPoint`: Number of digits to consider after the decimal point.

**Return:**
- The string length.

---

## Util_StrToFloat

Converts a null-terminated string representing a number into a floating-point value. It can convert negative numbers too.

**Parameters:**
- `str`: Pointer to the string to scan. Returns until where it has scanned.
- `res`: Pointer to where to store the result.

**Return:**
- `true` if conversion was successful.
- `false` otherwise.

