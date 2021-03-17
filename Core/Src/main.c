/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "quadspi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include <string.h>
#define SECTORS_COUNT 100
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_QUADSPI_Init();
  /* USER CODE BEGIN 2 */
  uint8_t buffer_test[MEMORY_SECTOR_SIZE];
        uint32_t var = 0;

    	CSP_QUADSPI_Init();

    	for (var = 0; var < MEMORY_SECTOR_SIZE; var++) {
    		buffer_test[var] = (var & 0xff);
    	}

    	for (var = 0; var < SECTORS_COUNT; var++) {

    		if (CSP_QSPI_EraseSector(var * MEMORY_SECTOR_SIZE,
    				(var + 1) * (MEMORY_SECTOR_SIZE - 1)) != HAL_OK) {

    			while (1)
    				;  //breakpoint - error detected
    		}

    		if (CSP_QSPI_WriteMemory(buffer_test, var * MEMORY_SECTOR_SIZE,
    				sizeof(buffer_test)) != HAL_OK) {

    			while (1)
    				;  //breakpoint - error detected
    		}

    	}


    	CSP_QSPI_EraseSector(0, 4095);
    	CSP_QSPI_EraseSector(4096, 8190);
//    	CSP_QSPI_WriteMemory(buffer_test, 0, sizeof(buffer_test));


    	uint8_t testData[3] = {8, 12, 20};
    	CSP_QSPI_WriteMemory(testData, 0, 3);
//
//    	QSPI_CommandTypeDef sCommand;
//    		uint8_t test_buffer[4] = { 0 };
//
//    		sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
//    		sCommand.Instruction = 0x0B;
//    		sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
//    		sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
//    		sCommand.Address = 0;
//    		sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//    		sCommand.DataMode = QSPI_DATA_1_LINE;
//    		sCommand.DummyCycles = 8;
//    		sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
//    		sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
//    		sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
//    		sCommand.NbData = 3;
//
//    		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
//    				!= HAL_OK) {
//    			return HAL_ERROR;
//    		}
//    		if (HAL_QSPI_Receive(&hqspi, test_buffer,
//    		HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
//    			return HAL_ERROR;
//    		}







    	if (CSP_QSPI_EnableMemoryMappedMode() != HAL_OK) {

    		while (1)
    			; //breakpoint - error detected
    	}

    	uint8_t firstValue = *(uint8_t*) (0x90000000 + 0 * MEMORY_SECTOR_SIZE);
//    	uint8_t value = 0;
    	for (var = 0; var < SECTORS_COUNT; var++) {
//    		value = *(uint8_t*) (0x90000000 + var * MEMORY_SECTOR_SIZE);
//    		HAL_Delay(400);
    		if (memcmp(buffer_test,
    				(uint8_t*) (0x90000000 + var * MEMORY_SECTOR_SIZE),
    				MEMORY_SECTOR_SIZE) != HAL_OK) {
    			while (1)
    				;  //breakpoint - error detected - otherwise QSPI works properly
    		}
    	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 116;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
