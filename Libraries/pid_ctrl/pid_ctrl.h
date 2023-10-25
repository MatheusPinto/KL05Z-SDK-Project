/**
 * @file	pid_ctrl.h
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
 * This module contains an API to provide PID control. The user must initialize the
 * specific PID controller beforehand and pass to the API.
 */

#ifndef PID_CTRL_H_
#define PID_CTRL_H_

#include <libraries/delay/delay.h>
#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"

/*!
 * @addtogroup pid_ctrl
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< Defines if PID control instances will be created statically.
 *   If commented, serial instances will be allocated dynamically in heap. */
#define PID_CTRL_STATIC_OBJECTS_CREATION
#ifdef PID_CTRL_STATIC_OBJECTS_CREATION
#define PID_CTRL_MAX_STATIC_OBJECTS 1 /*!< The number of object instances that will be created statically.*/
#endif


/*!
 * @brief Serial manager configuration structure
 *
 * This structure holds the configuration settings for the serial module.
 */
typedef struct
{
	float gainP; /*!< Proportional gain.*/
	float gainI; /*!< Integral gain.*/
	float gainD; /*!< Derivative gain.*/
	float delthaTime; /*!< Time interval between two samples.*/
	float lastTime; /*!< Time instant when last PID calculation was done.*/
	float lastError; /*!< Input error when last PID calculation was done.*/
	float ErrorD; /*!< Input error derivative.*/
	float ErrorI; /*!< Input error integral.*/
	float ctrlP; /*!< Output proportional control.*/
	float ctrlI;  /*!< Output integral control.*/
	float ctrlD;  /*!< Output derivative control.*/
}pidCtrlConfig_t;


/*!< The PID controller specific handle that will must be passed to the API.*/
typedef void* pidCtrlHandle_t;


/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Creates the structure to configure the PID controller instance.
 *
 * @return - The configuration structure or;
 *         - NULL, if was not possible to create the structure.
 *
 */
pidCtrlConfig_t* CtrlPID_CreateConfig(void);

/**
 * @brief Initialize the PID controller.
 *
 * @param config - the variable with the configurations defined.
 *
 * @return - The specific PID controller handler that must be passed
 *           to the API or;
 *         - NULL, if was not possible to create the handler.
 *
 */
pidCtrlHandle_t CtrlPID_Init(pidCtrlConfig_t *config);


/**
 * @brief Calculates the PID control output.
 *
 *        This function will calculates the
 *        time interval based on actual time
 *        and the last time that calculation
 *        was done.
 *
 * @param handle - the specific PID handle.
 * @param actualError - the input actual error.
 * @param actualTime - the input actual time.
 *
 * @return The PID control value.
 *
 */
float CtrlPID_Calculate(pidCtrlHandle_t handle, float actualError, float actualTime);


/**
 * @brief Calculates the PID control output.
 *
 *        This function will NOT calculates the
 *        time interval. User must pass the
 *        time interval to API.
 *
 * @param handle - the specific PID handle.
 * @param actualError - the input actual error.
 * @param delthaTime - the time interval between samples.
 *
 * @return The PID control value.
 *
 */
float CtrlPID_CalculateWithInterval(pidCtrlHandle_t handle, float actualError, float delthaTime);

/*! @}*/

#endif /* PID_CTRL_H_ */
