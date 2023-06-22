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
#include <string>
#include <string.h> // Also need the C functions
#include <stdio.h>
#include "SimpleWatchdog.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PID_DELAY 15
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern UART_HandleTypeDef huart2;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t SerialTaskHandle;
const osThreadAttr_t SerialTask_attributes = {
    .name = "SerialTask",
    .attr_bits = osThreadDetached,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = NULL,
    .stack_size = 128 * 10,
    .priority = (osPriority_t)osPriorityNormal,
    .tz_module = 0,
    .reserved = 0};


osThreadId_t ServoTaskHandle;
const osThreadAttr_t ServoTask_attributes = {
    .name = "ServoTask",
    .attr_bits = osThreadDetached,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = NULL,
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
    .tz_module = 0,
    .reserved = 0};

const osMutexAttr_t servo_mutex_attributes = {
  "ServoMutex",
  osMutexRecursive | osMutexPrioInherit,
  NULL,
  0U
};


osMutexId ServoDataMutex;
SimpleWatchDog watchDog(IWDG);

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartServoTask(void *argument);
void StartSerialTask(void *argument);
void PrepareTimerInput();
void PrepareTimerOutput();

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */
  ServoDataMutex = osMutexNew(&servo_mutex_attributes);
  if(ServoDataMutex == NULL)
  {
    return;
  }
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

  PrepareTimerInput();
  PrepareTimerOutput();
  /* Create the thread(s) */
  /* creation of defaultTask */
  SerialTaskHandle = osThreadNew(StartSerialTask, NULL, &SerialTask_attributes);
  ServoTaskHandle = osThreadNew(StartServoTask, NULL, &ServoTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
  // TODO: Uncomment this before sending it
  // watchDog.SetPrescaler(WatchDogPrescaler::PSC_256);
  // watchDog.SetTimeout(1); // One second timeout should be enough
  // watchDog.Start();
}

// Admittedly not the cleanest solution, but working > pretty, especially when you're on a deadline
void PrepareTimerInput()
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  TIM3->PSC = 72;
  TIM3->EGR |= TIM_EGR_UG;
  TIM3->SR = ~TIM_SR_UIF;

  TIM3->CCMR1 = (TIM3->CCMR1 & ~TIM_CCMR1_CC1S_Msk) | (0b01 << TIM_CCMR1_CC1S_Pos);
  TIM3->CCER = (TIM3->CCER & ~TIM_CCER_CC1NP_Msk) | (0 << TIM_CCER_CC1NP_Pos);
  TIM3->CCER = (TIM3->CCER & ~TIM_CCER_CC1P_Msk) | (0 << TIM_CCER_CC1P_Pos);
  TIM3->CCER = (TIM3->CCER & ~TIM_CCER_CC1E_Msk) | (0b1 << TIM_CCER_CC1E_Pos);

  TIM3->CCMR1 = (TIM3->CCMR1 & ~TIM_CCMR1_CC2S_Msk) | (0b10 << TIM_CCMR1_CC2S_Pos);
  TIM3->CCER = (TIM3->CCER & ~TIM_CCER_CC2NP_Msk) | (0b0 << TIM_CCER_CC2NP_Pos);
  TIM3->CCER = (TIM3->CCER & ~TIM_CCER_CC2P_Msk) | (0b1 << TIM_CCER_CC2P_Pos);
  TIM3->CCER = (TIM3->CCER & ~TIM_CCER_CC2E_Msk) | (0b1 << TIM_CCER_CC2E_Pos);
  TIM3->CCER = (TIM3->CCER & ~TIM_CCER_CC2E_Msk) | (0b1 << TIM_CCER_CC2E_Pos);

  TIM3->SMCR = (TIM3->SMCR & ~TIM_SMCR_TS_Msk) | (0b101 << TIM_SMCR_TS_Pos);
  TIM3->SMCR = (TIM3->SMCR & ~TIM_SMCR_SMS_Msk) | (0b100 << TIM_SMCR_SMS_Pos);

  TIM3->CR1 = (TIM3->CR1 & ~TIM_CR1_CEN_Msk) | (0b1 << TIM_CR1_CEN_Pos);

  GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER4) | (0b10 << GPIO_MODER_MODER4_Pos);
  GPIOB->AFR[0] = (GPIOB->AFR[0] & ~GPIO_AFRL_AFRL4) | (0B0010 << GPIO_AFRL_AFRL4_Pos);
}

