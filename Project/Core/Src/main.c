/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Application.h"
#include "UART_Application.h"
#include "ButtonCore.h"
#include "LED_Controller.h"
#include <stdio.h>
#include "TMC2208_Registers.h"
#include "TMC2208_Driver.h"

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
Button_t userButton;
uint32_t sent_value = 0;
uint32_t read_back_value = 0;
uint32_t match_count = 0;
TMC2208_t g_tmc2208_driver;
TMC2208_GCONF_t g_gconf;
extern UART_HandleTypeDef huart2;

volatile bool g_i_scale_analog = false;
volatile bool g_internal_rsense = false;
volatile bool g_en_spreadcycle = false;
volatile bool g_shaft = false;
volatile bool g_index_otpw = false;
volatile bool g_index_step = false;
volatile bool g_pdn_disable = false;
volatile bool g_mstep_reg_select = false;
volatile bool g_multistep_filt = false;
volatile bool g_test_mode = false;
volatile uint32_t g_gconf_raw = 0;

volatile bool g_gconf_read_success = false;
volatile bool g_gconf_write_success = false;

TMC2208_IOIN_t g_ioin;
volatile bool g_enn_pin = false;           /* Bit 0 */
volatile bool g_pdn_uart_pin = false;      /* Bit 1 */
volatile bool g_ms1_pin = false;           /* Bit 2 */
volatile bool g_ms2_pin = false;           /* Bit 3 */
volatile bool g_diag_pin = false;          /* Bit 4 */
volatile bool g_step_222x_pin = false;     /* Bit 5 */
volatile bool g_pdn_uart_bit6 = false;     /* Bit 6 */
volatile bool g_step_pin = false;          /* Bit 7 */
volatile bool g_sel_a_pin = false;         /* Bit 8 */
volatile bool g_dir_pin = false;           /* Bit 9 */
volatile uint8_t g_ioin_version = 0;
volatile uint32_t g_ioin_raw = 0;

volatile bool g_ioin_read_success = false;
volatile bool g_driver_enabled = false;
volatile uint8_t g_microstep_mode = 0;
volatile bool g_is_tmc220x = false;
volatile bool g_is_tmc222x = false;

TMC2208_IHOLD_IRUN_t g_ihold_irun;

volatile uint8_t g_ihold = 0;              /* Standstill current (0-31) */
volatile uint8_t g_irun = 0;               /* Run current (0-31) */
volatile uint8_t g_iholddelay = 0;         /* Hold delay (0-15) */
volatile uint32_t g_ihold_irun_raw = 0;

/* Current in mA (for 0.11Ω internal Rsense) */
volatile uint16_t g_irun_ma = 0;
volatile uint16_t g_ihold_ma = 0;

