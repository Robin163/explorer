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

#ifndef __DISPLAY_H
#define __DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint-gcc.h>

/*=============================================================
**Constant vector define
**===========================================================*/
#define RST_Pin GPIO_PIN_0
#define RST_GPIO_PORT GPIOE
#define CS2_Pin GPIO_PIN_1
#define CS2_GPIO_PORT GPIOE
#define CS1_Pin GPIO_PIN_2
#define CS1_GPIO_PORT GPIOE

#define EN_Pin GPIO_PIN_4
#define EN_GPIO_PORT GPIOE
#define RW_Pin GPIO_PIN_5
#define RW_GPIO_PORT GPIOE
#define RS_Pin GPIO_PIN_6
#define RS_GPIO_PORT GPIOE

#define DATA0_Pin GPIO_PIN_0
#define DATA0_GPIO_PORT GPIOD
#define DATA1_Pin GPIO_PIN_1
#define DATA1_GPIO_PORT GPIOD
#define DATA2_Pin GPIO_PIN_2
#define DATA2_GPIO_PORT GPIOD
#define DATA3_Pin GPIO_PIN_3
#define DATA3_GPIO_PORT GPIOD
#define DATA4_Pin GPIO_PIN_4
#define DATA4_GPIO_PORT GPIOD
#define DATA5_Pin GPIO_PIN_5
#define DATA5_GPIO_PORT GPIOD
#define DATA6_Pin GPIO_PIN_6
#define DATA6_GPIO_PORT GPIOD
#define DATA7_Pin GPIO_PIN_7
#define DATA7_GPIO_PORT GPIOD

#define DISP_DEFAULT_FONT       font16x8
#define SET_COLUMN_ADDR         ( 0x15 )
#define START_COLUMN            ( 0x18 )
#define END_COLUMN              ( 0x53 )
#define SET_ROW_ADDR            ( 0x75 )
#define START_ROW               ( 0x00 )
#define END_ROW                 ( 0x7F )
#define WRITE_RAM_COM           ( 0x5C )

/**
 * @brief Define the LED behavior types.
 */


/*=============================================================
**Public function prototypes
**===========================================================*/
/*LED configration.*/
extern void LcdConfig( void );
extern void CleanScreen(void);
extern void LcdDisplay(void);
extern void DispOneLine( uint8_t line, uint8_t *str );
extern void CleanOneLine( uint8_t line );
extern void DisplayChar( uint8_t line, uint8_t charCol, uint8_t ch );
extern void CleanChar( uint8_t line, uint8_t charCol );
extern void DelayMs( uint8_t cnt );

#ifdef __cplusplus
}/*extern C*/
#endif

#endif /* __DISPLAY_H */
/* ================================ End Of File ====================================*/

