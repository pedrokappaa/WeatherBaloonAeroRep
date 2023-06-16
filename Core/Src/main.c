/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "usbd_cdc_if.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "fatfs_sd.h"
#include "fatfs_sd_api.h"
#include "balloon_utils.h"
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

uint16_t ADC_results[5]; //it will have ADC results
//ADC_results[0] = Temp STM
//ADC_results[1] = Temp PT100
//ADC_results[2] = V_LC ------ Para descobrir esta tensao tenho de multiplicar o valor do ADC por 3
//ADC_results[3] = V_AA ------ Para descobrir esta tensao tenho de multiplicar o valor do ADC por 2
//ADC_results[4] = V_9V ------ Para descobrir esta tensao tenho de multiplicar o valor do ADC por 3
char MSG[52] = ""; //Message to send

float B1; //battery 1 level
float B2; //battery 2 level
float B3; //battery 3 level
float T_PT100; //Temperature provided by PT100
float dT;//Temperature gradient provided by STM
int time; // to save the time -


void Build_MSG(){
	//builds the Message that will be sent from the STM(Master) to the Arduino(Slave)
	sprintf(MSG, "{t:%d;dT:%.2f;T:%.2f;B1:%.1f;B2:%.1f;B3:%.1f}",time,dT, T_PT100, B1, B2, B3);
}


void convert_adc_to_physicalvalue(){

	time += 2; //add 2s to time since 2s passed from the last measure

	float aux0, aux1, aux2, aux3, aux4;

	//Pass ADC result from bits to tension
	aux0 = ADC_results[0]*3.3/4096; //ADC result in Volts
	aux1 = ADC_results[1]*3.3/4096;
	aux2 = ADC_results[2]*3.3/4096;
	aux3 = ADC_results[3]*3.3/4096;
	aux4 = ADC_results[4]*3.3/4096;

	//Temperature line: Temperature = 34,469 * ADC_result - 67,653
	//Correction factor for V_LC: 3
	//Correction factor for V_AA: 2
	//Correction factor for V_9V: 3

	//Convert ADC result to temperature and voltages
	dT = 34.469*aux0 - 67.653; //STM temperature variation
	T_PT100 = 34.469*aux1 - 67.653; //PT100 temperature
	B1 = aux2 * 3 ; //V_LC
	B2 = aux3 * 2; //V_AA
	B3 = aux4 * 3; //V_9V

}


void Send_MSG(){
	//Send Message from STM(Master) to Arduino(Slave) using SPI4

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET); // CS Low => initiate communication between Master and Slave
	int i = 0;

	for(i=0;i<strlen(MSG);i++){
		char dataSend = MSG[i];
		HAL_SPI_Transmit(&hspi4, (uint8_t*)&dataSend, 1,100); //send byte by byte
	}
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); // CS High => End communication

}

void print_adc()
{
	char message[1024];
	sprintf((char*) message, "%d, %.2fV, %.2fV, %.2fV, %.2fV", ADC_results[0],
			ADC_results[1]*3.33/4096, ADC_results[2]*3.33/4096,
			ADC_results[3]*3.33/4096, ADC_results[4]*3.33/4096);
	send_UART(message);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); //CS of SPI4 High => No communication between STM and Arduino
  time = 0; //start time counting
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
  MX_DMA_Init();
  MX_FATFS_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_SPI1_Init();
  MX_SPI4_Init();
  /* USER CODE BEGIN 2 */

  	/*
	reset_UART();
	send_UART(PROMPT);

	if(SDCardInit())
		send_UART("true");

	SDCardWrite("Hello\n");

	if(SDCardEnd())
		send_UART("true");

	HAL_TIM_Base_Start_IT(&htim1); //start timer


	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ADC_results, 5);
	// implementar sliding window average

	*/


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	while (1)
	{

		if(ReturnFlagTimer()){
			//2s Passed

			convert_adc_to_physicalvalue(); // Convert adc values to Temperature and Voltages
			//print_adc();
			Build_MSG(); //build Message before sending
			Send_MSG(); //Send Message to Slave(Arduino)
			ClearFlagTimer(); //set flag = false
		}

/*
		if(has_message_from_UART())
		{
		  uint8_t message[BUFFER_SIZE];

		  read_UART((char*) message);

		  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);

		  send_UART((char*) message);

		  while(is_transmitting_to_UART());

		  reset_UART();
		  send_UART(PROMPT);
		}


		if(isButPressed())
		{
			butClear();
			send_UART("Button pressed");
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

			char message2[5];
			int adc = read_ADC();
			sprintf(message2, "%d\n", adc);
			send_UART(message2);
			reset_UART();
			send_UART(PROMPT);
		}
*/

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV6;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
