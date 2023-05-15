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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <Pin.hpp>
#include <timer.hpp>
#include <SimpleWatchDog.h>
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
/* USER CODE BEGIN Variables */

extern UART_HandleTypeDef huart2;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t TerminalTaskHandle;
const osThreadAttr_t TerminalTask_attributes = {
    .name = "terminalTask",
    .attr_bits = osThreadDetached,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = NULL,
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
    .tz_module = 0,
    .reserved = 0};

osThreadId_t PWMTaskHandle;
const osThreadAttr_t PWMTaskHandle_attributes = {
    .name = "PWMTask",
    .attr_bits = osThreadDetached,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = NULL,
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
    .tz_module = 0,
    .reserved = 0};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void startSerialTask(void *argument);
void startPWMTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

int termninalInput = -1;

const int PWM_PSC = 72;
const int PWM_ARR = 200;

Timer pwmOutput = {TIM2};
Timer pwmInput = {TIM3};

SimpleWatchDog watchDog = SimpleWatchDog(IWDG);

int pwmInputValue = 0;
int pwmPos = 0;

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */
  
  BasicTimerPackage basicTimerPackageOutput = {PWM_PSC, PWM_ARR, TimerBit::TIMER2};
  BasicTimerPackage basicTimerPackageInput = {PWM_PSC, PWM_ARR, TimerBit::TIMER3};

  PWMOutputPackage pwmOutputPackage = {basicTimerPackageOutput, 1, CC_ChannelType::CC_CHANNELTYPE_PWMOutput, OCM_Type::OCM_TYPE_PWM1, 1280};

  // PIN B4 (D5) -> TIM3 INPUT
  GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER4) | (0b10 << GPIO_MODER_MODER4_Pos);
  GPIOB->AFR[0] = (GPIOB->AFR[0] & ~GPIO_AFRL_AFRL4) | (0B0010 << GPIO_AFRL_AFRL4_Pos);

  // PIN 
  GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODER0) | (0b10 << GPIO_MODER_MODER0_Pos);
  GPIOA->AFR[0] = (GPIOA->AFR[0] & ~GPIO_AFRL_AFRL0) | (0B0001 << GPIO_AFRL_AFRL0_Pos);

  pwmInput.EnableAsPWMInput(basicTimerPackageInput);
  pwmOutput.EnableAsPWMOutput(pwmOutputPackage);
  
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  

  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  TerminalTaskHandle = osThreadNew(startSerialTask, NULL, &TerminalTask_attributes);
  PWMTaskHandle = osThreadNew(startPWMTask, NULL, &PWMTaskHandle_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void startSerialTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */

  for (;;)
  {
    if(HAL_UART_Receive(&huart2, (uint8_t *)&termninalInput, 1, 1000) == HAL_OK)
    {
      pwmInputValue = termninalInput;
    }
  }
  /* USER CODE END StartDefaultTask */
}

void startPWMTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  watchDog.SetPrescaler(WatchDogPrescaler::PSC_256);
  watchDog.SetTimeout(RLR_SECOND * SWD_PSC_SECOND);
  watchDog.Start();
  for (;;)
  {
    // Data Transmission
    pwmPos = pwmInput.GetPWMInput();
    HAL_UART_Transmit(&huart2, (uint8_t *)&pwmPos, 4, 1000);

    //PWM Signal
    pwmOutput.SendPWMSignal(pwmInputValue);
    watchDog.Feed();
  }

  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
