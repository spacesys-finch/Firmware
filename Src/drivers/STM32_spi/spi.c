/*
 * spi.c
 *
 *  Created on: Jul 20, 2020
 *      Author: Tobias
 */


#include "spi.h"
SPI spi_create_wrapper(SPI_TypeDef *spi_instance, uint8_t poll_mode, GPIO_OUTPUT* chip_sel_list){
	/*
	Creates the spi wrapper data structure. To initialize spi, call spi_init after calling this function
	spi_instance: needs to one of the values defined by macros:
	SPI1, SPI2 or SPI3 ... etc. The number of available macros depends on MCU.
	example instantiation: spi(SPI2, false)
	poll_mode: 1 --> polling mode, 0 --> interrupt mode
	chip_sel_list: send in an array of type chip_sel_pin which is list of chip select pins
	to different devices
	*/
	SPI spi_wrapper;
	spi_wrapper.poll_mode = poll_mode;
	spi_wrapper.spi_handle.Instance = spi_instance;
	spi_wrapper.spi_handle.Init.Mode = SPI_MODE_MASTER;
	spi_wrapper.spi_handle.Init.Direction = SPI_DIRECTION_2LINES;
	spi_wrapper.spi_handle.Init.DataSize = SPI_DATASIZE_8BIT;
	spi_wrapper.spi_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
	spi_wrapper.spi_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
	spi_wrapper.spi_handle.Init.NSS = SPI_NSS_SOFT;
	spi_wrapper.spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	spi_wrapper.spi_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi_wrapper.spi_handle.Init.TIMode = SPI_TIMODE_DISABLE;
	spi_wrapper.spi_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spi_wrapper.spi_handle.Init.CRCPolynomial = 7;
	spi_wrapper.spi_handle.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	spi_wrapper.spi_handle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	spi_wrapper.chip_sel_list = chip_sel_list;

	spi_wrapper.transaction_timeout = 5000;
	return spi_wrapper;
}

void spi_init(SPI* spi_wrapper, GPIO_TypeDef *sck_port, uint16_t sck_pin, GPIO_TypeDef *mosi_port, uint16_t mosi_pin, GPIO_TypeDef *miso_port, uint16_t miso_pin, uint8_t alt_func_spi) {
	/*
	 * Call spi_create_wrapper before calling this
	 *
	 * sck_port and sck_pin are gpio port and pin settings for sck
	 * mosi_port and mosi_pin are gpio port and pin settings for mosi
	 * miso_port and miso_pin are gpio port and pin settings for miso
	 *
	 * alt_func_spi parameter
	 */
	SPIClock_Config(&(spi_wrapper->spi_handle));
	spi_wrapper->sck_wrapper = gpio_init_alt(sck_port, sck_pin, alt_func_spi);
	spi_wrapper->mosi_wrapper = gpio_init_alt(mosi_port, mosi_pin, alt_func_spi);
	spi_wrapper->miso_wrapper = gpio_init_alt(miso_port, miso_pin, alt_func_spi);
	if (HAL_SPI_Init(&(spi_wrapper->spi_handle)) != HAL_OK)
		{
			Error_Handler();
		}
}

void set_direction(SPI *spi_wrapper, uint32_t Direction) {
	spi_wrapper->spi_handle.Init.Direction = Direction;
}

void set_datasize(SPI *spi_wrapper, uint32_t DataSize) {
	spi_wrapper->spi_handle.Init.DataSize = DataSize;
}

void set_clkpolarity(SPI *spi_wrapper, uint32_t CLKPolarity) {
	spi_wrapper->spi_handle.Init.CLKPolarity = CLKPolarity;
}

void set_clkphase(SPI *spi_wrapper, uint32_t CLKPhase) {
	spi_wrapper->spi_handle.Init.CLKPhase = CLKPhase;
}

void set_nss(SPI *spi_wrapper, uint32_t NSS) {
	spi_wrapper->spi_handle.Init.NSS = NSS;
}

void set_baud_rate_prescaler(SPI *spi_wrapper, uint32_t BaudRatePrescaler) {
	spi_wrapper->spi_handle.Init.BaudRatePrescaler = BaudRatePrescaler;
}

void set_first_bit(SPI *spi_wrapper, uint32_t FirstBit) {
	spi_wrapper->spi_handle.Init.FirstBit = FirstBit;
}

void set_TI_mode(SPI *spi_wrapper, uint32_t TIMode) {
	spi_wrapper->spi_handle.Init.TIMode = TIMode;
}

void set_crc_calculation(SPI *spi_wrapper, uint32_t CRCCalculation) {
	spi_wrapper->spi_handle.Init.CRCCalculation = CRCCalculation;
}

void set_crc_polynomial(SPI *spi_wrapper, uint32_t CRCPolynomial) {
	spi_wrapper->spi_handle.Init.CRCPolynomial = CRCPolynomial;
}

void set_crc_length(SPI *spi_wrapper, uint32_t CRCLength) {
	spi_wrapper->spi_handle.Init.CRCLength = CRCLength;
}

void set_nssp_mode(SPI *spi_wrapper, uint32_t NSSPMode) {
	spi_wrapper->spi_handle.Init.NSSPMode = NSSPMode;
}

