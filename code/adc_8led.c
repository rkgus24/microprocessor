#include "main.h"
#include <stdio.h>

#define ADC_MAX		4096
#define CDS_FACTOR		2.2
#define BASE_R			10
#define CDS_R_10LX		1 					// 주변 환경에 따라 변경 필요
#define CDS_R_3LX		CDS_R_10LX * CDS_FACTOR
#define CDS_R_1LX		CDS_R_3LX * CDS_FACTOR
#define CDS_R_0_3LX		CDS_R_1LX * CDS_FACTOR
#define CDS_R_0_1LX		CDS_R_0_3LX * CDS_FACTOR
#define CDS_R_0_03LX		CDS_R_0_1LX * CDS_FACTOR
#define CDS_R_0_01LX		CDS_R_0_03LX * CDS_FACTOR
#define CDS_R_0_003LX	CDS_R_0_01LX * CDS_FACTOR
#define ADC_10LX		(ADC_MAX * BASE_R) / (CDS_R_10LX + BASE_R)
#define ADC_3LX			(ADC_MAX * BASE_R) / (CDS_R_3LX + BASE_R)
#define ADC_1LX			(ADC_MAX * BASE_R) / (CDS_R_1LX + BASE_R)
#define ADC_0_3LX		(ADC_MAX * BASE_R) / (CDS_R_0_3LX + BASE_R)
#define ADC_0_1LX		(ADC_MAX * BASE_R) / (CDS_R_0_1LX + BASE_R)
#define ADC_0_03LX		(ADC_MAX * BASE_R) / (CDS_R_0_03LX + BASE_R)
#define ADC_0_01LX		(ADC_MAX * BASE_R) / (CDS_R_0_01LX + BASE_R)
#define ADC_0_003LX		(ADC_MAX * BASE_R) / (CDS_R_0_003LX + BASE_R)

ADC_HandleTypeDef hadc2;

UART_HandleTypeDef huart2;

int adc_value;
uint8_t count;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC2_Init(void);
static void MX_USART2_UART_Init(void);

int __io_putchar(int ch) {
   HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 1000);
   if (ch == '\n')
      HAL_UART_Transmit(&huart2, (uint8_t *)"\r", 1, 1000);
   return ch;
}
typedef struct led {
  GPIO_TypeDef *port;
  uint16_t pin;
   } LED;
LED led[8] = {
  {GPIOC, GPIO_PIN_3},   {GPIOC, GPIO_PIN_2},
  {GPIOC, GPIO_PIN_1},   {GPIOC, GPIO_PIN_0},
  {GPIOB, GPIO_PIN_15},   {GPIOB, GPIO_PIN_14},
  {GPIOB, GPIO_PIN_13},   {GPIOB, GPIO_PIN_12},
};
void led_on(uint8_t count) {
  for(uint8_t i=0; i < count; i++) {
    HAL_GPIO_WritePin(led[i].port, led[i].pin, 1);
  }
}
void led_off() {
  for(uint8_t i=0; i < 8; i++) {
    HAL_GPIO_WritePin(led[i].port, led[i].pin, 0);
  }
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC2_Init();
  MX_USART2_UART_Init();
  HAL_ADCEx_Calibration_Start(&hadc2);
  HAL_ADC_Start(&hadc2);
  while (1)
  {
	  if(HAL_ADC_PollForConversion(&hadc2, 10) == HAL_OK) {
		  adc_value = HAL_ADC_GetValue(&hadc2);
		  if(adc_value > ADC_10LX) {
			  count = 0;
		  } else if(adc_value > ADC_3LX) {
			  count = 1;
		  } else if(adc_value > ADC_1LX) {
			  count = 2;
		  } else if(adc_value > ADC_0_3LX) {
			  count = 3;
		  } else if(adc_value > ADC_0_1LX) {
			  count = 4;
		  } else if(adc_value > ADC_0_03LX) {
			  count = 5;
		  } else if(adc_value > ADC_0_01LX) {
			  count = 6;
		  } else if(adc_value > ADC_0_003LX) {
			  count = 7;
		  } else {
			  count = 8;
		  }
		  led_on(count);
		  printf("adc_value = %d\n\n", adc_value);
		  HAL_Delay(100);
		  led_off();
	  }
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}
static void MX_ADC2_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
