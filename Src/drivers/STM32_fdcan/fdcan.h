/*
 * fdcan.h
 *
 *  Created on: Jul. 8, 2020
 *      Author: ambrose
 */

#ifndef DRIVERS_STM32_FDCAN_FDCAN_H_
#define DRIVERS_STM32_FDCAN_FDCAN_H_

#include <drivers/STM32_gpio/gpio.h>
#include <stdint.h>

#ifdef STM32G474xx
#include <stm32g4xx_hal.h>
#endif

#ifdef STM32H743xx
#include <stm32h7xx_hal.h>
#endif

typedef struct {
	FDCAN_HandleTypeDef fdcan_handler;


} FDCAN;

#endif
