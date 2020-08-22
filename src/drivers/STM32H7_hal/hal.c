/*
 * hal.c
 *
 *  Created on: Aug. 22, 2020
 *      Author: Kanver
 */
#include "hal.h"

void hal_init(void)
{
	HAL_Init();
}

/**
  * Initializes the Global MSP. Called in HAL_Init()
  * to do the global low level hardware initialization.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
}
