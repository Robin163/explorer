/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "cmsis_os.h"
#include "bsp_key.h"
#include "bsp_driver_sd.h"
#include "fatfs.h"
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
osThreadId Led0TaskHandle;
osThreadId KeyScanTaskHandle;
osThreadId KeyTaskHandle;
osThreadId FileTaskHandle;


/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartLed0Task(void const * argument);
void StartKeyTask(void const * argument);
void StartFileTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

FRESULT scan_files ( char* path );

/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void MX_FREERTOS_Init(void) {
    /* USER CODE BEGIN Init */

    KeyConfig();
    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_THREADS */
    osThreadDef(keyScanTask, StartKeyScanTask, osPriorityNormal, 0, 128);
    KeyScanTaskHandle = osThreadCreate(osThread(keyScanTask), NULL);
    /* USER CODE END RTOS_THREADS */

    osThreadDef(keyTask, StartKeyTask, osPriorityNormal, 0, 128);
    KeyTaskHandle = osThreadCreate(osThread(keyTask), NULL);

    /* definition and creation of defaultTask */
    osThreadDef(led0Task, StartLed0Task, osPriorityNormal, 0, 128);
    Led0TaskHandle = osThreadCreate(osThread(led0Task), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    osThreadDef(fileTask, StartFileTask, osPriorityNormal, 0, 1024);
    FileTaskHandle = osThreadCreate(osThread(fileTask), NULL);
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */
}

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartLed0Task(void const * argument)
{

    /* USER CODE BEGIN 5 */
    /* Infinite loop */
    for(;;)
    {
        HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9 );
//	printf("Led0 test Toggle! \r\n");
        osDelay(500);
    }
    /* USER CODE END 5 */
}


/**
 * @name   KeyHandle
 * @param  none.
 * @retval none.
 * @brief  key handle
 *
 */
void
StartKeyTask( void const * argument )
{
    KeyCode_E keyCode;

    for(;;) {
        xQueueReceive( xQueueKey, &(keyCode), 0 );

        switch( keyCode )
        {
        case Key0_Press:
            HAL_GPIO_WritePin( GPIOF, GPIO_PIN_10, GPIO_PIN_SET );
            break;
        case Key0_Release:
            HAL_GPIO_WritePin( GPIOF, GPIO_PIN_10, GPIO_PIN_RESET );
            break;
        case Key0_Long:
            break;

        case Key1_Press:
            HAL_GPIO_WritePin( GPIOF, GPIO_PIN_10, GPIO_PIN_SET );
            break;
        case Key1_Release:
            HAL_GPIO_WritePin( GPIOF, GPIO_PIN_10, GPIO_PIN_RESET );
            break;
        case Key1_Long:
            break;

        case Key2_Press:
            HAL_GPIO_WritePin( GPIOF, GPIO_PIN_10, GPIO_PIN_SET );
            break;
        case Key2_Release:
            HAL_GPIO_WritePin( GPIOF, GPIO_PIN_10, GPIO_PIN_RESET );
            break;
        case Key2_Long:
            break;

        case Key3_Press:
            HAL_GPIO_WritePin( GPIOF, GPIO_PIN_10, GPIO_PIN_SET );
            break;
        case Key3_Release:
            HAL_GPIO_WritePin( GPIOF, GPIO_PIN_10, GPIO_PIN_RESET );
            break;
        case Key3_Long:
            break;

        default:
            break;
        }

        osDelay(10);
    }
}

void StartFileTask( void const *argument )
{
//	FRESULT res;                                          /* FatFs function common result code */
//	uint32_t byteswritten,bytesread;                     /* File write/read counts */
//	uint8_t wtext[] = "add some content in the last!\n\r";
    char rtext[100];                                   /* File read buffer */

    /*##-1- Link the micro SD disk I/O driver ##################################*/
    if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
    {
        /*##-2- Register the file system object to the FatFs module ##############*/
        if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0) != FR_OK)
        {
            /* FatFs Initialization Error */
            Error_Handler();
        }
        else
        {
		    f_open(&SDFile, "a167.txt", FA_CREATE_ALWAYS | FA_WRITE);
			f_close(&SDFile);

            strcpy(rtext, SDPath);
            scan_files(rtext);
//			/*##-7- Open the text file object with read access ###############*/
//			if(f_open(&SDFile, "STM32.TXT", FA_READ) != FR_OK)
//			{
//			  /* 'STM32.TXT' file Open for read Error */
//			  Error_Handler();
//			}
//			else
//			{
//			  /*##-8- Read data from the text file ###########################*/
//			  f_read(&SDFile, rtext, sizeof(rtext), (UINT*)&bytesread);
//
//			  /*##-9- Close the open text file #############################*/
//			  f_close(&SDFile);
//
//			}
        }
    }

    /*##-11- Unlink the RAM disk I/O driver ####################################*/
    FATFS_UnLinkDriver(SDPath);

//				printf((TCHAR *)rtext);

    /* Infinite Loop */
    for( ;; )
    {
    }
}


FRESULT scan_files ( char* path )
{
    FRESULT res;
    DIR dir;
    UINT i;
    UINT item = 0;
    static FILINFO fno;

    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */

			item++;
            if (fno.fattrib == AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
				res = f_opendir(&dir, path);
				if(res == FR_OK){
					for(;;){
						res = f_readdir(&dir, &fno);
						if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
						printf("%s/%s\n", path, fno.fname);
					}

					res = f_closedir(&dir);
					path[i] = 0;
					res= f_opendir(&dir, path);
					for(i=0;i<item;i++){
						res = f_readdir(&dir, &fno);
					}
				}
            }
			else {                                       /* It is a file. */
                printf("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }
    return res;
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
