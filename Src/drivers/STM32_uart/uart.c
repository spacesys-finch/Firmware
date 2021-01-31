/*
 * usart.c
 *
 *  Created on: Oct 10, 2020
 *      Author: Ketan
 */

#include <drivers/STM32_uart/uart.h>

// alt - e.g. GPIO_AF7_USART3 for an instance of USART3
UART uart_init_base(USART_TypeDef *instance, uint32_t baud_rate, uint8_t alt,
		GPIO_TypeDef *tx_port, uint16_t tx_pin,
		GPIO_TypeDef *rx_port, uint16_t rx_pin) {

	__HAL_RCC_SYSCFG_CLK_ENABLE();

	UART uart;
	uart.handle.Instance = instance;
	uart.handle.Init.BaudRate = baud_rate;
	uart.handle.Init.WordLength = UART_WORDLENGTH_8B;
	uart.handle.Init.StopBits = UART_STOPBITS_1;
	uart.handle.Init.Parity = UART_PARITY_NONE;
	uart.handle.Init.Mode = UART_MODE_TX_RX;
	uart.handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.handle.Init.OverSampling = UART_OVERSAMPLING_16;
	uart.handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	uart.handle.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	uart.handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	if (HAL_UARTEx_SetTxFifoThreshold(&uart.handle, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&uart.handle, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&uart.handle) != HAL_OK) {
		Error_Handler();
	}

	//tx pin init
	uart.tx = gpio_init_alt(tx_port, tx_pin, alt);

	//rx pin init
	uart.rx = gpio_init_alt(rx_port, rx_pin, alt);

	// Peripheral clock enable for the appropriate UxART peripheral
	if(uart.handle.Instance==USART1) {
		__HAL_RCC_USART1_CLK_ENABLE();
	}
	if(uart.handle.Instance==USART2) {
		__HAL_RCC_USART2_CLK_ENABLE();
	}
	if(uart.handle.Instance==USART3) {
		__HAL_RCC_USART3_CLK_ENABLE();
	}
	if(uart.handle.Instance==UART4) {
		__HAL_RCC_UART4_CLK_ENABLE();
	}
	if(uart.handle.Instance==UART5) {
		__HAL_RCC_UART5_CLK_ENABLE();
	}
	if(uart.handle.Instance==USART6) {
		__HAL_RCC_USART6_CLK_ENABLE();
	}
	if(uart.handle.Instance==UART7) {
		__HAL_RCC_UART7_CLK_ENABLE();
	}
	if(uart.handle.Instance==UART8) {
		__HAL_RCC_UART8_CLK_ENABLE();
	}

	return uart;
}

// Initialize normal UART
UART uart_init(USART_TypeDef *instance, uint32_t baud_rate, uint8_t alt,
		GPIO_TypeDef *tx_port, uint16_t tx_pin,
		GPIO_TypeDef *rx_port, uint16_t rx_pin) {

	UART uart = uart_init_base(instance, baud_rate, alt, tx_port, tx_pin, rx_port, rx_pin);

	if (HAL_UART_Init(&uart.handle) != HAL_OK) {
		Error_Handler();
	}

	return uart;
}

// Initialize UART + RS-485
UART uart_init_rs485(USART_TypeDef *instance, uint32_t baud_rate, uint8_t alt,
		GPIO_TypeDef *tx_port, uint16_t tx_pin,
		GPIO_TypeDef *rx_port, uint16_t rx_pin,
		GPIO_TypeDef *de_port, uint16_t de_pin) {

	UART uart = uart_init_base(instance, baud_rate, alt, tx_port, tx_pin, rx_port, rx_pin);

	// RS-485 DE pin init
	uart.de = gpio_init_alt(de_port, de_pin, alt);

	if (HAL_RS485Ex_Init(&uart.handle, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK) {
		Error_Handler();
	}

	return uart;
}
