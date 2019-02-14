/* USER CODE BEGIN Header */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "usb_host.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "uart_com.h"
#include "bsp_lcd.h"
#include "touch.h"
#include "spi_flash.h"

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

DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
osThreadId defaultTaskHandle;

osThreadId ledTaskHandle;
osThreadId keyScanTaskHandle;
osThreadId uartSendTaskHandle;
osThreadId lcdTaskHandle;
osThreadId touchTaskHandle;

//extern ApplicationTypeDef Appli_state;
extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */

extern osMessageQId AppliEvent;
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
void StartDefaultTask(void const * argument);
static void MSC_Application(void);
void StartLcdTask(void const * argument);
void StartTouchTask(void const * argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
    LedConfig();
    KeyConfig();
    UartComConfig();

    SpiFlashConfig();

    LCD_Init();
	POINT_COLOR=RED;     			//画笔颜色：红色

    MX_GPIO_Init();
    MX_DMA_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 1024);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    osMessageQDef(osqueue, 1, uint16_t);
    AppliEvent = osMessageCreate(osMessageQ(osqueue), NULL);
    /* USER CODE END RTOS_THREADS */

    osThreadDef(ledTask, StartLedToggleTask, osPriorityNormal, 0, 128);
    ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);

    osThreadDef(keyScanTask, StartKeyScanTask, osPriorityNormal, 0, 128);
    keyScanTaskHandle = osThreadCreate(osThread(keyScanTask), NULL);
    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */
    osThreadDef(uartSendTask, StartUartSendTask, osPriorityNormal, 0, 128);
    uartSendTaskHandle = osThreadCreate(osThread(uartSendTask), NULL);

//    osThreadDef(lcdTask, StartLcdTask, osPriorityNormal, 0, 128);
//    lcdTaskHandle = osThreadCreate(osThread(lcdTask), NULL);

    osThreadDef(touchTask, StartTouchTask, osPriorityNormal, 0, 128);
    touchTaskHandle = osThreadCreate(osThread(touchTask), NULL);

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {

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

    /**Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 144;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 3;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
        |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
 * Enable DMA controller clock
 * Configure DMA for memory to memory transfers
 *   hdma_memtomem_dma2_stream0
 */
static void MX_DMA_Init(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* Configure DMA request hdma_memtomem_dma2_stream0 on DMA2_Stream0 */
    hdma_memtomem_dma2_stream0.Instance = DMA2_Stream0;
    hdma_memtomem_dma2_stream0.Init.Channel = DMA_CHANNEL_0;
    hdma_memtomem_dma2_stream0.Init.Direction = DMA_MEMORY_TO_MEMORY;
    hdma_memtomem_dma2_stream0.Init.PeriphInc = DMA_PINC_ENABLE;
    hdma_memtomem_dma2_stream0.Init.MemInc = DMA_MINC_ENABLE;
    hdma_memtomem_dma2_stream0.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_memtomem_dma2_stream0.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_memtomem_dma2_stream0.Init.Mode = DMA_NORMAL;
    hdma_memtomem_dma2_stream0.Init.Priority = DMA_PRIORITY_LOW;
    hdma_memtomem_dma2_stream0.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_memtomem_dma2_stream0.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_memtomem_dma2_stream0.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_memtomem_dma2_stream0.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_memtomem_dma2_stream0) != HAL_OK)
    {
        Error_Handler( );
    }

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);

    /*Configure GPIO pin : PG8 */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*Configure GPIO pin : PA15 */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
    osEvent event;
    /* init code for USB_HOST */
    MX_USB_HOST_Init();

    /* init code for FATFS */
    MX_FATFS_Init();

    /* USER CODE BEGIN 5 */
    /* Infinite loop */
    for(;;)
    {
        event = osMessageGet(AppliEvent, osWaitForever);

        if(event.status == osEventMessage)
        {
            switch(event.value.v)
            {
                case APPLICATION_START:
                    MSC_Application();
                    break;

                case APPLICATION_IDLE:
                    f_mount(NULL, (TCHAR const*)"", 0);
                    break;

                default:
                    break;
            }
            //    osDelay(10);
        }
    }
    /* USER CODE END 5 */
}

/**
 * @brief  Main routine for Mass Storage Class
 * @param  None
 * @retval None
 */
static void MSC_Application(void)
{
    FRESULT res;                                          /* FatFs function common result code */
    uint32_t byteswritten, bytesread;                     /* File write/read counts */
    uint8_t wtext[] = "This is stm456 working with FatFs"; /* File write buffer */
    uint8_t rtext[100];                                   /* File read buffer */

    /* Register the file system object to the FatFs module */
    if(f_mount(&USBHFatFS, (TCHAR const*)USBHPath, 0) != FR_OK)
    {
        /* FatFs Initialization Error */
        Error_Handler();
    }
    else
    {
        /* Create and Open a new text file object with write access */
        if(f_open(&USBHFile, "stm456.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
        {
            /* 'stm456.TXT' file Open for write Error */
            Error_Handler();
        }
        else
        {
            /* Write data to the text file */
            res = f_write(&USBHFile, wtext, sizeof(wtext), (void *)&byteswritten);


            if((byteswritten == 0) || (res != FR_OK))
            {
                /* 'stm456.TXT' file Write or EOF Error */
                Error_Handler();
            }
            else
            {
                /* Close the open text file */
                f_close(&USBHFile);

                /* Open the text file object with read access */
                if(f_open(&USBHFile, "stm456.txt", FA_READ) != FR_OK)
                {
                    /* 'stm456.TXT' file Open for read Error */
                    Error_Handler();
                }
                else
                {
                    /* Read data from the text file */
                    res = f_read(&USBHFile, rtext, sizeof(rtext), (void *)&bytesread);

                    if((bytesread == 0) || (res != FR_OK))
                    {
                        /* 'stm456.TXT' file Read or EOF Error */
                        Error_Handler();
                    }
                    else
                    {
                        /* Close the open text file */
                        f_close(&USBHFile);

                        /* Compare read data with the expected data */
                        if((bytesread != byteswritten))
                        {
                            /* Read data is different from the expected data */
                            Error_Handler();
                        }
                        else
                        {
                            /* Success of the demo: no error occurrence */
                            HAL_GPIO_WritePin(GPIOF, LED1_Pin, GPIO_PIN_RESET);
                        }
                    }
                }
            }
        }
    }

    /* Unlink the USB disk I/O driver */
    FATFS_UnLinkDriver(USBHPath);
}

void  StartLcdTask(  void const * argument )
{
    /* USER CODE BEGIN 5 */
    static uint8_t statusFlag = 0;
	char lcd_id[12];				//存放LCD ID字符串
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。
    /* Infinite loop */
    for(;;)
    {
        if(statusFlag)
        {
            statusFlag = 0;
            LCD_Clear(GREEN);
        }
        else
        {
            statusFlag = 1;
            LCD_Clear(YELLOW);
        }
		POINT_COLOR=RED;
		LCD_ShowString(30,40,210,24,24,"Explorer STM32F4");
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
		LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
 		LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID
		LCD_ShowString(30,130,200,12,12,"2017/4/8");
        osDelay(3000);
    }
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM6) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    HAL_GPIO_WritePin(GPIOF, LED0_Pin, GPIO_PIN_RESET);
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
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
