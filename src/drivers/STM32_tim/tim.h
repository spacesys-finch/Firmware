/*
 * tim.h
 *
 *  Created on: Jul 22, 2020
 *      Author: Cameron Rodriguez
 */

#ifndef DRIVERS_STM32_TIM_TIM_H_
#define DRIVERS_STM32_TIM_TIM_H_

#include "../../Core/Inc/main.h"

/* User created struct to hold timer mode data, to be filled as follows:
 * - Mode is always required, see constants
 * - All functions except Base use channel for timer channel to use; OnePulse mode also uses output_channel and input channel
 * - OnePulse function also requires onepulse_mode (_SINGLE/_REPETITIVE)
 * - DMA mode uses src_addr for data source address; Encoder also uses src_addr2
 */
typedef struct {
	uint8_t mode;
	uint32_t channel;
	uint32_t output_channel;
	uint32_t input_channel;
	uint32_t onepulse_mode;
	uint32_t *src_addr;
	uint32_t *src_addr2;
} TimMode;

// Struct for timer handles and values, generated by the driver
typedef struct {
	TIM_HandleTypeDef *handle;
	TIM_IC_InitTypeDef *ic_cfg;
	TIM_OC_InitTypeDef *oc_cfg;
	TIM_OnePulse_InitTypeDef *onepulse_cfg;
	TIM_Encoder_InitTypeDef *encoder_cfg;
	TIM_ClockConfigTypeDef *clk_cfg;
	TIM_SlaveConfigTypeDef *slave_cfg;
	uint8_t function;
	TimMode *mode;
} TimFunc;

// Constants for timer functions and modes
enum tim_mode_constants {TIM_Mode_None, TIM_Mode_IT, TIM_Mode_DMA};
enum tim_func_constants {TIM_Base, TIM_IC, TIM_OC, TIM_PWM, TIM_OnePulse, TIM_Encoder};

// Helper calculator macros
#define tim_calc_prescaler(tim_clk, count_clk) 				__HAL_TIM_CALC_PSC(tim_clk, count_clk)
#define tim_calc_period(tim_clk, prescaler, frequency) 		__HAL_TIM_CALC_PERIOD(tim_clk, prescaler, frequency)
#define tim_calc_pulse(tim_clk, prescaler, us_delay)		__HAL_TIM_CALC_PULSE(tim_clk, prescaler, us_delay)
#define tim_calc_pulse_dither(tim_clk, prescaler, us_delay)	__HAL_TIM_CALC_PULSE_DITHER(tim_clk, prescaler, us_delay)

// User-friendly macros for counter, interrupt flags, clock, and slave
#define tim_get_counter(handle) 							__HAL_TIM_GET_COUNTER(handle)
#define tim_enable_IT_flag(handle, flag)					__HAL_TIM_ENABLE_IT(handle, flag)
#define tim_disable_IT_flag(handle, flag)					__HAL_TIM_DISABLE_IT(handle, flag)
#define tim_get_IT_flag(handle, flag)						__HAL_TIM_GET_FLAG(handle, flag)
#define tim_clear_IT_flag(handle, flag)						__HAL_TIM_CLEAR_FLAG(handle, flag)
#define tim_config_clock(handle, clk_cfg)					HAL_TIM_ConfigClockSource(handle, clk_cfg)
#define tim_config_slave(handle, slave_cfg)					HAL_TIM_SlaveConfigSynchro(handle, slave_cfg)
#define tim_config_slave_IT(handle, slave_cfg)				HAL_TIM_SlaveConfigSynchro(handle, slave_cfg)

// Timer config functions
TIM_HandleTypeDef *tim_populate_handle(TIM_HandleTypeDef *handle, TIM_TypeDef *instance, uint32_t prescaler, uint32_t time_counter_mode, uint32_t period,
		uint32_t clock_division, uint32_t repitition_counter, uint32_t autoreload_preload);
TimFunc *tim_init_struct();
TIM_HandleTypeDef *tim_init_handle(TIM_TypeDef *instance, uint32_t prescaler, uint32_t period);
TimFunc *tim_config_base(TIM_TypeDef *instance, TimMode *mode, uint32_t prescaler, uint32_t period);
TimFunc *tim_config_ic(TIM_TypeDef *instance, TimMode *mode, uint32_t prescaler, uint32_t period, uint32_t ic_polarity,
		uint32_t ic_selection, uint32_t ic_prescaler, uint32_t ic_filter);
TimFunc *tim_config_oc(TIM_TypeDef *instance, TimMode *mode, uint32_t prescaler, uint32_t period, uint32_t oc_mode, uint32_t oc_pulse,
		uint32_t oc_polarity);
TimFunc *tim_config_pwm(TIM_TypeDef *instance, TimMode *mode, uint32_t prescaler, uint32_t period, uint32_t pwm_mode, uint32_t pwm_pulse,
		uint32_t pwm_polarity);
TimFunc *tim_config_onepulse(TIM_TypeDef *instance, TimMode *mode, uint32_t prescaler, uint32_t period, uint32_t oc_mode, uint32_t oc_pulse,
		uint32_t oc_polarity, uint32_t ic_polarity, uint32_t ic_selection, uint32_t ic_filter);


// Timer init/start/stop functions
uint8_t tim_init(TIM_HandleTypeDef *handle, uint8_t function, uint32_t onepulse_mode, TIM_Encoder_InitTypeDef *encoder_cfg);
uint8_t tim_deinit(TIM_HandleTypeDef *handle, uint8_t function);
uint8_t tim_start(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel);
uint8_t tim_start_IT(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel);
uint8_t tim_start_DMA(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel, uint32_t *src_addr, uint32_t *src_addr2, uint16_t length);
uint8_t tim_stop(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel);
uint8_t tim_stop_IT(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel);
uint8_t tim_stop_DMA(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel);

#endif /* DRIVERS_STM32_TIM_TIM_H_ */
