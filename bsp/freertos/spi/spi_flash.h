/**
 ***********************************************************************************
 * @file  uart_drv.h
 * @brief Definitions for uart driver.
 * @author Robin Li
 * @version V1.0.0
 * @date 09/05/2018
 ************************************************************************************
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY (WUXI) CO.,LTD.</center></h2>
 ************************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint-gcc.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor SPI_FLASH instance used and associated
   resources */
/* Definition for SPI_FLASH clock resources */
#define SPI_FLASH                             SPI1
#define SPI_FLASH_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPI_FLASH_CLK_DISABLE()               __HAL_RCC_SPI1_CLK_DISABLE()
#define SPI_FLASH_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI_FLASH_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI_FLASH_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define SPI_FLASH_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
#define SPI_FLASH_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

/* Definition for SPI_FLASH Pins */
#define SPI_FLASH_SCK_PIN                     GPIO_PIN_3
#define SPI_FLASH_SCK_GPIO_PORT               GPIOB
#define SPI_FLASH_SCK_AF                      GPIO_AF5_SPI1
#define SPI_FLASH_MISO_PIN                    GPIO_PIN_4
#define SPI_FLASH_MISO_GPIO_PORT              GPIOB
#define SPI_FLASH_MISO_AF                     GPIO_AF5_SPI1
#define SPI_FLASH_MOSI_PIN                    GPIO_PIN_5
#define SPI_FLASH_MOSI_GPIO_PORT              GPIOB
#define SPI_FLASH_MOSI_AF                     GPIO_AF5_SPI1

/**
 * @brief Define the SPI behavior types.
 */


/*=============================================================
**Public function prototypes
**===========================================================*/
/*SPI configration.*/
void SpiFlashConfig(void);
void SpiFlashSetSpeed(uint8_t SPI_BaudRatePrescaler);
uint8_t SpiFlashReadWriteByte(uint8_t TxData);


#ifdef __cplusplus
}/*extern C*/
#endif

#endif /* __SPI_FLASH_H */
/* ================================ End Of File ====================================*/

