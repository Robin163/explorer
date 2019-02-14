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
#ifndef __UART_COM_H
#define __UART_COM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint-gcc.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USART_COM/UARTx instance used and associated
   resources */
/* Definition for USART_COM clock resources */
#define USART_COM                           USART3
#define USART_COM_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE()
#define USART_COM_CLK_DISABLE()             __HAL_RCC_USART3_CLK_DISABLE()
#define USART_COM_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USART_COM_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define USART_COM_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USART_COM_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USART_COM Pins */
#define USART_COM_TX_PIN                    GPIO_PIN_10
#define USART_COM_TX_GPIO_PORT              GPIOB
#define USART_COM_TX_AF                     GPIO_AF7_USART3
#define USART_COM_RX_PIN                    GPIO_PIN_11
#define USART_COM_RX_GPIO_PORT              GPIOB
#define USART_COM_RX_AF                     GPIO_AF7_USART3


/**
 * @brief Define the UART behavior types.
 */


/*=============================================================
**Public function prototypes
**===========================================================*/
/*UART configration.*/
extern void UartComConfig( void );
extern void  StartUartSendTask(  void const * argument );

#ifdef __cplusplus
}/*extern C*/
#endif

#endif /* __UART_COM_H */
/* ================================ End Of File ====================================*/