void check_init_constraints(SPI *spi_wrapper) {
	if (HAL_SPI_Init(&(spi_wrapper->spi_handle)) != HAL_OK)
	{
		Error_Handler();
	}
}

void set_timeout(SPI *spi_wrapper, uint32_t timeout_value) {
	spi_wrapper -> transaction_timeout = timeout_value;
}

// for now, just support polling mode
void spi_write(SPI *spi_wrapper, uint8_t *write_data, uint16_t write_data_size, uint32_t cs_index) {
	/* Used to write data from MCU to slave peripheral
	 *
	 * spi_wrapper: send in spi wrapper struct found in spi.h
	 *
	 * write_data: array of bytes being written to peripheral
	 *
	 * write_data_size: number of bytes, i.e. size of write data array
	 *
	 * cs_index:use this to identify which chip select pin is being set to determine
	 * which peripheral to talk to
	 */

	HAL_StatusTypeDef transfer_status;
	GPIO_PinState chip_enable = GPIO_PIN_RESET;
	GPIO_PinState chip_disable = GPIO_PIN_SET;

	HAL_GPIO_WritePin(spi_wrapper->chip_sel_list[cs_index].port, spi_wrapper->chip_sel_list[cs_index].pin, chip_disable);
	HAL_GPIO_WritePin(spi_wrapper->chip_sel_list[cs_index].port, spi_wrapper->chip_sel_list[cs_index].pin, chip_enable);
	transfer_status = HAL_SPI_Transmit(&(spi_wrapper->spi_handle), write_data, write_data_size, spi_wrapper->transaction_timeout);

	if (transfer_status != HAL_OK) {
		//To do: Add in error checking
	}

	HAL_GPIO_WritePin(spi_wrapper->chip_sel_list[cs_index].port, spi_wrapper->chip_sel_list[cs_index].pin, chip_disable);
}

//Read data from slave device. read_data is where the data will be stored into.
void spi_read(SPI *spi_wrapper, uint8_t *read_data, uint16_t read_data_size, uint32_t cs_index) {
	/* Used to read data from MCU to slave peripheral
	 *
	 * spi_wrapper: send in spi wrapper struct found in spi.h
	 *
	 * read_data: array of bytes being read to from peripheral
	 *
	 * read_data_size: number of bytes, i.e. size of read data array
	 *
	 * cs_index:use this to identify which chip select pin is being set to determine
	 * which peripheral to talk to
	 */

	HAL_StatusTypeDef transfer_status;
	GPIO_PinState chip_enable = GPIO_PIN_RESET;
	GPIO_PinState chip_disable = GPIO_PIN_SET;

	HAL_GPIO_WritePin(spi_wrapper->chip_sel_list[cs_index].port, spi_wrapper->chip_sel_list[cs_index].pin, chip_disable);
	HAL_GPIO_WritePin(spi_wrapper->chip_sel_list[cs_index].port, spi_wrapper->chip_sel_list[cs_index].pin, chip_enable);
	transfer_status = HAL_SPI_Receive(&(spi_wrapper->spi_handle), read_data, read_data_size, spi_wrapper->transaction_timeout);
	if (transfer_status != HAL_OK) {
		//To do: Add in error checking
	}

	HAL_GPIO_WritePin(spi_wrapper->chip_sel_list[cs_index].port, spi_wrapper->chip_sel_list[cs_index].pin, chip_disable);
}

void spi_read_write(SPI *spi_wrapper, uint8_t *read_data, uint8_t *write_data, uint16_t data_size, uint32_t cs_index) {
	/* Used to read and write data b/w MCU and slave peripheral simultaneously
	 *
	 * spi_wrapper: send in spi wrapper struct found in spi.h
	 *
	 * read_data: array of bytes being read to from peripheral
	 *
	 * write_data: array of bytes being written to peripheral
	 *
	 * data_size: number of bytes being read and written, i.e. size of arrays
	 *
	 * cs_index:use this to identify which chip select pin is being set to determine
	 * which peripheral to talk to
	 */

	HAL_StatusTypeDef transfer_status;
	GPIO_PinState chip_enable = GPIO_PIN_RESET;
	GPIO_PinState chip_disable = GPIO_PIN_SET;

	HAL_GPIO_WritePin(spi_wrapper->chip_sel_list[cs_index].port, spi_wrapper->chip_sel_list[cs_index].pin, chip_disable);
	HAL_GPIO_WritePin(spi_wrapper->chip_sel_list[cs_index].port, spi_wrapper->chip_sel_list[cs_index].pin, chip_enable);

	for (uint16_t i = 0; i < data_size; i++) {
		transfer_status = HAL_SPI_TransmitReceive(&(spi_wrapper->spi_handle), &write_data[i], &read_data[i], 1, spi_wrapper->transaction_timeout);
		if (transfer_status != HAL_OK) {
			//To do: Add in error checking
		}
	}

	HAL_GPIO_WritePin(spi_wrapper->chip_sel_list[cs_index].port, spi_wrapper->chip_sel_list[cs_index].pin, chip_disable);
}
