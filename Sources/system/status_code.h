/**
 * @file	status_code.h
 * @author  Matheus Leitzke Pinto <matheus.pinto@ifsc.edu.br>
 * @version 1.0
 * @date    2021
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
 * The general status codes for use in the whole system.
 *
 */

#ifndef SYSTEM_STATUS_CODE_H_
#define SYSTEM_STATUS_CODE_H_

enum
{
	SYSTEM_STATUS_SUCCESS = 0,  /*!< Generic status for Success. */
	SYSTEM_STATUS_FAIL = 1,      /*!< Generic status for Fail. */
	SYSTEM_STATUS_READ_ONLY = 2,    /*!< Generic status for read only failure. */
	SYSTEM_STATUS_OUT_OF_RANGE = 3,   /*!< Generic status for out of range access. */
	SYSTEM_STATUS_INVALID_ARGUMENT = 4,   /*!< Generic status for invalid argument check. */
	SYSTEM_STATUS_TIMEOUT = 5,   /*!< Generic status for timeout. */
	SYSTEM_STATUS_NO_TRANSFER_IN_PROGRESS = 6,   /*!< Generic status for no transfer in progress. */
	SYSTEM_STATUS_BUSY = 7   /*!< Generic status for module is busy. */
};

#endif /* SYSTEM_STATUS_CODE_H_ */
