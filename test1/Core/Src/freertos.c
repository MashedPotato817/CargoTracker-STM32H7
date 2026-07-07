/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app/app.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Task_StateMachi */
osThreadId_t Task_StateMachiHandle;
const osThreadAttr_t Task_StateMachi_attributes = {
  .name = "Task_StateMachi",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Task_4G_MQTT */
osThreadId_t Task_4G_MQTTHandle;
const osThreadAttr_t Task_4G_MQTT_attributes = {
  .name = "Task_4G_MQTT",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Task_I2C_Sensor */
osThreadId_t Task_I2C_SensorHandle;
const osThreadAttr_t Task_I2C_Sensor_attributes = {
  .name = "Task_I2C_Sensor",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_GPS */
osThreadId_t Task_GPSHandle;
const osThreadAttr_t Task_GPS_attributes = {
  .name = "Task_GPS",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_Flash */
osThreadId_t Task_FlashHandle;
const osThreadAttr_t Task_Flash_attributes = {
  .name = "Task_Flash",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Task_Alarm */
osThreadId_t Task_AlarmHandle;
const osThreadAttr_t Task_Alarm_attributes = {
  .name = "Task_Alarm",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for queue_activation */
osMessageQueueId_t queue_activationHandle;
const osMessageQueueAttr_t queue_activation_attributes = {
  .name = "queue_activation"
};
/* Definitions for queue_sensor_data */
osMessageQueueId_t queue_sensor_dataHandle;
const osMessageQueueAttr_t queue_sensor_data_attributes = {
  .name = "queue_sensor_data"
};
/* Definitions for queue_cloud_cmd */
osMessageQueueId_t queue_cloud_cmdHandle;
const osMessageQueueAttr_t queue_cloud_cmd_attributes = {
  .name = "queue_cloud_cmd"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartTask_StateMachine(void *argument);
void StartTask_4G_MQTT(void *argument);
void StartTask_I2C_Sensors(void *argument);
void StartTask_GPS(void *argument);
void StartTask_Flash(void *argument);
void StartTask_Alarm(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

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

  /* Create the queue(s) */
  /* creation of queue_activation */
  queue_activationHandle = osMessageQueueNew (16, sizeof(uint16_t), &queue_activation_attributes);

  /* creation of queue_sensor_data */
  queue_sensor_dataHandle = osMessageQueueNew (16, sizeof(uint32_t), &queue_sensor_data_attributes);

  /* creation of queue_cloud_cmd */
  queue_cloud_cmdHandle = osMessageQueueNew (8, sizeof(uint16_t), &queue_cloud_cmd_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_StateMachi */
  Task_StateMachiHandle = osThreadNew(StartTask_StateMachine, NULL, &Task_StateMachi_attributes);

  /* creation of Task_4G_MQTT */
  Task_4G_MQTTHandle = osThreadNew(StartTask_4G_MQTT, NULL, &Task_4G_MQTT_attributes);

  /* creation of Task_I2C_Sensor */
  Task_I2C_SensorHandle = osThreadNew(StartTask_I2C_Sensors, NULL, &Task_I2C_Sensor_attributes);

  /* creation of Task_GPS */
  Task_GPSHandle = osThreadNew(StartTask_GPS, NULL, &Task_GPS_attributes);

  /* creation of Task_Flash */
  Task_FlashHandle = osThreadNew(StartTask_Flash, NULL, &Task_Flash_attributes);

  /* creation of Task_Alarm */
  Task_AlarmHandle = osThreadNew(StartTask_Alarm, NULL, &Task_Alarm_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartTask_StateMachine */
/**
  * @brief  Function implementing the Task_StateMachi thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask_StateMachine */
void StartTask_StateMachine(void *argument)
{
  /* USER CODE BEGIN StartTask_StateMachine */
  App_TaskStateMachine();
  /* USER CODE END StartTask_StateMachine */
}

/* USER CODE BEGIN Header_StartTask_4G_MQTT */
/**
* @brief Function implementing the Task_4G_MQTT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_4G_MQTT */
void StartTask_4G_MQTT(void *argument)
{
  /* USER CODE BEGIN StartTask_4G_MQTT */
  App_Task4GMQTT();
  /* USER CODE END StartTask_4G_MQTT */
}

/* USER CODE BEGIN Header_StartTask_I2C_Sensors */
/**
* @brief Function implementing the Task_I2C_Sensor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_I2C_Sensors */
void StartTask_I2C_Sensors(void *argument)
{
  /* USER CODE BEGIN StartTask_I2C_Sensors */
  App_TaskI2CSensors();
  /* USER CODE END StartTask_I2C_Sensors */
}

/* USER CODE BEGIN Header_StartTask_GPS */
/**
* @brief Function implementing the Task_GPS thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_GPS */
void StartTask_GPS(void *argument)
{
  /* USER CODE BEGIN StartTask_GPS */
  App_TaskGPS();
  /* USER CODE END StartTask_GPS */
}

/* USER CODE BEGIN Header_StartTask_Flash */
/**
* @brief Function implementing the Task_Flash thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Flash */
void StartTask_Flash(void *argument)
{
  /* USER CODE BEGIN StartTask_Flash */
  App_TaskFlash();
  /* USER CODE END StartTask_Flash */
}

/* USER CODE BEGIN Header_StartTask_Alarm */
/**
* @brief Function implementing the Task_Flash thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Alarm */
void StartTask_Alarm(void *argument)
{
  /* USER CODE BEGIN StartTask_Alarm */
  App_TaskAlarm();
  /* USER CODE END StartTask_Alarm */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

