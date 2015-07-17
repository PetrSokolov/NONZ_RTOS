/*
 * LEDTask.hpp
 *
 *  Created on: 05 февр. 2014 г.
 *      Author: pavlov
 */

#ifndef LEDTASK_HPP_
#define LEDTASK_HPP_

#include <tasks.hpp>

class LEDTask: public Task {
private:
	uint16_t	mTime;
	uint16_t 	mPin;

public:
	LEDTask(uint16_t, uint16_t);
	virtual ~LEDTask() {};
	virtual void init();
	virtual void impl(void *pvParameters);
};

#endif /* LEDTASK_HPP_ */
