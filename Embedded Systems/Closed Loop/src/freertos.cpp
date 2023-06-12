/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>
#include <cstdlib>

#include "Pin.hpp"
#include "Servo.hpp"
#include "SimpleWatchDog.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
constexpr int Max_Timeout = 200;

struct Queue_Message
{
  char Identifier;
  int Value;
};

constexpr uint32_t QueueSize = 10;

osMessageQueueId_t MessageQueue;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
enum LastDirection
{
  ClockWise = 1,
  CounterClockWise = -1
};
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern UART_HandleTypeDef huart2;
ADC_HandleTypeDef hadc1;
SimpleWatchDog watchDog = {IWDG};

Pin OutputPin = {GPIOB, 10};
Pin InputPin = {GPIOB, 4};

Servo servo(InputPin, OutputPin);

int LightIntensity;
int previousLightIntensity;
LastDirection lastDirection;

osMutexId_t LightIntensityMutex;

const osMutexAttr_t LightIntensityMutex_Attributes =
{
 "Light Intensity Mutex",
 osMutexRecursive | osMutexPrioInherit,
 NULL,
 0U
};

/* USER CODE END Variables */
/* Definitions for defaultTask */

osThreadId_t SerialTaskHandle;
const osThreadAttr_t SerialTask_Attributes = {
    .name = "SerialTask",
    .attr_bits = osThreadDetached,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = NULL,
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal,
    .tz_module = 0,
    .reserved = 0};

osThreadId_t ServoTaskHandle;
const osThreadAttr_t ServoTask_Attributes = {
    .name = "ServoTask",
    .attr_bits = osThreadDetached,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = NULL,
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal,
    .tz_module = 0,
    .reserved = 0};

    osThreadId_t ADCTaskHandle;
const osThreadAttr_t ADCTask_Attributes = {
    .name = "ADCTask",
    .attr_bits = osThreadDetached,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = NULL,
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal,
    .tz_module = 0,
    .reserved = 0};

    osThreadId_t PID_Serial_Input_Handle;
const osThreadAttr_t PID_Serial_Input_Attributes = {
    .name = "PID_Serial_Input_Task",
    .attr_bits = osThreadDetached,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = NULL,
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal,
    .tz_module = 0,
    .reserved = 0};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartSerialTask(void* argument);
void StartServoTask(void* argument);
void StartADCTask(void* argument);
void StartPID_Serial_InputTask(void* argument);
void SwitchDirection();

static void MX_ADC1_Init(void);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */
  watchDog.SetPrescaler(PSC_256);
  watchDog.SetTimeout(IWDG_RLR_SECOND * 2);
  /* USER CODE END Init */
  MX_ADC1_Init();
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  LightIntensityMutex = osMutexNew(&LightIntensityMutex_Attributes);
  if(LightIntensityMutex == nullptr)
  {
    return;
  }
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  MessageQueue = osMessageQueueNew(QueueSize,sizeof(Queue_Message),nullptr);
  if(MessageQueue == nullptr)
  {
    return;
  }
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  SerialTaskHandle = osThreadNew(StartSerialTask, NULL, &SerialTask_Attributes);
  ServoTaskHandle = osThreadNew(StartServoTask, NULL, &ServoTask_Attributes);
  ADCTaskHandle = osThreadNew(StartADCTask, NULL, &ADCTask_Attributes);
  PID_Serial_Input_Handle = osThreadNew(StartPID_Serial_InputTask,NULL,&PID_Serial_Input_Attributes);
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  // watchDog.Start(); // ACTIVATE THE WATCH DOG
  const int bufferSize = 20;
  char buffer[bufferSize];
  snprintf(buffer,bufferSize,"Starting up!\n");
  HAL_UART_Transmit(&huart2,(uint8_t*)buffer,bufferSize,1000);
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */


void StartSerialTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  const int bufferSize = 20;
  char buffer[bufferSize] = {' '};
  for (;;)
  {
    snprintf(buffer, 100, "Position: %d\n", servo.GetPosition());
    HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), Max_Timeout);
    if(HAL_UART_Receive(&huart2, (uint8_t *)buffer, bufferSize, Max_Timeout) == HAL_OK) // Fucks my shit
    {
      Queue_Message newMessage = {buffer[0], atoi(buffer + 1)};
      osMessageQueuePut(MessageQueue,&newMessage,NULL,osWaitForever);
    }
    watchDog.Feed();
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

void StartServoTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;)
  {
    osMutexAcquire(LightIntensityMutex,osWaitForever);
    if (previousLightIntensity < LightIntensity)
    {
      // Light is getting darker
      SwitchDirection();
    }
    osMutexRelease(LightIntensityMutex);
    servo.SetAngle(servo.GetPosition() + (int) lastDirection);
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

void StartADCTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 1);
  for (;;)
  {
    osMutexAcquire(LightIntensityMutex,osWaitForever);
    previousLightIntensity = LightIntensity;
    LightIntensity = HAL_ADC_GetValue(&hadc1);
    osMutexRelease(LightIntensityMutex);
    osDelay(50);
  }
  /* USER CODE END StartDefaultTask */
}

void StartPID_Serial_InputTask(void* argument)
{
  for(;;)
  {
    if(osMessageQueueGetCount(MessageQueue) > 0)
    {
      Queue_Message message;

      if(osMessageQueueGet(MessageQueue,&message,NULL,osWaitForever) != osStatus_t::osOK)
      {
        return;
      }

      servo.SetPIDValues(message.Identifier,message.Value);
    }
    osDelay(500);
  }
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void SwitchDirection()
{
  if(lastDirection == LastDirection::ClockWise)
  {
    lastDirection = LastDirection::CounterClockWise;
  }
  else
  {
    lastDirection = LastDirection::ClockWise;
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
