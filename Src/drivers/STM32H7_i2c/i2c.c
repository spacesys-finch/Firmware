/*
 * i2c.c
 *
 *  Created on: Sep. 2, 2020
 *      Author: Khalil Damouni
 */

#include <STM32H7_i2c/i2c.h>

I2C_HandleTypeDef hi2c;

enum i2c_operation { i2c_WRITE = 0, i2c_READ = 1};

int i2c_init(void)
{
    //Configuration of the I2C peripheral
    hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x10707DBC;
	hi2c1.Init.OwnAddress1 = 0;   		//I2C_ADDRESS;
	hi2c1.Init.AddressingMode = 0;  	//I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = 0; 	//I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;//0;
	hi2c1.Init.OwnAddress2Masks = 0;	//I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = 0;		//I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = 0;  		//I2C_NOSTRETCH_DISABLE;

    //Configuration of the device using specified parameters
    //Check if the above configuration was successfull
    if(HAL_I2C_Init(&hi2c) != HAL_OK)
    {
        //Initialization Error
        Error_Handler();
        return 1;
    }
    else
        return 0;
}

int i2c_deinit(void)
{
    if(HAL_I2C_Init(&hi2c) != HAL_OK)
    {
        //Deinitialization Error
        Error_Handler();
        return 1;
    }
    else
        return 0;
}

int i2c_parent_operation(enum i2c_operation op, I2C_HandleTypeDef* hi2c, uint16_t DevAddress, uint8_t* pData, uint16_t Size, uint32_t Timeout)
{
    if(i2c_operation == i2c_WRITE)
    {
        if(HAL_StatusTypeDef HAL_I2C_Master_Transmit (I2C_HandleTypeDef * hi2c, uint16_t DevAddress, uint8_t* pData, uint16_t Size, uint32_t Timeout) != HAL_OK)
        {
            //Transmittion Error
            Error_Handler();
            return 1;
        }

    }

    else if(i2c_operation == i2c_READ)
    {
        if(HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef * hi2c, uint16_t DevAddress, uint8_t* pData, uint16_t Size, uint32_t Timeout) != HAL_OK)
        {
            //Receiving Error
            Error_Handler();
            return 1;
        }
    }
    return 0;
}

int i2c_child_operation(enum i2c_operation op, I2C_HandleTypeDef* hi2c, uint16_t DevAddress, uint8_t* pData, uint16_t Size, uint32_t Timeout)
{
    if(i2c_operation == i2c_WRITE)
    {
        if(HAL_StatusTypeDef HAL_I2C_Slave_Transmit (I2C_HandleTypeDef * hi2c, uint16_t DevAddress, uint8_t* pData, uint16_t Size, uint32_t Timeout) != HAL_OK)
        {
            //Transmittion Error
            Error_Handler();
            return 1;
        }

    }

    else if(i2c_operation == i2c_READ)
    {
        if(HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef * hi2c, uint16_t DevAddress, uint8_t* pData, uint16_t Size, uint32_t Timeout) != HAL_OK)
        {
            //Receiving Error
            Error_Handler();
            return 1;
        }
    }
    return 0;
}

int i2c_mem_operation(enum i2c_operation op, I2C_HandleTypeDef * hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t * pData, uint16_t Size, uint32_t Timeout)
{
    if(i2c_operation == i2c_WRITE)
    {
        if(HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef * hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t * pData, uint16_t Size, uint32_t Timeout) != HAL_OK)
        {
            //Writing Error
            Error_Handler();
            return 1;
        }

    }

    else if(i2c_operation == i2c_READ)
    {
        if(HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef * hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t * pData, uint16_t Size, uint32_t Timeout) != HAL_OK)
        {
            //Reading Error
            Error_Handler();
            return 1;
        }
    }
    return 0;
}

/*
void Error_Handler(void)
{
    printf("ERROR\n");
}*/
