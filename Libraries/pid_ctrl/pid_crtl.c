/**
 * @file	pid_ctrl.c
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
 * This module contains a PID Control implementation.
 */

#include <libraries/pid_ctrl/pid_ctrl.h>

struct pidCtrlHandle_s{
	pidCtrlConfig_t* config;
};

#ifdef PID_CTRL_STATIC_OBJECTS_CREATION
static pidCtrlConfig_t g_pidCtrlConfigList[PID_CTRL_MAX_STATIC_OBJECTS];
static struct pidCtrlHandle_s g_pidCtrlHandleList[PID_CTRL_MAX_STATIC_OBJECTS];
static uint8_t g_staticConfigsCreated, g_staticHandlesCreated;
#endif


enum{
	PID_CTRL_OBJECT_IS_HANDLE,
	PID_CTRL_OBJECT_IS_CONFIG
};


static void* CreateObject(uint8_t objectType)
{
	void* objectCreated = NULL;
#ifdef PID_CTRL_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case PID_CTRL_OBJECT_IS_HANDLE:
		if(g_staticHandlesCreated < PID_CTRL_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_pidCtrlHandleList[g_staticHandlesCreated++];
		}
		break;
	case PID_CTRL_OBJECT_IS_CONFIG:
		if(g_staticConfigsCreated < PID_CTRL_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_pidCtrlConfigList[g_staticConfigsCreated++];
		}
		break;
	}


#else
	objectCreated = embUtil_Malloc(sizeof(pidCtrlConfig_t));
#endif
	return objectCreated;
}

static void DestroyObject(void* obj, uint8_t objectType)
{
#ifdef PID_CTRL_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case PID_CTRL_OBJECT_IS_HANDLE:
		if(g_staticHandlesCreated)
			--g_staticHandlesCreated;
		break;
	case PID_CTRL_OBJECT_IS_CONFIG:
		if(g_staticConfigsCreated)
			--g_staticConfigsCreated;
		break;
	}

#else
	embUtil_Free(obj);
#endif
	obj = NULL;
}


/**
 * @brief Creates the structure to configure the PID controller instance.
 *
 * @return - The configuration structure or;
 *         - NULL, if was not possible to create the structure.
 *
 */
pidCtrlConfig_t* CtrlPID_CreateConfig(void)
{
	return CreateObject(PID_CTRL_OBJECT_IS_CONFIG);
}

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
pidCtrlHandle_t CtrlPID_Init(pidCtrlConfig_t *config)
{
	SYSTEM_ASSERT(config);

	struct pidCtrlHandle_s* handle = CreateObject(PID_CTRL_OBJECT_IS_HANDLE);
	if (handle)
	{
		handle->config = config;
	}

	return handle;
}


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
float CtrlPID_Calculate(pidCtrlHandle_t handle, float actualError, float actualTime)
{
	pidCtrlConfig_t* pidConfig = (pidCtrlConfig_t*)handle;

	pidConfig->delthaTime = actualTime - pidConfig->lastTime;

	pidConfig->ErrorI += actualError * pidConfig->delthaTime;
	pidConfig->ErrorD = (actualError - pidConfig->lastError)/pidConfig->delthaTime;

	pidConfig->ctrlP = pidConfig->gainP * actualError;
	pidConfig->ctrlI = pidConfig->gainI * pidConfig->ErrorI;
	pidConfig->ctrlD = pidConfig->gainD * pidConfig->ErrorD;

	pidConfig->lastTime = actualTime;

	pidConfig->lastError = actualError;

    return pidConfig->ctrlP + pidConfig->ctrlI + pidConfig->ctrlD;
}


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
float CtrlPID_CalculateWithInterval(pidCtrlHandle_t handle, float actualError, float delthaTime)
{
	pidCtrlConfig_t* pidConfig = (pidCtrlConfig_t*)handle;

	pidConfig->ErrorI += actualError * delthaTime;
	pidConfig->ErrorD = (actualError - pidConfig->lastError)/delthaTime;

	pidConfig->ctrlP = pidConfig->gainP * actualError;
	pidConfig->ctrlI = pidConfig->gainI * pidConfig->ErrorI;
	pidConfig->ctrlD = pidConfig->gainD * pidConfig->ErrorD;

	pidConfig->lastError = actualError;

    return pidConfig->ctrlP + pidConfig->ctrlI + pidConfig->ctrlD;
}
