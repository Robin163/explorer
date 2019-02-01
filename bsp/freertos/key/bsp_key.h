/**
 ***********************************************************************************
 * @file  key.h
 * @brief Definitions for key driver.
 * @author Robin Li
 * @version V1.0.0
 * @date 09/05/2018
 ************************************************************************************
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY (WUXI) CO.,LTD.</center></h2>
 ************************************************************************************
 */

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint-gcc.h>

/*=============================================================
**Constant vector define
**===========================================================*/
/* GPIO define */
#define KEY3_Pin                        GPIO_PIN_0             /* PA.00*/
#define KEY3_GPIO_PORT                  GPIOA
#define KEY3_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY3_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOA_CLK_DISABLE()

#define KEY2_Pin                        GPIO_PIN_2             /* PE.02*/
#define KEY2_GPIO_PORT                  GPIOE
#define KEY2_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY2_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()

#define KEY1_Pin                        GPIO_PIN_3             /* PE.03*/
#define KEY1_GPIO_PORT                  GPIOE
#define KEY1_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY1_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()

#define KEY0_Pin                        GPIO_PIN_4             /* PE.04*/
#define KEY0_GPIO_PORT                  GPIOE
#define KEY0_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY0_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()

#define KEY_MAX_COUNT  4

/**
 * @brief define key ID
 */
typedef enum {
    KID_Key0 = 0,
    KID_Key1,
    KID_Key2,
    KID_Key3,
} KeyID_E;

/**
 * @brief define key signal
 */
typedef enum {
    Key_None = 0,			/* 0 表示按键事件 */

    Key0_Press,
    Key0_Release,
    Key0_Long,

    Key1_Press,
    Key1_Release,
    Key1_Long,

    Key2_Press,
    Key2_Release,
    Key2_Long,

    Key3_Press,
    Key3_Release,
    Key3_Long,
} KeyCode_E;

#define KEY_FILTER_TIME   8 /* timebase = 10ms; filter 80ms */
#define KEY_LONG_TIME     100 /* timebase = 10ms; longpress is 1s */

/**
 * @brief have one struct every one key
 */
typedef struct
{
    uint8_t ( *IsKeyPressFunc )( void ); /* 按键按下的判断函数,1表示按下 */

    uint8_t  cnt;			/* 滤波器计数器 */
    uint16_t longCnt;		/* 长按计数器 */
    uint16_t longTime;		/* 按键按下持续时间, 0表示不检测长按 */
    uint8_t  status;		/* 按键当前状态（按下还是弹起） */
    uint8_t  rptSpeed;	/* 连续按键周期 */
    uint8_t  rptCnt;	/* 连续按键计数器 */
} KeyInfor_T;

extern xQueueHandle xQueueKey;

/*=============================================================
**Public function prototypes
**===========================================================*/
/* KEY configration. */
extern void KeyConfig( void );
extern void StartKeyScanTask( void const * argument );

#ifdef __cplusplus
}/*extern C*/
#endif

#endif /* __BSP_KEY_H */
/* ================================ End Of File ====================================*/

