/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <stdio.h>
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define HOLD_THRESHOLD_MS   100    // Press longer than this = hold
#define DELAY_COUNT 4
#define MODE_IDLE   0
#define MODE_SINGLE 1
#define MODE_DOUBLE 2
#define MODE_HOLD   3

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t delays[] = {250, 500, 1000, 2000};
uint32_t active_delays[DELAY_COUNT];

int8_t idx = 0;
int8_t dir = 1;

uint32_t last_led_tick = 0;
/* Button handling */
uint8_t last_btn_state = 0;
uint32_t last_btn_tick = 0;
uint8_t click_count = 0;
uint32_t btn_press_tick = 0;
uint32_t last_hold_tick = 0;
uint8_t  hold_active = 0;

uint8_t prev_mode = 255;
uint32_t prev_delay = 0;

uint8_t current_mode;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* ================= LCD DRIVER (4-bit) ================= */

void LCD_EnablePulse(void)
{
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

void LCD_Send4Bit(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LCD_EnablePulse();
}

void LCD_Command(uint8_t cmd)
{
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);

    LCD_Send4Bit(cmd >> 4);
    LCD_Send4Bit(cmd & 0x0F);

    HAL_Delay(2);
}

void LCD_Data(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

    LCD_Send4Bit(data >> 4);
    LCD_Send4Bit(data & 0x0F);

    HAL_Delay(2);
}
void LCD_Init(void)
{
    HAL_Delay(20);

    LCD_Command(0x33);
    LCD_Command(0x32);
    LCD_Command(0x28);   // 4-bit, 2 line
    LCD_Command(0x0C);   // Display ON
    LCD_Command(0x06);   // Entry mode
    LCD_Command(0x01);   // Clear
    HAL_Delay(5);
}
void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr;
    if (row == 0) addr = 0x80;
    else if (row == 1) addr = 0xC0;
    else if (row == 2) addr = 0x94;
    else addr = 0xD4;

    LCD_Command(addr + col);
}

void LCD_Print(char *str)
{
    while (*str)
        LCD_Data(*str++);
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
  /* USER CODE BEGIN 2 */
  LCD_Init();

  LCD_SetCursor(0,0);
  LCD_Print("Mode: IDLE");

  LCD_SetCursor(1,0);
  LCD_Print("Delay: 250 ms");

  for (int i = 0; i < DELAY_COUNT; i++)
  {
      active_delays[i] = delays[i];  // start with normal delays
  }


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*  LED BLINK LOGIC  */
	  	  if(!hold_active &&
	  		    (HAL_GetTick() - last_led_tick >= active_delays[idx]))

	  	    {
	  	      last_led_tick = HAL_GetTick();
	  	      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);

	  	    /* AUTO move to next delay */
	  	    idx += dir;

	  	    if (idx >= DELAY_COUNT)
	  	        idx = 0;
	  	    else if (idx < 0)
	  	        idx = DELAY_COUNT - 1;
	  	    }
	  	 /*  BUTTON LOGIC */
	  	    uint8_t btn = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

	  	    /* Detect rising edge */
	  	    if (btn && !last_btn_state)
	  	    {
	  	    	btn_press_tick = HAL_GetTick();
	  	    	last_btn_tick  = btn_press_tick;
	  	    	if (click_count < 2)           // Restrict clicks to max 2
	  	    	        click_count++;
	  	    }
	  	  /*  Button Release Detection */
	  	    if (!btn && last_btn_state)
	  	    {
	  	        if (hold_active)
	  	        {
	  	          // Restore original delays
	  	              for (int i = 0; i < DELAY_COUNT; i++)
	  	                  active_delays[i] = delays[i];
	  	        }
	  	        if (!hold_active)
	  	        {
	  	              /* Wait for single/double click resolution */
	  	              last_btn_tick = HAL_GetTick();
	  	         }
	  	         hold_active = 0;

	  	    }

	  	    /*  Hold Detection */
	  	    if (btn && !hold_active &&
	  	      (HAL_GetTick() - btn_press_tick >= HOLD_THRESHOLD_MS))
	  	    {
	  	    	hold_active = 1;
	  	    	click_count = 0;   // cancel click logic
	  	    	last_hold_tick = HAL_GetTick();

	  	    	 // HALVE the delays while holding
	  	    	 for (int i = 0; i < DELAY_COUNT; i++)
	  	    	      active_delays[i] = delays[i] / 2;
	  	    }

	  	    /* Hold Action (Fast Cycling)  */
	  	    if (hold_active &&
	  	      (HAL_GetTick() - last_hold_tick >= active_delays[idx]))
	  	    {
	  	    	last_hold_tick = HAL_GetTick();
	  	    	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);

	  	    	idx += dir;
	  	    	if (idx >= DELAY_COUNT)
	  	    		idx = 0;
	  	    	else if (idx < 0)
	  	    		idx = DELAY_COUNT - 1;
	  	    }

	  	    /* Click Resolution */
	  	    if (!hold_active &&
	  	      click_count > 0 &&
	  	      (HAL_GetTick() - last_btn_tick > 50))
	  	    {
	  	    	if (click_count == 1)
	  	    	{
	  	    		idx += dir;
	  	    		if (idx >= DELAY_COUNT) idx = 0;
	  	    		else if (idx < 0) idx = DELAY_COUNT - 1;
	  	    	}
	  	    	else
	  	    	{
	  	    		dir = -dir;
	  	    	}

	  	    	click_count = 0;
	  	    }


	  	  last_btn_state = btn;

	  	  /* ===== DETERMINE CURRENT MODE ===== */
	  	  if (hold_active)
	  	      current_mode = MODE_HOLD;
	  	  else if (click_count == 2)
	  	      current_mode = MODE_DOUBLE;
	  	  else if (click_count == 1)
	  	      current_mode = MODE_SINGLE;
	  	  else
	  	      current_mode = MODE_IDLE;

	  	  char buffer[20];

	  	/* ===== UPDATE LCD ROW 1: MODE (only if changed) ===== */
	  	if (current_mode != prev_mode)
	  	{
	  	    LCD_SetCursor(0, 0);

	  	    switch (current_mode)
	  	    {
	  	        case MODE_HOLD:
	  	            LCD_Print("Mode: HOLD");
	  	            break;
	  	        case MODE_DOUBLE:
	  	            LCD_Print("Mode: DOUBLE ");
	  	            break;
	  	        case MODE_SINGLE:
	  	            LCD_Print("Mode: SINGLE ");
	  	            break;
	  	        default:
	  	            LCD_Print("Mode: IDLE");
	  	            break;
	  	    }

	  	    prev_mode = current_mode;
	  	}

	  	/* ===== UPDATE LCD ROW 2: DELAY (only if changed) ===== */
	  	if (active_delays[idx] != prev_delay)
	  	{
	  	    LCD_SetCursor(1, 0);
	  	    sprintf(buffer, "Delay: %lu ms   ", active_delays[idx]);
	  	    LCD_Print(buffer);

	  	    prev_delay = active_delays[idx];
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

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
#ifdef USE_FULL_ASSERT
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
