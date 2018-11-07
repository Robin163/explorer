/**
 ***********************************************************************************
 * @weakgroup bsp_driver
 * @{
 * @brief Device drivers module.
 * @file  spi.c
 * @brief SPI TX and RX driver.
 * @author Robin Li
 * @version V1.0.0
 * @date 09/05/2018
 ***********************************************************************************
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY (WUXI) CO.,LTD.</center></h2>
 ***********************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint-gcc.h>

#include "stm32f1xx_hal.h"
//#include "stm32f1xx_hal_gpio.h"
#include "spi_lcd.h"


/** @addtogroup Template_Project
  * @{
  */

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/*
*********************************************************************************************************
*	函 数 名: OLED_WriteCmd
*	功能说明: 向SSD1306发送一字节命令
*	形    参:  命令字
*	返 回 值: 无
*********************************************************************************************************
*/
void WriteCommand( uint8_t cmd )
{
    uint8_t cmdTemp = cmd;
    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8,  GPIO_PIN_RESET );
    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_10, GPIO_PIN_RESET );
    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_12, GPIO_PIN_RESET );
    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_10, GPIO_PIN_SET );

    for( uint8_t i = 0; i < 8; i++ )
    {
        if( cmdTemp & 0x80 )
        {
            HAL_GPIO_WritePin( GPIOC, GPIO_PIN_12, GPIO_PIN_SET );
        }
        else
        {
            HAL_GPIO_WritePin( GPIOC, GPIO_PIN_12, GPIO_PIN_RESET );
        }

        HAL_GPIO_WritePin( GPIOC, GPIO_PIN_10, GPIO_PIN_RESET );
        cmdTemp <<= 1;
        HAL_GPIO_WritePin( GPIOC, GPIO_PIN_10, GPIO_PIN_SET );
    }

    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_SET );
}

/*
*********************************************************************************************************
*	函 数 名: OLED_WriteData
*	功能说明: 向SSD1306发送一字节数据
*	形    参:  命令字
*	返 回 值: 无
*********************************************************************************************************
*/
void WriteData( uint8_t data )
{
    uint8_t dataTemp = data;
    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_RESET );
    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_10, GPIO_PIN_RESET );
    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_12, GPIO_PIN_SET );
    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_10, GPIO_PIN_SET );

    for( uint8_t i = 0; i < 8; i++ )
    {
        if( dataTemp & 0x80 )
        {
            HAL_GPIO_WritePin( GPIOC, GPIO_PIN_12, GPIO_PIN_SET );
        }
        else
        {
            HAL_GPIO_WritePin( GPIOC, GPIO_PIN_12, GPIO_PIN_RESET );
        }

        HAL_GPIO_WritePin( GPIOC, GPIO_PIN_10, GPIO_PIN_RESET );
        dataTemp <<= 1;
        HAL_GPIO_WritePin( GPIOC, GPIO_PIN_10, GPIO_PIN_SET );
    }

    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_SET );
}

/*
*********************************************************************************************************
*	函 数 名: TurnOnLcdReset
*	功能说明: 向SSD1306发送一字节数据
*	形    参:  命令字
*	返 回 值: 无
*********************************************************************************************************
*/
void TurnOnLcdReset( void )
{
    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_RESET );
}

/*
*********************************************************************************************************
*	函 数 名: TurnOffLcdReset
*	功能说明: 向SSD1306发送一字节数据
*	形    参:  命令字
*	返 回 值: 无
*********************************************************************************************************
*/
void TurnOffLcdReset( void )
{
    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_SET );
}

/** @} */

/* ================================ End Of File ====================================*/

