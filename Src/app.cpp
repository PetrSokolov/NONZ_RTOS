/*
 * app.cpp
 *
 *  Created on: 05 февр. 2014 г.
 *      Author: pavlov
 */
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usb_otg.h"
#include "gpio.h"

#include "app.hpp"

void Application::init() {
//	GPIO_InitTypeDef GPIO_InitStructure;
/*	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin =
			GPIO_Pin_12 |
			GPIO_Pin_13 |
			GPIO_Pin_14 |
			GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStructure);*/
}

void Application::start() {
	vTaskStartScheduler();
}

void Application::attach(Task *task,  char *title) {
	FNMETHOD impl = &Task::impl;
//typedef void (*TaskFunction_t)( void * )
  
//	pdTASK_CODE proc = ((pdTASK_CODE)(task->*impl));
	pdTASK_CODE proc;
  proc = ((pdTASK_CODE)(task->*impl));
	xTaskCreate(
		proc,
		(const char *) title,
		configMINIMAL_STACK_SIZE,
		task,
		2,
		(xTaskHandle *) 0);

}

void Application::attachTask(pdTASK_CODE task, char *title) {
	xTaskCreate(
		task,
		(const char *) title,
		configMINIMAL_STACK_SIZE,
		NULL,
		2,
		(xTaskHandle *) 0);

}