volatile bool g_ihold_irun_read_success = false;
volatile bool g_ihold_irun_write_success = false;
volatile bool g_crc_error = false;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void TMC2208_Debug_Init(void) {
    TMC_Init(&g_tmc2208_driver, &huart2, 0x00);
}
void TMC2208_Debug_ReadAll(void) {

    g_gconf_read_success = TMC2208_ReadGCONF(&g_tmc2208_driver, &g_gconf);
    if (g_gconf_read_success) {
        g_i_scale_analog = g_gconf.i_scale_analog;
        g_internal_rsense = g_gconf.internal_rsense;
        g_en_spreadcycle = g_gconf.en_spreadcycle;
        g_shaft = g_gconf.shaft;
        g_index_otpw = g_gconf.index_otpw;
        g_index_step = g_gconf.index_step;
        g_pdn_disable = g_gconf.pdn_disable;
        g_mstep_reg_select = g_gconf.mstep_reg_select;
        g_multistep_filt = g_gconf.multistep_filt;
        g_test_mode = g_gconf.test_mode;
        g_gconf_raw = g_gconf.raw_value;
    }
       g_ioin_read_success = TMC2208_ReadIOIN(&g_tmc2208_driver, &g_ioin);
       if (g_ioin_read_success) {
           g_enn_pin = g_ioin.enn;
           g_pdn_uart_pin = g_ioin.pdn_uart;
           g_ms1_pin = g_ioin.ms1;
           g_ms2_pin = g_ioin.ms2;
           g_diag_pin = g_ioin.diag;
           g_step_222x_pin = g_ioin.step_222x;
           g_pdn_uart_bit6 = g_ioin.pdn_uart_bit6;
           g_step_pin = g_ioin.step;
           g_sel_a_pin = g_ioin.sel_a;
           g_dir_pin = g_ioin.dir;
           g_ioin_version = g_ioin.version;
           g_ioin_raw = g_ioin.raw_value;
           g_driver_enabled = TMC2208_IsEnabled(&g_ioin);
           g_microstep_mode = TMC2208_GetMicrostepMode(&g_ioin);
           g_is_tmc220x = TMC2208_IsTMC220x(&g_ioin);
           g_is_tmc222x = TMC2208_IsTMC222x(&g_ioin);
       }

         g_ihold_irun_read_success = TMC2208_ReadIHOLD_IRUN(&g_tmc2208_driver, &g_ihold_irun);
         if (g_ihold_irun_read_success) {
             g_ihold = g_ihold_irun.ihold;
             g_irun = g_ihold_irun.irun;
             g_iholddelay = g_ihold_irun.iholddelay;
             g_ihold_irun_raw = g_ihold_irun.raw_value;
             g_irun_ma = TMC2208_CurrentToRMS(g_irun, 110);
             g_ihold_ma = TMC2208_CurrentToRMS(g_ihold, 110);
         }


         g_crc_error = g_tmc2208_driver.isCRCError;
}
void TMC2208_Debug_WriteGCONF(void) {
    g_gconf.i_scale_analog = g_i_scale_analog;
    g_gconf.internal_rsense = g_internal_rsense;
    g_gconf.en_spreadcycle = g_en_spreadcycle;
    g_gconf.shaft = g_shaft;
    g_gconf.index_otpw = g_index_otpw;
    g_gconf.index_step = g_index_step;
    g_gconf.pdn_disable = g_pdn_disable;
    g_gconf.mstep_reg_select = g_mstep_reg_select;
    g_gconf.multistep_filt = g_multistep_filt;
    g_gconf.test_mode = g_test_mode;

    TMC2208_WriteGCONF(&g_tmc2208_driver, &g_gconf);
    g_gconf_write_success = true;

    HAL_Delay(10);
    TMC2208_Debug_ReadAll();
}
void TMC2208_Debug_WriteIHOLD_IRUN(void) {
    g_ihold_irun.ihold = g_ihold & 0x1F;
    g_ihold_irun.irun = g_irun & 0x1F;
    g_ihold_irun.iholddelay = g_iholddelay & 0x0F;

    TMC2208_WriteIHOLD_IRUN(&g_tmc2208_driver, &g_ihold_irun);
    g_ihold_irun_write_success = true;

    HAL_Delay(10);
    TMC2208_Debug_ReadAll();
}
void Example1_CompleteInit(void) {
    TMC2208_Debug_Init();

    /* Configure GCONF for silent operation */
    g_internal_rsense = true;
    g_i_scale_analog = true;
    g_en_spreadcycle = false;
    g_shaft = false;
    g_mstep_reg_select = true;
    g_pdn_disable = true;
    g_multistep_filt = true;
    g_test_mode = false;
    TMC2208_Debug_WriteGCONF();

    /* Set motor current */
    g_irun = 20;        /* ~700mA run current */
    g_ihold = 10;       /* ~350mA hold current */
    g_iholddelay = 10;  /* Delay before reducing current */
    TMC2208_Debug_WriteIHOLD_IRUN();


    TMC2208_Debug_ReadAll();
}
void Example2_CheckAllStatus(void) {
    TMC2208_Debug_ReadAll();
}
void Example6_ContinuousMonitoring(void) {
    static uint32_t last_update = 0;
    uint32_t current_time = HAL_GetTick();

    if (current_time - last_update >= 100) {
        last_update = current_time;
        TMC2208_Debug_ReadAll();
    }
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  TMC2208_Debug_Init();
  Example1_CompleteInit();
  Example2_CheckAllStatus();
 // ApplicationInit();
 // ButtonCore_Init(&userButton, Button_GPIO_Port, Button_Pin);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  Example6_ContinuousMonitoring();
//	  HAL_Delay(10);
//
	        sent_value = 0x00061F0A;
	              TMC_WriteRegister(&g_tmc2208_driver, TMC2208_IHOLD_IRUN, sent_value);
//
	             HAL_Delay(100);
//
//
	              if (TMC_ReadRegister(&g_tmc2208_driver, TMC2208_IHOLD_IRUN, &read_back_value)) {
//
	                  if (read_back_value == sent_value) {
	                      match_count++;
	                  }
	              }
//}\
//
//	             HAL_Delay(500);
//	              sent_value = 0x00060505;
//	                    TMC_WriteRegister(&g_driver, TMC2208_IHOLD_IRUN, sent_value);
//
//	                    HAL_Delay(100);
//
//
//	                    if (TMC_ReadRegister(&g_driver, TMC2208_IHOLD_IRUN, &read_back_value)) {
//
//	                        if (read_back_value == sent_value) {
//	                            match_count++;
//	                        }
//	                    }
//                    HAL_Delay(500);



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


//*TODO*
	  //ApplicationProcess();
	  //ButtonCore_Update(&userButton);
	  //LCDApplication_Process();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_SYSTICK_Callback(void)
{

    HandleTick();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        UART_OnByteReceived();
    }
}
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
