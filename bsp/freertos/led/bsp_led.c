/**
 ***********************************************************************************
 * @weakgroup bsp_driver
 * @{
 * @brief Device drivers module.
 * @file  oled.c
 * @brief OLED driver.
 * @author Robin Li
 * @version V1.0.0
 * @date 09/05/2018
 ************************************************************************************
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY (WUXI) CO.,LTD.</center></h2>
 ************************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint-gcc.h>
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "cmsis_os.h"

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/** @addtogroup Template_Project
  * @{
  */

/**
 * @brief   OLED configration.
 * @param   None.
 * @retval  None.
 * @sa
 *
 */
void LedConfig( void )
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
    BEEP_GPIO_CLK_ENABLE();
    LED0_GPIO_CLK_ENABLE();
    LED1_GPIO_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_Pin, GPIO_PIN_SET);

	/*Configure GPIO pins : BEEP_pin */
	GPIO_InitStruct.Pin = BEEP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStruct);

	/*Configure GPIO pins : Led0_pin */
	GPIO_InitStruct.Pin = LED0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStruct);

	/*Configure GPIO pins : Led1_pin */
	GPIO_InitStruct.Pin = LED1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);
}

void  StartLedToggleTask(  void const * argument )
{
    /* USER CODE BEGIN 5 */
    /* Infinite loop */
    for(;;)
    {
//        HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_Pin);
//        osDelay(500);

        KeyCode_E keyCode = Key_None;
        xQueueReceive(xQueueKey, &keyCode, 10);
        if(keyCode != Key_None){
            switch( keyCode ) {
                case Key0_Press:
                    HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_Pin, GPIO_PIN_RESET);
                    break;
                case Key0_Release:
                    HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_Pin, GPIO_PIN_SET);
                    break;
                case Key1_Press:
                    HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_Pin, GPIO_PIN_RESET);
                    break;
                case Key1_Release:
                    HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_Pin, GPIO_PIN_SET);
                    break;
                case Key2_Press:
                    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_Pin, GPIO_PIN_RESET);
                    break;
                case Key2_Release:
                    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_Pin, GPIO_PIN_SET);
                    break;
                case Key3_Press:
                    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_Pin, GPIO_PIN_RESET);
                    break;
                case Key3_Release:
                    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_Pin, GPIO_PIN_SET);
                    break;
                default:
                    break;
            }
        }
    }
}

/** @} */

/* ================================ End Of File ====================================*/

