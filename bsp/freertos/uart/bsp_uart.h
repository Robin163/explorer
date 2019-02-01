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

#ifndef __BSP_UART_H
#define __BSP_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint-gcc.h>

/*=============================================================
**Constant vector define
**===========================================================*/
//#define BEEP_Pin                         GPIO_PIN_8             /* PF.08*/
//#define BEEP_GPIO_PORT                   GPIOF
//#define BEEP_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
//#define BEEP_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()
//
//#define UART0_Pin                         GPIO_PIN_9             /* PF.09*/
//#define UART0_GPIO_PORT                   GPIOF
//#define UART0_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
//#define UART0_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()
//
//#define UART1_Pin                         GPIO_PIN_10            /* PF.10*/
//#define UART1_GPIO_PORT                   GPIOF
//#define UART1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
//#define UART1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()


/**
 * @brief Define the UART behavior types.
 */


/*=============================================================
**Public function prototypes
**===========================================================*/
/*UART configration.*/
extern void UartConfig( void );
extern void  StartUartSendTask(  void const * argument );

#ifdef __cplusplus
}/*extern C*/
#endif

#endif /* __BSP_UART_H */
/* ================================ End Of File ====================================*/

