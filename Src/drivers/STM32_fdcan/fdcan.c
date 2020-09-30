#include "fdcan.h"



FDCAN fdcan_init(uint16_t instance, GPIO_TypeDef *port_high, GPIO_TypeDef *port_low, uint16_t high, uint16_t low, uint8_t alt)
{


  FDCAN_HandleTypeDef hfdcan1;

  FDCAN fdcan;


  GPIO_ALT fdcan_rx = gpio_init_alt(port_high, high, alt);
  GPIO_ALT fdcan_tx = gpio_init_alt(port_low, low, alt);

  hfdcan1.Instance = instance;
  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_FD_NO_BRS;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = DISABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 1;
  hfdcan1.Init.NominalSyncJumpWidth = 1;
  hfdcan1.Init.NominalTimeSeg1 = 2;
  hfdcan1.Init.NominalTimeSeg2 = 2;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 1;
  hfdcan1.Init.DataTimeSeg2 = 1;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }

  fdcan.fdcan_handler = hfdcan1;

  return fdcan;

}

//write

//read
