/*
 * LEDTask.cpp
 *
 *  Created on: 05 февр. 2014 г.
 *      Author: pavlov
 */

#include "ledtask.hpp"

LEDTask::LEDTask(uint16_t ledNumber, uint16_t time) : Task() {
	mTime = time;
	mPin = ledNumber;
}

void LEDTask::init(void) {

}

inline void LEDTask::impl(void *pvParameters) {
	for (;;) {
//		GPIO_SetBits(GPIOD, mPin);
        vTaskDelay(mTime);
//        GPIO_ResetBits(GPIOD, mPin);
        vTaskDelay(mTime);
	}
}
