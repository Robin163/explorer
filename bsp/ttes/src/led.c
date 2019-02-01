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
#include "led.h"

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
    LED2_GPIO_CLK_ENABLE();
    LED3_GPIO_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_Pin, GPIO_PIN_SET);

	/*Configure GPIO pins : Led2_pin */
	GPIO_InitStruct.Pin = LED2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);

	/*Configure GPIO pins : Led3_pin */
	GPIO_InitStruct.Pin = LED3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);
}

void Led2Toggle( void )
{
	HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_Pin);
}

/** @} */

/* ================================ End Of File ====================================*/

