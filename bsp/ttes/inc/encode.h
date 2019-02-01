/**
 ***********************************************************************************
 * @file  encode.h
 * @brief Definitions for encode driver.
 *      if you want to use the encode function,
 *      just put the below code into your main function.
 *      KeyConfig(); //add to mian.c
 *      SchAddTask( KeyScan, 41, 1 ); //add to mian.c
 *	    KeyCode_E keyCode = Key_None;
 *	    keyCode = GetKeyCodeFromBuf();
 *	    if(keyCode != Key_None) {
 *	    	switch( keyCode ) {
 *	    		case Key1_Pressed:
 *	    			break;
 *	    		case Key1_Double:
 *	    			break;
 *	    		case Encode_CW:
 *	    			break;
 *	    		case Encode_CCW:
 *	    			break;
 *	    		default:
 *	    			break;
 *	    	}
 *	    }
 * @author Robin Li
 * @version V1.0.0
 * @date 09/05/2018
 *
 ************************************************************************************
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY (WUXI) CO.,LTD.</center></h2>
 ************************************************************************************
 */

#ifndef __ENCODE_H
#define __ENCODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint-gcc.h>

/*=============================================================
 **Constant vector define
 **===========================================================*/
/* GPIO define */
#define SA_Pin                      GPIO_PIN_0             /* PA.00*/
#define SA_GPIO_PORT                GPIOA
#define SA_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define SA_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()

#define SB_Pin                      GPIO_PIN_1             /* PA.01*/
#define SB_GPIO_PORT                GPIOA
#define SB_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define SB_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()

#define KEY1_Pin                    GPIO_PIN_2             /* PA.02*/
#define KEY1_GPIO_PORT              GPIOA
#define KEY1_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY1_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOA_CLK_DISABLE()

#define KEY_FILTER_NUM      30 /* timebase = 1ms; filter 30ms */
#define ENCODE_FILTER_NUM   3 /* timebase = 1ms; filter 3ms */
#define DOUBLE_PRESS_MAX_INTERVAL 500 /* timebase = 1ms; interval is 500ms */

/**
 * @brief define encode signal
 */
typedef enum
{
    Key_None = 0,
    Key1_Pressed,
    Key1_Released,
    Key1_Double,
    Encode_CW,
    Encode_CCW,
} KeyCode_E;

/* encode buffer size (max store encode values) */
#define KEY_BUF_SIZE	20
/**
 * @brief have one struct every one encode
 */
typedef struct
{
    KeyCode_E keyBuf[KEY_BUF_SIZE];		/* keyCode buffer */
    uint8_t read;					/* buffer read point */
    uint8_t write;					/* buffer write point*/
} KeyBuf_T;

/*=============================================================
 **Public function prototypes
 **===========================================================*/
extern void KeyConfig( void );
extern void KeyScan( void );
extern KeyCode_E GetKeyCodeFromBuf( void );

#ifdef __cplusplus
}/*extern C*/
#endif

#endif /* __ENCODE_H */
/* ================================ End Of File ====================================*/

