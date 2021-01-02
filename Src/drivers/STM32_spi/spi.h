/*
 * spi.h
 *
 *  Created on: Jul 20, 2020
 *      Author: Tobias
 */

#ifndef DRIVERS_STM32_SPI_SPI_H_
#define DRIVERS_STM32_SPI_SPI_H_

#include <drivers/STM32_gpio/gpio.h>

#ifdef STM32H743xx
#include <stm32h7xx_hal.h>
#include <stm32h7_startup.h>
#include <drivers/STM32H7_clock/clock.h>
#endif

#ifdef STM32G474xx
#include <stm32g4xx_hal.h>
#include <stm32g4_startup.h>
#include <drivers/STM32G4_clock/clock.h>
#endif
//spi wrapper struct
typedef struct {
	SPI_HandleTypeDef spi_handle;
	uint8_t poll_mode;
	GPIO_OUTPUT* chip_sel_list;
	GPIO_ALT sck_wrapper;
	GPIO_ALT miso_wrapper;
	GPIO_ALT mosi_wrapper;
	uint32_t transaction_timeout;
}SPI;

// spi_create_wrapper creates spi_wrapper data struct. spi_init initializes spi hardware setup.
SPI spi_create_wrapper(SPI_TypeDef *spi_instance, uint8_t poll_mode, GPIO_OUTPUT* chip_sel_list);
void spi_init(SPI* spi_wrapper, GPIO_TypeDef *sck_port, uint16_t sck_pin, GPIO_TypeDef *mosi_port, uint16_t mosi_pin, GPIO_TypeDef *miso_port, uint16_t miso_pin, uint8_t alt_func_spi);

/* Functions to modify spi initialization pin settings.
The constructor sets default pin settings. Make sure to run
check_init_constraints() once you've made all required changes.*/
void set_direction(SPI *spi_wrapper, uint32_t Direction);
void set_datasize(SPI *spi_wrapper, uint32_t DataSize);
void set_clkpolarity(SPI *spi_wrapper, uint32_t CLKPolarity);
void set_clkphase(SPI *spi_wrapper, uint32_t CLKPhase);
void set_nss(SPI *spi_wrapper, uint32_t NSS);
void set_baud_rate_prescaler(SPI *spi_wrapper, uint32_t BaudRatePrescaler);
void set_first_bit(SPI *spi_wrapper, uint32_t FirstBit);
void set_TI_mode(SPI *spi_wrapper, uint32_t TIMode);
void set_crc_calculation(SPI *spi_wrapper, uint32_t CRCCalculation);
void set_crc_polynomial(SPI *spi_wrapper, uint32_t CRCPolynomial);
void set_crc_length(SPI *spi_wrapper, uint32_t CRCLength);
void set_nssp_mode(SPI *spi_wrapper, uint32_t NSSPMode);

// function to set spi transaction timeout value
void set_timeout(SPI *spi_wrapper, uint32_t timeout_value);

void check_init_constraints(SPI *spi_wrapper);

void spi_write(SPI *spi_wrapper, uint8_t *write_data, uint16_t write_data_size, uint32_t cs_index);
void spi_read(SPI *spi_wrapper, uint8_t *read_data, uint16_t read_data_size, uint32_t cs_index);
void spi_read_write(SPI *spi_wrapper, uint8_t *read_data, uint8_t *write_data, uint16_t data_size, uint32_t cs_index);


#endif /* DRIVERS_STM32_SPI_SPI_H_ */
