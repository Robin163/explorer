/**
 ***********************************************************************************
 * @file  key.h
 * @brief Definitions for key driver.
 *      timerbase is 10ms
 *      if you want to use the encode function,
 *      just put the below code into your main function.
 *      KeyConfig(); //add to mian.c
 *      SchAddTask( KeyScan, 41, 1 ); //add to mian.c
 *	    KeyCode_E keyCode = Key_None;
 *	    keyCode = GetKeyCodeFromBuf();
 *	    if(keyCode != Key_None) {
 *	    	switch( keyCode ) {
 *	    		case Key_EntryPress:
 *	    			break;
 *	    		case Key_BackPress:
 *	    			break;
 *	    		case Key_DownPress:
 *	    			break;
 *	    		case Key_UpPress:
 *	    			break;
 *	    		default:
 *	    			break;
 *	    	}
 *	    }
 * @author Robin Li
 * @version V1.0.0
 * @date 09/05/2018
 ************************************************************************************
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY (WUXI) CO.,LTD.</center></h2>
 ************************************************************************************
 */

#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint-gcc.h>

/*=============================================================
**Constant vector define
**===========================================================*/
/* GPIO define */
#define KEY4_Pin                        GPIO_PIN_0             /* PA.00*/
#define KEY4_GPIO_PORT                  GPIOA
#define KEY4_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY4_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOA_CLK_DISABLE()

#define KEY3_Pin                        GPIO_PIN_2             /* PA.00*/
#define KEY3_GPIO_PORT                  GPIOE
#define KEY3_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY3_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()

#define KEY2_Pin                        GPIO_PIN_3             /* PA.00*/
#define KEY2_GPIO_PORT                  GPIOE
#define KEY2_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY2_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()

#define KEY1_Pin                        GPIO_PIN_4             /* PA.00*/
#define KEY1_GPIO_PORT                  GPIOE
#define KEY1_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY1_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()

#define KEY_MAX_COUNT  4

/**
 * @brief define key ID
 */
typedef enum
{
    KID_Entry = 0,
    KID_Back,
    KID_Down,
    KID_Up,
} KeyID_E;

/**
 * @brief define key signal
 */
typedef enum
{
    Key_None = 0,			/* 0 表示按键事件 */

    Key_EntryPress,
    Key_EntryRelease,
    Key_EntryLong,

    Key_BackPress,
    Key_BackRelease,
    Key_BackLong,

    Key_DownPress,
    Key_DownRelease,
    Key_DownLong,

    Key_UpPress,
    Key_UpRelease,
    Key_UpLong,

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

/* key buffer size (max store key values) */
#define KEY_BUF_SIZE	20
typedef struct
{
    KeyCode_E keyBuf[KEY_BUF_SIZE];		/* 键值缓冲区 */
    uint8_t read;					/* 缓冲区读指针1 */
    uint8_t write;					/* 缓冲区写指针 */
    uint8_t read2;					/* 缓冲区读指针2 */
} KeyBuf_T;

/*=============================================================
**Public function prototypes
**===========================================================*/
/* KEY configration. */
extern void KeyConfig( void );
extern void PutKeyCodeToBuf( KeyCode_E keyCode );
extern KeyCode_E GetKeyCodeFromBuf( void );
extern void KeyScan( void );

#ifdef __cplusplus
}/*extern C*/
#endif

#endif /* __KEY_H */
/* ================================ End Of File ====================================*/

