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
#include "fatfs.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "usbd_cdc_if.h"
#include "string.h"
#include "fatfs_sd.h"
#include <stdio.h>
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

<<<<<<< Updated upstream
// SD Card Init

FATFS fs;						// file system data
FIL fil;						// file pointer
FRESULT fresult;				// to store the result
char buffer[BUFFER_SIZE];		// to store data
UINT br, bw;					// file r/w count
FATFS *pfs;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;


//Construir a mensagem a enviar para o slave
void ConstMsg(char* Msg, float T_PT100, float T_STM, float B1, float B2, float B3){

	if(T_PT100>=0 && T_STM>=0){
		sprintf(Msg, "{T1:+%.2f;T2:+%.2f;B1:%.1f;B2:%.1f;B3:%.1f}",T_PT100, T_STM, B1, B2, B3);
	}

	if(T_PT100>=0 && T_STM<0){
		sprintf(Msg, "{T1:+%.2f;T2:-%.2f;B1:%.1f;B2:%.1f;B3:%.1f}",T_PT100, T_STM, B1, B2, B3);
	}

	if(T_PT100<0 && T_STM>=0){
		sprintf(Msg, "{T1:-%.2f;T2:+%.2f;B1:%.1f;B2:%.1f;B3:%.1f}",T_PT100, T_STM, B1, B2, B3);
	}

	if(T_PT100<0 && T_STM<0){
		sprintf(Msg, "{T1:-%.2f;T2:-%.2f;B1:%.1f;B2:%.1f;B3:%.1f}",T_PT100, T_STM, B1, B2, B3);
	}
=======
uint16_t ADC_results[5];
char MSG[42] = ""; //Message to send to arduino
float B1 = 0; //battery 1 level
float B2 = 0; //battery 2 level
float B3 = 0; //battery 3 level
float T_PT100 = 0; //Temperature provided by PT100
float T_STM = 0;//Temperature provided by STM

void Build_MSG(){
	//builds the Message that will be sent from the STM(Master) to the Arduino(Slave)
	sprintf(MSG, "{T1:%.2f;T2:%.2f;B1:%.1f;B2:%.1f;B3:%.1f}",T_PT100, T_STM, B1, B2, B3);
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
>>>>>>> Stashed changes

}

//Envio de dados para o Slave
void SendToSlave(char* MSG, int lenMSG){

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET); // colocar a CS em Low para iniciar a comunicacao
	HAL_Delay(10);
	int i = 0;
	for(i=0;i<strlen(MSG);i++){
		char dataSend = MSG[i];
		HAL_SPI_Transmit(&hspi4, (uint8_t*)&dataSend, 1,100); //realizar a transmissao byte a byte
		HAL_Delay(10); //esperar 10ms

	}
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); // Acabar comunicacao
	HAL_Delay(50); //esperar 50ms
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
<<<<<<< Updated upstream
	/* USER CODE BEGIN 1 */
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); // CS High => No communication between Slave (Arduino) and Master (STM)
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/
=======
  /* USER CODE BEGIN 1 */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); //CS of SPI4 High => No communication between STM and Arduino
  /* USER CODE END 1 */
>>>>>>> Stashed changes

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
	MX_ADC3_Init();
	MX_SPI1_Init();
	MX_FATFS_Init();
	MX_USART3_UART_Init();
	/* USER CODE BEGIN 2 */

	reset_UART();
	send_UART(PROMPT);

	// Wait for SD module reset
	HAL_Delay(500);

  	// Mount SD Card
  	if(f_mount(&fs, "", 0) != FR_OK)
  		send_UART("Not mounted");

  	HAL_Delay(500);

<<<<<<< Updated upstream
  	// Open file to write
  	if(f_open(&fil, "first.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) != FR_OK)
  		send_UART("Not opened");
=======
	HAL_TIM_Base_Start_IT(&htim1); //start timer

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ADC_results, 5);
	// implementar sliding window average
>>>>>>> Stashed changes

  	HAL_Delay(500);

  	// Check freeSpace space
  	if(f_getfree("", &fre_clust, &pfs) != FR_OK)
  		send_UART("Not free");

  	HAL_Delay(500);

  	totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
  	freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);

  	HAL_Delay(500);

  	// free space is less than 1kb
  	if(freeSpace < 1)
  		send_UART("Low storage");

  	HAL_Delay(500);

  	// Writing text
  	f_puts("STM32 SD Card I/O Example via SPI\n", &fil);
  	f_puts("Save the world!!!", &fil);

  	HAL_Delay(500);

  	// Close file
  	if(f_close(&fil) != FR_OK)
  		send_UART("Not closed");

  	HAL_Delay(500);

  	// Open file to read
  	if(f_open(&fil, "first.txt", FA_READ) != FR_OK)
  		send_UART("Not opened");

  	HAL_Delay(500);

  	while(f_gets(buffer, sizeof(buffer), &fil))
  	{
  		send_UART(buffer);
  	}

  	HAL_Delay(500);

  	// Close file
  	if(f_close(&fil) != FR_OK)
  		send_UART("Not closed");

  	HAL_Delay(500);

  	// Unmount SDCARD
  	if(f_mount(NULL, "", 1) != FR_OK)
  		send_UART("Not unmounted");

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

	while (1)
	{
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

			/*
			char message2[5];
			int adc = read_ADC();
			sprintf(message2, "%d\n", adc);
			send_UART(message2);
			reset_UART();
			send_UART(PROMPT);
			*/
		}

<<<<<<< Updated upstream
		//Test values
		float T_PT100 = 20.345689; // PT100 temperature
		float T_STM = 19.452389; // STM temperature
		float B1 = 4.56756; //battery 1 level
		float B2 = 3.23453; //battery 2 level
		float B3 = 5.001; //battery 3 level


		char MSG[42] = ""; //Message to send
		ConstMsg(MSG, T_PT100, T_STM, B1, B2, B3); // build Message
		SendToSlave(MSG, strlen(MSG)); //Send message MSG to Slave
=======
>>>>>>> Stashed changes

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
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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
