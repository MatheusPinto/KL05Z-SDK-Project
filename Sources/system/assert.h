/**
 * @file	assert.h
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
 * Assert function implementation.
 *
 */

#ifndef SYSTEM_ASSERT_H_
#define SYSTEM_ASSERT_H_

/*!< If FreeRTOS is present, uncomment this macro or set in the compiler parameters.*/
//#define __FREERTOS_H

//#define NDEBUG

#ifdef NDEBUG
#define SYSTEM_ASSERT(n) (void)0
#else
#ifdef __FREERTOS_H
#include <FreeRTOS.h>
#include <task.h>
#define SYSTEM_ASSERT(n) configASSERT(n)
#else
#define SYSTEM_ASSERT(n) \
    while (!(n))  \
    {             \
                 \
    }
#endif //__FREERTOS_H
#endif // NDEBUG

#endif /* SYSTEM_ASSERT_H_ */
