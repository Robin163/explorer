/**
 ***********************************************************************************
 * @weakgroup bsp_driver
 * @{
 * @brief Device drivers module.
 * @file  encode.c
 * @brief input driver.
 *      1. timetick is 1ms.
 *      2. KeyConfig() //add to mian.c
 *      3. SchAddTask( KeyScan, 41, 1 ); //add to mian.c
 *      4. GetKeyCodeFromBuf(); //call by some functon
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
#include "encode.h"

/* Externals ---------------------------------------------------------*/

/* static  Variables ----------------------------------------------*/
static uint8_t keyAStatus = 0;
static uint8_t keyBStatus = 0;
static KeyBuf_T   g_keyBuf;

/* Private functions ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void KeyConfigGPIO( void );
static void KeyConfigValue( void );
static void Key1CheckStatus(void);
static void KeyACheckStatus(void);
static void KeyBCheckStatus(void);

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
    SA_GPIO_CLK_ENABLE();
    SB_GPIO_CLK_ENABLE();
    KEY1_GPIO_CLK_ENABLE();

    /*Configure GPIO pins : SA_Pin */
    GPIO_InitStruct.Pin = SA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SA_GPIO_PORT, &GPIO_InitStruct);

    /*Configure GPIO pins : SB_Pin */
    GPIO_InitStruct.Pin = SB_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SB_GPIO_PORT, &GPIO_InitStruct);

    /*Configure GPIO pins : KEY1_Pin */
    GPIO_InitStruct.Pin = KEY1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * @name   KeyConfigValue
 * @param  None
 * @retval None
 * @brief  1. reserve
 *
 */
static void KeyConfigValue( void )
{
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
    /* USER CODE BEGIN StartDefaultTask */
    Key1CheckStatus();
    KeyACheckStatus();
    KeyBCheckStatus();
    /* USER CODE END StartDefaultTask */
}

/**
 * @name   Key1CheckStatus()
 * @param  None
 * @retval None
 * @brief  1. press once in 500ms is Key1_Pressed
 *         2. press 2 times in 500ms is Key1_Double
 *
 */
static void Key1CheckStatus( void )
{
    static uint8_t key1Status = 0;
    static uint8_t key1Cnt = 0;
    /* count the interval of pressed 2 times */
    static uint16_t key1IntervalCnt = 2*DOUBLE_PRESS_MAX_INTERVAL;
    /* no timer delay count after pressed2 */
    static uint8_t sendMsgOk = 0;

    key1Cnt++;
    key1IntervalCnt++;
    if( key1Status == 0) {
        if(!HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_Pin)) {
            if(key1Cnt >= KEY_FILTER_NUM) {
                key1Status = 1;
                /* pressed 2 times within 500ms */
                if( key1IntervalCnt < DOUBLE_PRESS_MAX_INTERVAL ) {
                    PutKeyCodeToBuf(Key1_Double);
                    sendMsgOk = 1;
                    key1IntervalCnt = 2*DOUBLE_PRESS_MAX_INTERVAL;
                }
            }
        }
        else {
            key1Cnt = 0;
            /* no pressed second time with in 500ms */
            if(key1IntervalCnt > DOUBLE_PRESS_MAX_INTERVAL) {
                key1IntervalCnt = 2*DOUBLE_PRESS_MAX_INTERVAL;
            }
            else if( key1IntervalCnt == DOUBLE_PRESS_MAX_INTERVAL ) {
                PutKeyCodeToBuf(Key1_Pressed);
            }
            else {
            }
        }
    }
    else//status ==1
    {
        if(HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_Pin)) {
            if(key1Cnt >= KEY_FILTER_NUM) {
                key1Status = 0;
                /* no re-counter if after pressed 2 in 500ms */
                if ( sendMsgOk ) {
                    sendMsgOk = 0;
                }
                else{
                    key1IntervalCnt = 0;
                }
            }
        }
        else {
            key1Cnt = 0;
        }
    }
}

/**
 * @name   KeyACheckStatus
 * @param  None
 * @retval None
 * @brief  1. check encode SA status.
 *         2. timetick is 1ms. normally the debounce time less than 3ms.
 *
 */
static void KeyACheckStatus( void )
{
    static uint8_t keyACnt = 0;

    keyACnt++;
    if( keyAStatus == 0) {
        if(HAL_GPIO_ReadPin(SA_GPIO_PORT, SA_Pin)) {
            if(keyACnt >= ENCODE_FILTER_NUM) {
                keyAStatus = 1;
            }
        }
        else {
            keyACnt = 0;
        }
    }
    else {
        if(!HAL_GPIO_ReadPin(SA_GPIO_PORT, SA_Pin)) {
            if(keyACnt >= ENCODE_FILTER_NUM) {
                keyAStatus = 0;
            }
        }
        else {
            keyACnt = 0;
        }
    }
}

/**
 * @name   KeyScan
 * @param  None
 * @retval None
 * @brief  1. press once in 500ms is Key1_Pressed
 *         2. press 2 times in 500ms is Key1_Double
 *
 */
static void KeyBCheckStatus( void )
{
    static uint8_t keyBCnt = 0;

    keyBCnt++;
    if( keyBStatus == 0) {
        if(HAL_GPIO_ReadPin(SB_GPIO_PORT, SB_Pin)) {
            if(keyBCnt >= ENCODE_FILTER_NUM) {
                keyBStatus = 1;
                if(keyAStatus == 1) {
                    PutKeyCodeToBuf(Encode_CW);
                }
                else {
                    PutKeyCodeToBuf(Encode_CCW);
                }
            }
        }
        else {
            keyBCnt = 0;
        }
    }
    else {
        if(!HAL_GPIO_ReadPin(SB_GPIO_PORT, SB_Pin)) {
            if(keyBCnt >= ENCODE_FILTER_NUM) {
                keyBStatus = 0;
            }
        }
        else {
            keyBCnt = 0;
        }
    }
}

/** @} */

/* ================================ End Of File ====================================*/

