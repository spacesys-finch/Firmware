/*
 * sd_test.c
 *
 *  Created on: Dec 27, 2020
 *      Author: Tobias
 *
 * refer to the video here: https://controllerstech.com/sd-card-using-spi-in-stm32/
 * on how/what to expect for running the test
 */

#include <drivers/STM32_sd/fatfs_sd.h>
#include "string.h"
#include "stdio.h"

#ifdef STM32H743xx
#include <drivers/STM32H7_clock/clock.h>
#include "fatfs.h"
#endif

#ifdef STM32G474xx
#include <drivers/STM32G4_clock/clock.h>
#endif

//global variables
SPI_HandleTypeDef hspi;
UART_HandleTypeDef huart;
FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count
/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;
#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];  // to store strings..

//////////////////////////// helper functions /////////////////////////////////////////
int i=0;

int bufsize (char *buf)
{
	int i=0;
	while (*buf++ != '\0') i++;
	return i;
}

void clear_buffer (void)
{
	for (int i=0; i<BUFFER_SIZE; i++) buffer[i] = '\0';
}

void send_uart (char *string)
{
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart, (uint8_t *) string, len, HAL_MAX_DELAY);  // transmit in blocking mode
}


static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi.Instance = SPI1;
  hspi.Init.Mode = SPI_MODE_MASTER;
  hspi.Init.Direction = SPI_DIRECTION_2LINES;
  hspi.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi.Init.NSS = SPI_NSS_SOFT;
  hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi.Init.CRCPolynomial = 0x0;
  hspi.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
  hspi.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hspi->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PD7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }

}

static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart.Instance = USART1;
  huart.Init.BaudRate = 115200;
  huart.Init.WordLength = UART_WORDLENGTH_8B;
  huart.Init.StopBits = UART_STOPBITS_1;
  huart.Init.Parity = UART_PARITY_NONE;
  huart.Init.Mode = UART_MODE_TX_RX;
  huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart.Init.OverSampling = UART_OVERSAMPLING_16;
  huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB14     ------> USART1_TX
    PB15     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }

}


/////////////////////////////////////////////////////////////////////////////////////////

/**PIN CONFIGURATIONS

  SPI pin configurations, using SPI1 to interface with SD card
  PA5     ------> SPI1_SCK
  PA6     ------> SPI1_MISO
  PD7     ------> SPI1_MOSI
  PF3     ------> SPI1_CS
*/

#define CS_PORT GPIOF
#define CS_PIN GPIO_PIN_5

int main() {

	// Initialization
	HAL_Init();
	clock_init();
	MX_SPI1_Init();
	MX_USART1_UART_Init();
	init_sd_card_handler(hspi, CS_PORT, CS_PIN);
	MX_FATFS_Init();

	fresult = f_mount(&fs, "/", 1);
	if (fresult != FR_OK) send_uart ("ERROR!!! in mounting SD CARD...\n\n");
	else send_uart("SD CARD mounted successfully...\n\n");


	/*************** Card capacity details ********************/

	/* Check free space */
	f_getfree("", &fre_clust, &pfs);

	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	sprintf (buffer, "SD CARD Total Size: \t%lu\n",total);
	send_uart(buffer);
	clear_buffer();
	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
	sprintf (buffer, "SD CARD Free Space: \t%lu\n\n",free_space);
	send_uart(buffer);
	clear_buffer();



	/************* The following operation is using PUTS and GETS *********************/

	/* Open file to write/ create a file if it doesn't exist */
	fresult = f_open(&fil, "file1.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

	/* Writing text */
	f_puts("This data is from the FILE1.txt. And it was written using ...f_puts... ", &fil);

	/* Close file */
	fresult = f_close(&fil);

	if (fresult == FR_OK)send_uart ("File1.txt created and the data is written \n");

	/* Open file to read */
	fresult = f_open(&fil, "file1.txt", FA_READ);

	/* Read string from the file */
	f_gets(buffer, f_size(&fil), &fil);

	send_uart("File1.txt is opened and it contains the data as shown below\n");
	send_uart(buffer);
	send_uart("\n\n");

	/* Close file */
	f_close(&fil);

	clear_buffer();




	/**************** The following operation is using f_write and f_read **************************/

	/* Create second file with read write access and open it */
	fresult = f_open(&fil, "file2.txt", FA_CREATE_ALWAYS | FA_WRITE);

	/* Writing text */
	strcpy (buffer, "This is File2.txt, written using ...f_write... and it says Hello from Controllerstech\n");

	fresult = f_write(&fil, buffer, bufsize(buffer), &bw);

	send_uart ("File2.txt created and data is written\n");

	/* Close file */
	f_close(&fil);



	// clearing buffer to show that result obtained is from the file
	clear_buffer();

	/* Open second file to read */
	fresult = f_open(&fil, "file2.txt", FA_READ);
	if (fresult == FR_OK)send_uart ("file2.txt is open and the data is shown below\n");

	/* Read data from the file
	 * Please see the function details for the arguments */
	f_read (&fil, buffer, f_size(&fil), &br);
	send_uart(buffer);
	send_uart("\n\n");

	/* Close file */
	f_close(&fil);

	clear_buffer();


	/*********************UPDATING an existing file ***************************/

	/* Open the file with write access */
	fresult = f_open(&fil, "file2.txt", FA_OPEN_EXISTING | FA_READ | FA_WRITE);

	/* Move to offset to the end of the file */
	fresult = f_lseek(&fil, f_size(&fil));

	if (fresult == FR_OK)send_uart ("About to update the file2.txt\n");

	/* write the string to the file */
	fresult = f_puts("This is updated data and it should be in the end", &fil);

	f_close (&fil);

	clear_buffer();

	/* Open to read the file */
	fresult = f_open (&fil, "file2.txt", FA_READ);

	/* Read string from the file */
	fresult = f_read (&fil, buffer, f_size(&fil), &br);
	if (fresult == FR_OK)send_uart ("Below is the data from updated file2.txt\n");
	send_uart(buffer);
	send_uart("\n\n");

	/* Close file */
	f_close(&fil);

	clear_buffer();


	/*************************REMOVING FILES FROM THE DIRECTORY ****************************/

	fresult = f_unlink("/file1.txt");
	if (fresult == FR_OK) send_uart("file1.txt removed successfully...\n");

	fresult = f_unlink("/file2.txt");
	if (fresult == FR_OK) send_uart("file2.txt removed successfully...\n");

	/* Unmount SDCARD */
	fresult = f_mount(NULL, "/", 1);
	if (fresult == FR_OK) send_uart ("SD CARD UNMOUNTED successfully...\n");

	while (1) {

	}


	return 1;


}
