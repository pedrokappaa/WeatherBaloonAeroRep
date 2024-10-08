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
uint8_t convCompleted = 0;	//Flag to check the state of the ADC conversions
uint16_t ADC_results[3]; 	//List with ADC results
char MSG[512]; 				//Message to send

float V_LC; 	//battery 1 level
float V_AA; 	//battery 2 level
float V_9V; 	//battery 3 level
float T_PT100;  //Temperature provided by PT100
float dT;		//Temperature gradient provided by STM
int time; 		//To save the time

bool flagSDEnd = false; 	//Flag to check if the SDCard was closed
bool switchFont = false;	//Flag to check if the energy supplier was already switched

void Build_MSG(){	//builds the Message that will be sent from the STM(Master) to the Arduino(Slave)
	sprintf(MSG, "%d;%.1f;%.1f;%.1f\n",time, V_LC, V_AA, V_9V);
}

void convert_adc_to_physicalvalue(){
	float aux1, aux2, aux3;

	//Pass ADC result from bits to tension
	aux1 = ADC_results[0]*3.3/4096;	//ADC result in Volts
	aux2 = ADC_results[1]*3.3/4096;
	aux3 = ADC_results[2]*3.3/4096;

	//Correction factor for V_LC: 3
	//Correction factor for V_AA: 2
	//Correction factor for V_9V: 3
	//Convert ADC result to temperature and voltages
	V_LC = aux1 * 3;
	V_AA = aux2 * 2;
	V_9V = aux3 * 3.3;
}

void Send_MSG()		//Send Message from STM(Master) to Arduino(Slave) using SPI4
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET); 		// CS Low => initiate communication between Master and Slave
	int i = 0;

	for(i=0;i<strlen(MSG);i++){
		char dataSend = MSG[i];
		HAL_SPI_Transmit(&hspi4, (uint8_t*)&dataSend, 1,100); 	//Send byte by byte
	}
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 		// CS High => End communication
}

void print_adc()
{
	reset_UART();
	send_UART(MSG);
}

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
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_SPI4_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); 	//Feed STM

  while(!isButPressed()); //wait to press STM bottom to feed LoRa and Datalogger
  butClear();

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);	//O Led vermelho (led dos erros) desligado
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_SET); 		//Feed Datalogger
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); 	//Alimentacao de BackUp off


  if(!SDCardInit()){
	  //Error - Turn on Error Led
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
  }

  HAL_Delay(3000);

  SDCardWrite("time;V_LC;V_AA;V_9V\n");

  time = 0; //start time counting

  reset_UART();
  send_UART(PROMPT);
  reset_UART();
  send_UART("time;V_LC;V_AA;V_9V");

  HAL_Delay(1000);

  HAL_TIM_Base_Start_IT(&htim1); //start timer

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	while (1)
	{
		if(ReturnFlagTimer()){
			//2s Passed
			time += 2; //add 2s to time since 2s passed from the last measure
			HAL_ADC_Start_DMA(&hadc1, (uint32_t *) ADC_results, 3);

			while(!convCompleted);

			convCompleted = 0;

			convert_adc_to_physicalvalue(); // Convert adc values to Temperature and Voltages
			Build_MSG(); //build Message before sending
			print_adc();

			if(!flagSDEnd){
				//Ainda nao foi realizado o End do cartao
				if(SDCardHasLowSpace()){
					//Low space at the SD card => End SD card
					if(!SDCardEnd()){
						//Nao conseguiu realizar o End com sucesso
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); //Acidionar led de erro
					}
					else{
						//Realizou-se o End do cartao
						flagSDEnd = true;
					}
				}
				else{
					//O cartao tem espaco ainda
					if(!SDCardWrite(MSG)){
						//Nao conseguiu escrever
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); //Acidionar led de erro
					}
				}
			}

			ClearFlagTimer(); //set flag = false
		}


		//Ver se e necessario trocar a fonte de alimentacao do datalogger
		if(V_9V <= MINIMUM_VOLTAGE_DATALOGGER){
			//A alimentacao do datalogger deve ser trocada para a backup
			if (!switchFont){
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // Ativar a Alimentacao de backup
				HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_RESET); //Desligar a alimentacao inicial do datalogger
				SDCardWrite("Switched to Backup Battery\n");
				switchFont = true;
			}
		}
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
