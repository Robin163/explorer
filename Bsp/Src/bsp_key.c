/**
 ***********************************************************************************
 * @weakgroup bsp_driver
 * @{
 * @brief Device drivers module.
 * @file  input.c
 * @brief input driver.
 * @author Robin Li
 * @version V1.0.0
 * @date 09/05/2018
 ************************************************************************************
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY (WUXI) CO.,LTD.</center></h2>
 ************************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint-gcc.h>

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "bsp_key.h"
#include "cmsis_os.h"

/* Externals ---------------------------------------------------------*/

/* static  Variables ----------------------------------------------*/
static KeyInfor_T g_keyInfor[KEY_MAX_COUNT];
xQueueHandle xQueueKey;

/* Private functions ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//static void KeyConfigGPIO( void );
static void KeyConfigValue( void );
static uint8_t IsKey0Pressed( void );
static uint8_t IsKey1Pressed( void );
static uint8_t IsKey2Pressed( void );
static uint8_t IsKey3Pressed( void );

/** @addtogroup Template_Project
  * @{
  */

/**
 * @brief   input configration.
 * @param   None.
 * @retval  None.
 * @sa
 *
 */
void KeyConfig( void )
{
    //    KeyConfigGPIO();
    KeyConfigValue();
}

/**
 * @brief   input configration.
 * @param   None.
 * @retval  None.
 * @sa
 *
 */
//static void KeyConfigGPIO( void ) {
//    /* config input gpio */
//    GPIO_InitTypeDef initInput = GPIOD_INIT_INPUT_PARA;
//    /* Enable LED GPIO and AFIO clock */
//    RCC_APB2PeriphClockCmd( RCC_GPIOD_INPUT, ENABLE );
//    /* LED pins configuration */
//    GPIO_Init( GPIOD, &initInput );
//    /* config input gpio */
//    initInput.GPIO_Pin = PINE_INPUT_ALL;
//    /* Enable LED GPIO and AFIO clock */
//    RCC_APB2PeriphClockCmd( RCC_GPIOE_INPUT, ENABLE );
//    /* LED pins configuration */
//    GPIO_Init( PORTE_INPUT, &initInput );
//}

/**
 * @brief   key config value
 * @param   None.
 * @retval  None.
 * @sa
 *
 */
static void KeyConfigValue( void )
{
	xQueueKey = xQueueCreate( 10, sizeof(uint8_t) );
	if( xQueueKey ==0 ){

	}

    /* 给每个按键结构体成员变量赋一组缺省值 */
    for( uint8_t keyCnt = 0; keyCnt < KEY_MAX_COUNT; keyCnt++ )
    {
        g_keyInfor[keyCnt].longTime = KEY_LONG_TIME;			/* 长按时间 0 表示不检测长按键事件 */
        g_keyInfor[keyCnt].cnt = KEY_FILTER_TIME / 2;		/* 计数器设置为滤波时间的一半 */
        g_keyInfor[keyCnt].status = 0;							/* 按键缺省状态，0为未按下 */
        g_keyInfor[keyCnt].rptSpeed = 0;						/* 按键连发的速度，0表示不支持连发 */
        g_keyInfor[keyCnt].rptCnt = 0;						/* 连发计数器 */
    }

    /* 如果需要单独更改某个按键的参数，可以在此单独重新赋值 */
    /* 比如，我们希望按键1按下超过1秒后，自动重发相同键值 */
    g_keyInfor[KID_Key0].longTime = 100;
    g_keyInfor[KID_Key0].rptSpeed = 5;	/* 每隔50ms自动发送键值 */
    g_keyInfor[KID_Key1].longTime = 100;
    g_keyInfor[KID_Key1].rptSpeed = 5;	/* 每隔50ms自动发送键值 */
    /* 判断按键按下的函数 */
    g_keyInfor[0].IsKeyPressFunc = IsKey0Pressed;
    g_keyInfor[1].IsKeyPressFunc = IsKey1Pressed;
    g_keyInfor[2].IsKeyPressFunc = IsKey2Pressed;
    g_keyInfor[3].IsKeyPressFunc = IsKey3Pressed;
}

/**
 * @brief   read button signal
 * @param   None.
 * @retval  None.
 * @sa
 *
 */
static uint8_t IsKey0Pressed( void )
{
    return ( HAL_GPIO_ReadPin( GPIOE, GPIO_PIN_4 ) );
}

static uint8_t IsKey1Pressed( void )
{
    return ( HAL_GPIO_ReadPin( GPIOE, GPIO_PIN_3 ) );
}

static uint8_t IsKey2Pressed( void )
{
    return ( HAL_GPIO_ReadPin( GPIOE, GPIO_PIN_2 ) );
}
static uint8_t IsKey3Pressed( void )
{
    return ( !HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_0 ) );
}


/*
*********************************************************************************************************
*	函 数 名: bsp_DetectKey
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参:  按键结构变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void KeyCheckStatus( uint8_t keyID )
{
    KeyInfor_T *pNowInfor;
    pNowInfor = &g_keyInfor[keyID];
	KeyCode_E keyValue = 0;

    if( pNowInfor->IsKeyPressFunc() )
    {
        if( pNowInfor->cnt < KEY_FILTER_TIME )
        {
            pNowInfor->cnt = KEY_FILTER_TIME;
        }
        else if( pNowInfor->cnt < 2 * KEY_FILTER_TIME )
        {
            pNowInfor->cnt++;
        }
        else
        {
            if( pNowInfor->status == 0 )
            {
                pNowInfor->status = 1;
                /* send key is pressed signal */

//				  HAL_GPIO_TogglePin( GPIOD, GPIO_PIN_10);
				keyValue = 3*keyID + 1;
				xQueueSend( xQueueKey, &(keyValue), 100 );
            }

            if( pNowInfor->longTime > 0 )
            {
                if( pNowInfor->longCnt < pNowInfor->longTime )
                {
                    /* send key is continue pressed signal */
                    if( ++pNowInfor->longCnt == pNowInfor->longTime )
                    {
                        /* put key Value to buffer */
						keyValue = 3*keyID + 3;
						xQueueSend( xQueueKey, &(keyValue), 100 );
//						xQueueSend( xQueueKey, &(3*keyID +3), 100 );
                    }
                }

                //				else {
                //					if ( pNowInfor->rptSpeed > 0 ) {
                //						if ( ++pNowInfor->rptCnt >= pNowInfor->rptSpeed ) {
                //							pNowInfor->rptCnt = 0;
                //							/* 常按键后，每隔10ms发送1个按键 */
                //							PutKeyCodeToBuf( (KeyCode_E)(3 * keyID + 1) );
                //						}
                //					}
                //				}
            }
        }
    }
    else
    {
        if( pNowInfor->cnt > KEY_FILTER_TIME )
        {
            pNowInfor->cnt = KEY_FILTER_TIME;
        }
        else if( pNowInfor->cnt != 0 )
        {
            pNowInfor->cnt--;
        }
        else
        {
            if( pNowInfor->status == 1 )
            {
                pNowInfor->status = 0;
                /* send key release signal */
				keyValue = 3*keyID + 2;
				xQueueSend( xQueueKey, &(keyValue), 100 );
            }
        }

        pNowInfor->longCnt = 0;
        pNowInfor->rptCnt = 0;
    }
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyScan
*	功能说明: 扫描所有按键。非阻塞，被systick中断周期性的调用
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void StartKeyScanTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    for( uint8_t keyCnt = 0; keyCnt < KEY_MAX_COUNT; keyCnt++ )
    {
        KeyCheckStatus( keyCnt );
    }
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/** @} */

/* ================================ End Of File ====================================*/

