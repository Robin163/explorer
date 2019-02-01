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

#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint-gcc.h>

/*=============================================================
**Constant vector define
**===========================================================*/
#define LED2_Pin                         GPIO_PIN_9             /* PA.09*/
#define LED2_GPIO_PORT                   GPIOA
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

#define LED3_Pin                         GPIO_PIN_8             /* PA.08*/
#define LED3_GPIO_PORT                   GPIOA
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()



/**
 * @brief Define the LED behavior types.
 */


/*=============================================================
**Public function prototypes
**===========================================================*/
/*LED configration.*/
extern void LedConfig( void );
extern void Led2Toggle( void );

#ifdef __cplusplus
}/*extern C*/
#endif

#endif /* __LED_H */
/* ================================ End Of File ====================================*/

