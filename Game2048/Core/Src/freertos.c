/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
/* USER CODE BEGIN Variables */
#include "cmsis_os2.h"
#include "queue.h"

// Định nghĩa các hướng di chuyển cho game 2048
#define DIR_NONE  0
#define DIR_UP    1
#define DIR_DOWN  2
#define DIR_LEFT  3
#define DIR_RIGHT 4

// Ngưỡng điện áp chặn
#define JOY_THRESHOLD_HIGH 3000
#define JOY_THRESHOLD_LOW  1000

// Kết nối với mảng dữ liệu ADC đang chạy ngầm do DMA bơm vào bên main.c
extern uint16_t adc_value[2];
extern osMessageQueueId_t joyQueueHandle;
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE BEGIN StartJoystickTask */
void StartJoystickTask(void *argument)
{
  // Biến ghi nhớ trạng thái: 1 = Tay cầm đang ở tâm
  uint8_t joystick_ready = 1;
  uint8_t current_dir = DIR_NONE;

  /* Vòng lặp vô tận của Task đọc Joystick */
  for(;;)
  {
    // Giả định: adc_value[0] là trục X (Ngang), adc_value[1] là trục Y (Dọc)
    uint16_t joy_x = adc_value[0];
    uint16_t joy_y = adc_value[1];

    current_dir = DIR_NONE;

    // Phân tích hướng gạt dựa trên điện áp ADC
    if (joy_x > JOY_THRESHOLD_HIGH) {
        current_dir = DIR_RIGHT;
    } else if (joy_x < JOY_THRESHOLD_LOW) {
        current_dir = DIR_LEFT;
    } else if (joy_y > JOY_THRESHOLD_HIGH) {
        current_dir = DIR_UP;
    } else if (joy_y < JOY_THRESHOLD_LOW) {
        current_dir = DIR_DOWN;
    }

    // Kiểm tra trạng thái gạt
    if (current_dir != DIR_NONE)
    {
        // Nếu Joystick đang sẵn sàng (vừa từ tâm gạt ra)
        if (joystick_ready == 1)
        {
            // NÉM TÍN HIỆU VÀO HỘP THƯ QUEUE (Timeout = 0: Không chờ đợi)
            osMessageQueuePut(joyQueueHandle, &current_dir, 0, 0);

            // LẬP TỨC KHÓA LẠI, không cho gửi liên tục nữa
            joystick_ready = 0;
        }
    }
    else
    {
        // Nếu current_dir == DIR_NONE tức là Joystick đã được thả về vùng an toàn (Tâm)
        // Kiểm tra xem Joystick đã thực sự về gần mốc ~2048 chưa để mở khóa
        if (joy_x > 1500 && joy_x < 2500 && joy_y > 1500 && joy_y < 2500)
        {
            joystick_ready = 1; // Mở khóa, sẵn sàng cho cú gạt tiếp theo
        }
    }

    // NHƯỜNG CPU: Phải ngủ 30ms để nhường cho TouchGFX vẽ đồ họa
    osDelay(30);
  }
}
/* USER CODE END StartJoystickTask */
/* USER CODE END Application */

