
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "stdlib.h"


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
static uint8_t DEBUG=0; // if DEBUG =1 It activates the USART1 for Testing
static uint8_t POWER[2]={0x6B,0}; //6B is the power management register
static uint8_t DATA_REGISTER=0x3B;
static const uint16_t MPU_ADDRESS=0xD2;
static unsigned char MPU_6050_INIT_STRING[]="\r\nInitialisng Sensors";
static unsigned char READING_START[]="\r\nREADING_STARTED ";
static unsigned char READING_END[]="READING_ENDED";
static unsigned char RECORDING_START[]="\r\nBATCH STARTED\r\n";
static unsigned char RECORDING_END[]="\r\nBATCH ENDED\r\n";
static uint8_t NO_OF_SENSORS=5;
static int PRED_BTN = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void MPU_6050_INIT(uint8_t);
void TRANSMIT_BT(uint8_t);
void ACTIVATE_SENSOR(uint8_t);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	int SENSOR_NO;
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */


	for(SENSOR_NO=1;SENSOR_NO<=NO_OF_SENSORS;SENSOR_NO++)
  {MPU_6050_INIT(SENSOR_NO);} //INITIATE THE SENSORS USING MULTIPLEXING
	
	MPU_6050_INIT(0); //DEACTIVATE THE MULTIPLEXER 
	HAL_Delay(1000);
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	
		if (PRED_BTN == 1 && HAL_GPIO_ReadPin(TOUCH_INPUT_GPIO_Port,TOUCH_INPUT_Pin))
  {
    if(DEBUG)
		{HAL_UART_Transmit(&huart1,RECORDING_START,sizeof(RECORDING_START),100);}
		
		HAL_UART_Transmit(&huart3,RECORDING_START,sizeof(RECORDING_START),100);
		
		PRED_BTN=0;
		
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
  
	}
		
		else if(HAL_GPIO_ReadPin(TOUCH_INPUT_GPIO_Port,TOUCH_INPUT_Pin))
		{
			
					if(DEBUG)
					{
						HAL_UART_Transmit(&huart1,READING_START,sizeof(READING_START),100);
					}
					
					while(HAL_UART_Transmit(&huart3,READING_START,sizeof(READING_START),100)!=HAL_OK)
					{;}
					
					for(SENSOR_NO=1;SENSOR_NO<=NO_OF_SENSORS;SENSOR_NO++)
					{
							TRANSMIT_BT(SENSOR_NO);
					
					}
					if(DEBUG)
					{
						HAL_UART_Transmit(&huart1,READING_END,sizeof(READING_END),100);
					}
					
					while(HAL_UART_Transmit(&huart3,READING_END,sizeof(READING_END),100)!=HAL_OK)
					{;}	
					
					
		}
		
		
		//IF BATCH ENDS
		
		else if (PRED_BTN == 0 && !(HAL_GPIO_ReadPin(TOUCH_INPUT_GPIO_Port,TOUCH_INPUT_Pin)))
		{
			if(DEBUG)
			{HAL_UART_Transmit(&huart1,RECORDING_END,sizeof(RECORDING_END),100);}
			
			while(HAL_UART_Transmit(&huart3,RECORDING_END,sizeof(RECORDING_END),100)!=HAL_OK)
			{;}
			PRED_BTN=1;
			HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
		
		}
		
		
		
		
		
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  } //While's end
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_16_9;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SENSOR_1_Pin|SENSOR_2_Pin|SENSOR_3_Pin|SENSOR_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SENSOR_5_GPIO_Port, SENSOR_5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SENSOR_1_Pin SENSOR_2_Pin SENSOR_3_Pin SENSOR_4_Pin */
  GPIO_InitStruct.Pin = SENSOR_1_Pin|SENSOR_2_Pin|SENSOR_3_Pin|SENSOR_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SENSOR_5_Pin */
  GPIO_InitStruct.Pin = SENSOR_5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SENSOR_5_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TOUCH_INPUT_Pin */
  GPIO_InitStruct.Pin = TOUCH_INPUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(TOUCH_INPUT_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
static void MPU_6050_INIT(uint8_t Sensor_Number)
{
	ACTIVATE_SENSOR(Sensor_Number);	
	
	while(HAL_I2C_IsDeviceReady(&hi2c1,MPU_ADDRESS,5,1000)!=HAL_OK) 
	{
		HAL_UART_Transmit(&huart1,MPU_6050_INIT_STRING,sizeof(MPU_6050_INIT_STRING),100);
		HAL_UART_Transmit(&huart3,MPU_6050_INIT_STRING,sizeof(MPU_6050_INIT_STRING),100);
		
	}
	while(HAL_I2C_IsDeviceReady(&hi2c1,MPU_ADDRESS,10,100)!=HAL_OK)
	{;}
	while(HAL_I2C_Master_Transmit(&hi2c1,MPU_ADDRESS,POWER,sizeof(POWER),100)!=HAL_OK)
	{;}
	
}
	
static void TRANSMIT_BT(uint8_t Sensor_Number)
{			
			/* Activate the required Sensor
				 Read the data using I2C
				 Transmit the data using BT( if DEBUG=1) then Also Transmit over the Serial For Testing
			*/
			
			ACTIVATE_SENSOR(Sensor_Number);
			
			unsigned char data[14]; //DATA Buffer for storing the I2C Data
			uint16_t AccX,AccY,AccZ,GyrX,GyrY,GyrZ;
			//uint16_t Temperature;
			uint16_t len_STRING_BUFFER;
			char STRING_BUFFER[80]; 
			
	
			//Request the data
			while(HAL_I2C_Master_Transmit(&hi2c1,MPU_ADDRESS,&DATA_REGISTER,1,100)!=HAL_OK)
			{;} 
			
				
				while(HAL_I2C_Master_Receive(&hi2c1,MPU_ADDRESS,data,sizeof(data),100)!=HAL_OK)
			{;}
			//Got the data now convert it to the useful form for using
				
				AccX=data[0]<<8|data[1];
				AccY=data[2]<<8|data[3];
				AccZ=data[4]<<8|data[5];
				//Temperature=data[6]<<8|data[7];
				GyrX=data[8]<<8|data[9];
				GyrY=data[10]<<8|data[11];
				GyrZ=data[12]<<8|data[13];
				
			//Now print them in a buffer and transmit
			len_STRING_BUFFER=sprintf(STRING_BUFFER,"Sensor %d: %d,%d,%d,%d,%d,%d END ",Sensor_Number,AccX,AccY,AccZ,GyrX,GyrY,GyrZ);
				if(DEBUG)
				{HAL_UART_Transmit(&huart1,(unsigned char *)STRING_BUFFER,len_STRING_BUFFER,100);}
			HAL_UART_Transmit(&huart3,(unsigned char *)STRING_BUFFER,len_STRING_BUFFER,100);
				
}


static void ACTIVATE_SENSOR(uint8_t Sensor_Number)
{


	switch(Sensor_Number)
	{case 1:
			HAL_GPIO_WritePin(SENSOR_1_GPIO_Port,SENSOR_1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_3_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_4_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_5_Pin,GPIO_PIN_SET);
			//HAL_Delay(50);
			break;
		case 2:
			HAL_GPIO_WritePin(SENSOR_1_GPIO_Port,SENSOR_1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_2_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_3_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_4_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_5_Pin,GPIO_PIN_SET);
			//HAL_Delay(50);
			break;
		case 3:
			HAL_GPIO_WritePin(SENSOR_1_GPIO_Port,SENSOR_1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_2_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_3_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_4_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_5_Pin,GPIO_PIN_SET);
			//HAL_Delay(50);
			break;
		case 4:
			HAL_GPIO_WritePin(SENSOR_1_GPIO_Port,SENSOR_1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_2_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_3_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_4_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_5_Pin,GPIO_PIN_SET);
			//HAL_Delay(50);
			break;
		case 5:
			HAL_GPIO_WritePin(SENSOR_1_GPIO_Port,SENSOR_1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_2_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_3_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_4_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_5_Pin,GPIO_PIN_RESET);
			//HAL_Delay(50);
			break;
	 	default:  //DEFAULT DEACTIVATES THE SENSOR MULTIPLEXERS
			HAL_GPIO_WritePin(SENSOR_1_GPIO_Port,SENSOR_1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_2_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_3_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_4_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,SENSOR_5_Pin,GPIO_PIN_SET);
			//HAL_Delay(50);
			break;
	
	}	

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
