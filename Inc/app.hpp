/*
 * app.hpp
 *
 *  Created on: 05 февр. 2014 г.
 *      Author: pavlov
 */

#ifndef APP_HPP_
#define APP_HPP_

//#include <stm32f4xx_conf.h>
//#include "stdint.h"
//#include "stm32f4xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#include "tasks.hpp"

/**
 * Application main file.
 */
class Application {
public:
	Application() {}

	/**
	 * Initialize an application.
	 */
	void init();

	/**
	 * Start an application main loop.
	 */
	void start();

	/**
	 * Attach and run task written in plain C.
	 *
	 * @param taskCode
	 * @param title
	 */
	void attachTask(pdTASK_CODE taskCode, char * title);

	/**
	 * Attach and run task written in C++, inherited from Task.
	 *
	 * @param task
	 * @param title
	 */
	void attach(Task *task, char *title);
};



#endif /* APP_HPP_ */
