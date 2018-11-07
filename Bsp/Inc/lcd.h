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
#include "spi_lcd.h"
#include "fonts.h"

/*=============================================================
**Constant vector define
**===========================================================*/
/* timebase define */
#define DISPLAY_DELAY           ( (uint16_t)43 )
#define DISPLAY_PERIOD          ( (uint16_t)1 )
#define DISPLAY_TIMER_ZERO      ( 00 )
#define DISPLAY_TIMER_BASE      ( DISPLAY_PERIOD*SYSTEM_TIMER_TICK )

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
extern void DisplayConfig( void );
extern Font *GetNowFont( void );
extern void SetNowFont( Font *font );
extern void DisplayToggle( void );
extern void DispOneLine( uint8_t line, uint8_t *str );
extern void CleanOneLine( uint8_t line );
extern void DispOneChar2( uint8_t line, uint8_t column, uint8_t ch );
extern void DispOneChar( uint8_t xPos, uint8_t yPos, uint8_t ch );
extern void CleanOneChar2( uint8_t line, uint8_t column );
extern void CleanOneChar( uint8_t xPos, uint8_t yPos );
extern void CleanScreen( void );

#ifdef __cplusplus
}/*extern C*/
#endif

#endif /* __DISPLAY_H */
/* ================================ End Of File ====================================*/

