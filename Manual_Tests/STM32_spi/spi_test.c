/*
 * spi.c
 *
 * spi testing functions
 *
 *  Created on: Jul 25, 2020
 *      Author: Tobias
 */

/*
 * Calibrate cap1188 capacitive touch sensor via SPI.
 *
 * Refer to https://docs.google.com/document/d/1YG3rkn8PKWSYPsniOojgW1znO4DWw4LhHtIniT5LMFM/edit
 * on how to set up the circuitry. However the pin locations are different
 *
 * datasheet for cap1188 sensor: https://cdn-shop.adafruit.com/datasheets/CAP1188.pdf
 *
 * For H7 mcu, user these pins:
 * PB5     ------> MOSI
 * PA5     ------> SCK
 * PA6     ------> MISO
 * PF3      ------> Chip select
 *
 * For G4 mcu, user these pins:
 * PB13     ------> MOSI
 * PB14     ------> SCK
 * PB15     ------> MISO
 * PC2      ------> Chip select
 *
 */

#include <drivers/STM32_spi/spi.h>
#ifdef STM32H743xx
	#include <drivers/STM32H7_clock/clock.h>
	#define SPI_INST SPI1
	#define SCK_PORT GPIOA
	#define SCK_PIN GPIO_PIN_5
	#define MISO_PORT GPIOA
	#define MISO_PIN GPIO_PIN_6
	#define MOSI_PORT GPIOB
	#define MOSI_PIN GPIO_PIN_5
	#define CHIP_SEL_PORT GPIOF
	#define CHIP_SEL_PIN GPIO_PIN_3
	#define ALT_FUNC_SPI GPIO_AF5_SPI1
#endif

#ifdef STM32G474xx
	#include <drivers/STM32G4_clock/clock.h>
	#define SPI_INST SPI2
	#define SCK_PORT GPIOB
	#define SCK_PIN GPIO_PIN_13
	#define MISO_PORT GPIOB
	#define MISO_PIN GPIO_PIN_14
	#define MOSI_PORT GPIOB
	#define MOSI_PIN GPIO_PIN_15
	#define CHIP_SEL_PORT GPIOC
	#define CHIP_SEL_PIN GPIO_PIN_2
	#define ALT_FUNC_SPI GPIO_AF5_SPI2
#endif


#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
#define BUFFERSIZE                       (COUNTOF(setCap1188MultiTouchAddrBuffer))
#define SET_CAP1188_SPI_ADDR (uint8_t)0x7DU
#define SET_CAP1188_SPI_WRITE (uint8_t)0x7EU
#define CAP1188_MULTITOUCH_ADDR (uint8_t)0x2AU
#define ENABLE_MULTITOUCH (uint8_t)0x00U
#define CAP1188_LED_LINK_ADDR (uint8_t)0x72U
#define CAP1188_LED_LINK (uint8_t)0xFFU

uint8_t setCap1188MultiTouchAddrBuffer[] = { SET_CAP1188_SPI_ADDR, CAP1188_MULTITOUCH_ADDR };
uint8_t setCap1188LedLinkAddrBuffer[] = { SET_CAP1188_SPI_ADDR, CAP1188_LED_LINK_ADDR };
uint8_t setCap1188MultiTouchCmd[] = {SET_CAP1188_SPI_WRITE, ENABLE_MULTITOUCH};
uint8_t setCap1188LedLinkCmd[] = {SET_CAP1188_SPI_WRITE, CAP1188_LED_LINK};
uint8_t read_buffer[BUFFERSIZE];

int main () {
	HAL_Init();
	clock_init();
	
	GPIO_OUTPUT cap_1188_chipsel;
	cap_1188_chipsel.port = CHIP_SEL_PORT;
	cap_1188_chipsel.pin = CHIP_SEL_PIN;

	gpio_init_output(cap_1188_chipsel.port, cap_1188_chipsel.pin);

	SPI cap_1188_spi_wrapper;
	cap_1188_spi_wrapper = spi_create_wrapper(SPI_INST, 1, &cap_1188_chipsel);
	spi_init(&cap_1188_spi_wrapper, SCK_PORT, SCK_PIN, MOSI_PORT, MOSI_PIN, MISO_PORT, MISO_PIN, ALT_FUNC_SPI);

	// set up multi touching for SPI
	spi_read_write(&cap_1188_spi_wrapper, read_buffer, setCap1188MultiTouchAddrBuffer, BUFFERSIZE, 0);
	spi_read_write(&cap_1188_spi_wrapper, read_buffer, setCap1188MultiTouchCmd, BUFFERSIZE, 0);

	// set up led blinking for when touching the sensors
	spi_read_write(&cap_1188_spi_wrapper, read_buffer, setCap1188LedLinkAddrBuffer, BUFFERSIZE, 0);
	spi_read_write(&cap_1188_spi_wrapper, read_buffer, setCap1188LedLinkCmd, BUFFERSIZE, 0);

	while (1) {

	}


	return 1;

}
