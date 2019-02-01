/**
 ***********************************************************************************
 * @weakgroup bsp_driver
 * @{
 * @brief Device drivers module.
 *      1. timetick is 10ms.
 *      2. KeyConfig() //add to mian.c
 *      3. SchAddTask( KeyScan, 41, 1 ); //add to mian.c
 *      4. GetKeyCodeFromBuf(); //call by some functon
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
#include "main.h"
#include "stm32f1xx_hal.h"
#include "key.h"

/* Externals ---------------------------------------------------------*/

/* static  Variables ----------------------------------------------*/
static KeyInfor_T g_keyInfor[KEY_MAX_COUNT];
static KeyBuf_T   g_keyBuf;

/* Private functions ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void KeyConfigGPIO( void );
static void KeyConfigValue( void );
static uint8_t IsKeyEntryPressed( void );
static uint8_t IsKeyBackPressed( void );
static uint8_t IsKeyDownPressed( void );
static uint8_t IsKeyUpPressed( void );
static void KeyCheckStatus( uint8_t keyID );

/** @addtogroup Template_Project
  * @{
  */

/**
 * @name   KeyConfig
 * @param  None
 * @retval None
 * @brief  1. Configure GPIO.
 *         2. Configure Value.
 */
void KeyConfig( void )
{
    KeyConfigGPIO();
    KeyConfigValue();
}

/**
 * @name   KeyConfigGPIO
 * @param  None
 * @retval None
 * @brief  1. enable ports Clock.
 *         2. Configure gpio as input.
 *
 */
static void KeyConfigGPIO( void ) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    KEY1_GPIO_CLK_ENABLE();
    KEY2_GPIO_CLK_ENABLE();
    KEY3_GPIO_CLK_ENABLE();
    KEY4_GPIO_CLK_ENABLE();

    /*Configure GPIO pins : KEY1_Pin */
    GPIO_InitStruct.Pin = KEY1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);

    /*Configure GPIO pins : KEY2_Pin */
    GPIO_InitStruct.Pin = KEY2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);

    /*Configure GPIO pins : KEY3_Pin */
    GPIO_InitStruct.Pin = KEY3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStruct);

    /*Configure GPIO pins : KEY4_Pin */
    GPIO_InitStruct.Pin = KEY4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStruct);

}

/**
 * @name   KeyConfigValue
 * @param  None
 * @retval None
 * @brief  1. clean FIFO buffer.
 *         2. Configure Value.
 */
static void KeyConfigValue( void )
{
    /* 对按键FIFO读写指针清零 */
    g_keyBuf.read = 0;
    g_keyBuf.write = 0;
    g_keyBuf.read2 = 0;

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
//    g_keyInfor[KID_Down].longTime = 100;
//    g_keyInfor[KID_Down].rptSpeed = 5;	/* 每隔50ms自动发送键值 */
//    g_keyInfor[KID_Up].longTime = 100;
//    g_keyInfor[KID_Up].rptSpeed = 5;	/* 每隔50ms自动发送键值 */
    /* 判断按键按下的函数 */
    g_keyInfor[0].IsKeyPressFunc = IsKeyEntryPressed;
    g_keyInfor[1].IsKeyPressFunc = IsKeyBackPressed;
    g_keyInfor[2].IsKeyPressFunc = IsKeyDownPressed;
    g_keyInfor[3].IsKeyPressFunc = IsKeyUpPressed;
}

/**
 * @name   IsKeyPressed
 * @param  None
 * @retval None
 * @brief  1. read key IO signal
 *
 */
static uint8_t IsKeyEntryPressed( void )
{
    return ( !HAL_GPIO_ReadPin( KEY1_GPIO_PORT, KEY1_Pin ) );
}

static uint8_t IsKeyBackPressed( void )
{
    return ( !HAL_GPIO_ReadPin( KEY2_GPIO_PORT, KEY2_Pin ) );
}
static uint8_t IsKeyDownPressed( void )
{
    return ( !HAL_GPIO_ReadPin( KEY3_GPIO_PORT, KEY3_Pin ) );
}
static uint8_t IsKeyUpPressed( void )
{
    return ( HAL_GPIO_ReadPin( KEY4_GPIO_PORT, KEY4_Pin ) );
}

/**
 * @name   PutKeyCodeToBuf
 * @param  keycode.
 * @retval None
 * @brief  1. write keycode into array in turn.
 *
 */
void PutKeyCodeToBuf( KeyCode_E keyCode )
{
    g_keyBuf.keyBuf[g_keyBuf.write] = keyCode;
    g_keyBuf.write++;

    if( g_keyBuf.write  >= KEY_BUF_SIZE )
    {
        g_keyBuf.write = 0;
    }
}


/**
 * @name   GetKeyCodeFromBuf
 * @param  None
 * @retval keyCode value
 * @brief  1. read keycode from array in turn.
 *
 */
KeyCode_E GetKeyCodeFromBuf( void )
{
    KeyCode_E keyCode = Key_None;

    if( g_keyBuf.read != g_keyBuf.write )
    {
        keyCode = g_keyBuf.keyBuf[g_keyBuf.read];
        g_keyBuf.read++;

        if( g_keyBuf.read >= KEY_BUF_SIZE )
        {
            g_keyBuf.read = 0;
        }
    }

    return keyCode;
}

/**
 * @name   KeyScan
 * @param  None
 * @retval None
 * @brief  1. polling encode signal in turn.
 *
 */
void KeyScan( void )
{
    for( uint8_t keyCnt = 0; keyCnt < KEY_MAX_COUNT; keyCnt++ )
    {
        KeyCheckStatus( keyCnt );
    }
}

/**
 * @name   KeyCheckStatus()
 * @param  keyID
 * @retval None
 * @brief  1. handle key pressed and released.
 *         2. handle key long pressed and continue pressed.
 *
 */
static void KeyCheckStatus( uint8_t keyID )
{
    KeyInfor_T *pNowInfor;
    pNowInfor = &g_keyInfor[keyID];

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
                PutKeyCodeToBuf( ( KeyCode_E )( 3 * keyID + 1 ) );
            }

            if( pNowInfor->longTime > 0 )
            {
                if( pNowInfor->longCnt < pNowInfor->longTime )
                {
                    /* send key is continue pressed signal */
                    if( ++pNowInfor->longCnt == pNowInfor->longTime )
                    {
                        /* put key Value to buffer */
                        PutKeyCodeToBuf( ( KeyCode_E )( 3 * keyID + 3 ) );
                    }
                }
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
                PutKeyCodeToBuf( ( KeyCode_E )( 3 * keyID + 2 ) );
            }
        }

        pNowInfor->longCnt = 0;
        pNowInfor->rptCnt = 0;
    }
}



/** @} */

/* ================================ End Of File ====================================*/

