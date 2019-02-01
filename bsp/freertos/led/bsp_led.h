/**
 ***********************************************************************************
 * @file  led_drv.h
 * @brief Definitions for led driver.
 * @author Robin Li
 * @version V1.0.0
 * @date 09/05/2018
 ************************************************************************************
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY (WUXI) CO.,LTD.</center></h2>
 ************************************************************************************
 */

#ifndef __BSP_LED_H
#define __BSP_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint-gcc.h>

/*=============================================================
**Constant vector define
**===========================================================*/
#define BEEP_Pin                         GPIO_PIN_8             /* PF.08*/
#define BEEP_GPIO_PORT                   GPIOF
#define BEEP_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define BEEP_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()

#define LED0_Pin                         GPIO_PIN_9             /* PF.09*/
#define LED0_GPIO_PORT                   GPIOF
#define LED0_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED0_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()

#define LED1_Pin                         GPIO_PIN_10            /* PF.10*/
#define LED1_GPIO_PORT                   GPIOF
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()


/**
 * @brief Define the LED behavior types.
 */


/*=============================================================
**Public function prototypes
**===========================================================*/
/*LED configration.*/
extern void LedConfig( void );
extern void  StartLedToggleTask(  void const * argument );

#ifdef __cplusplus
}/*extern C*/
#endif

#endif /* __BSP_LED_H */
/* ================================ End Of File ====================================*/

