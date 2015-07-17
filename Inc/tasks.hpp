/*
 * tasks.hpp
 *
 *  Created on: 05 февр. 2014 г.
 *      Author: pavlov
 */

#ifndef TASKS_HPP_
#define TASKS_HPP_

//#include <stm32f4xx_conf.h>
#include "stdint.h"
#include "stm32f4xx_hal.h"


#include "FreeRTOS.h"
#include "task.h"

/**
 * Task base class.
 */
class Task {
public:
	Task() {};
	virtual ~Task() {};

	/**
	 * Task initialization method, must be reimplemented in
	 * inherited classes.
	 */
	virtual void init() {};

	/**
	 * Task main loop implementation.
	 *
	 * @param pvParameters
	 */
	virtual void impl(void *pvParameters) {};
};

typedef void (Task::*FNMETHOD) (void *);

#endif /* TASKS_HPP_ */