void PrepareTimerOutput()
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->PSC = 72;
  TIM2->EGR |= TIM_EGR_UG;
  TIM2->SR = ~TIM_SR_UIF;
  TIM2->ARR = 20000;
  TIM2->CCMR1 = (TIM2->CCMR1 & ~TIM_CCMR1_OC1M_Msk) | (0b0110 << TIM_CCMR1_OC1M_Pos);
  TIM2->CCR1 = 1520;
  TIM2->CCER = (TIM2->CCER & ~TIM_CCER_CC1E_Msk) | (0b1 << TIM_CCER_CC1E_Pos);
  TIM2->CR1 = (TIM2->CR1 & ~TIM_CR1_CEN_Msk) | (0b1 << TIM_CR1_CEN_Pos);

  GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODER0) | (0b10 << GPIO_MODER_MODER0_Pos);
  GPIOA->AFR[0] = (GPIOA->AFR[0] & ~GPIO_AFRL_AFRL0) | (0B0001 << GPIO_AFRL_AFRL0_Pos);
}

int target = 0;
int currentPositon = 0;
int lastPosition = 0;

// SUPPORTING MATH
constexpr int posMax = 1024;
constexpr int posMin = 0;
constexpr int halfwayPosition = posMax / 2 * -1;
constexpr int angleMin = 0;
constexpr int angleMax = 360;

int PosToAngle(int value)
{
    return (value - posMin) * (angleMax - angleMin) / (posMax - posMin) + angleMin;
}

int CalculateDifference(int pos)
{
  int difference = target - pos;
  if (difference > halfwayPosition)
  {
    difference -= posMax;
  }
  else if (difference < halfwayPosition * -1)
  {
    difference += posMax;
  }
  return difference;
}


// SERIAL TASK

void StartSerialTask(void *argument)
{
  const int bufferSize = 50;
  char buffer[bufferSize] = {'\0'};
  for (;;)
  {
    osMutexAcquire(ServoDataMutex, osWaitForever);
    snprintf(buffer, bufferSize, "Target: %d, Current: %d\r\n", target, PosToAngle(currentPositon));
    HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), 200);
    osMutexRelease(ServoDataMutex);
    watchDog.Feed();
    osDelay(200);
  }
}
// PID TASK

constexpr float Kp = 0.1f;
constexpr float Ki = 0.001f;
constexpr float Kd = 0.2f;

float error = 0.0f;
float previousError = 0.0f;
float integral = 0.0f;
float derivative = 0.0f;

int PID_Calculations()
{
  error = CalculateDifference(currentPositon);
  integral += error;
  if(abs(error) < 10)
  {
    error = 0;
    integral = 0;
  }
  derivative = error - previousError;
  previousError = error;
  return (int)(Kp * error) + (Ki * integral) + (Kd * derivative);
}

int GetServoPosition()
{
    return TIM3->CCR2;
}

int GetNewPosition()
{
    return rand() % 360;
}

const int ClockWiseSpeed = 1460;
const int CounterClockWiseSpeed = 1510;

void MoveClockwise(int speed)
{
    TIM2->CCR1 = ClockWiseSpeed - speed;
}

void MoveCounterClockwise(int speed)
{
    TIM2->CCR1 = CounterClockWiseSpeed + speed;
}

void StopMoving() //TODO: Unused
{
    TIM2->CCR1 = 0;
}

void Move(int direction, int speed)
{
    if(direction > 0)
    {
      MoveCounterClockwise(speed);
    }
    else
    {
      MoveClockwise(speed);
    }
}

void StartServoTask(void *argument)
{
  int servoData = 0;
  target = GetNewPosition();

  for (;;)
  {
    osMutexAcquire(ServoDataMutex, osWaitForever);
    currentPositon = GetServoPosition();

    if(PosToAngle(currentPositon) == target)
    {
      target = GetNewPosition();
    }

    osMutexRelease(ServoDataMutex);

    servoData = PID_Calculations();

    osMutexAcquire(ServoDataMutex, osWaitForever);
    Move(error, servoData);
    osMutexRelease(ServoDataMutex);
    osDelay(PID_DELAY);
  }
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
