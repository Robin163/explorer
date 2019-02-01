/**
 * ============================================================================
 * @file    screen.h
 * @author  Robin li, Robin.li@defatechnology.com
 * @version V1.0
 * @date    2018年08月07日 09时02分33秒
 * @brief   key to screen
 *
 *
 * ============================================================================
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY CO.,LTD.</center></h2>
 * ============================================================================
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __SCREEN_H
#define  __SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint-gcc.h>

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum
{
    MenuID_Main,
    MenuID_Charge,
    MenuID_Supply,
    MenuID_Settings,
    MenuID_ChargeOpetion,
    MenuID_BatterySize,
    MenuID_Voltage,
    MenuID_Current,
    MenuID_Workshop,
    MenuID_Start,
    MenuID_Display,
    MenuID_Language,
    MenuID_Buzzer,
    MenuID_FactoryReset,
    MenuID_None = 255
} MenuID;

typedef enum
{
    ItemID_Default = 0,
    ItemID_Max = 4,
    ItemID_None = 255
} ItemID;

typedef struct
{
    uint8_t max;
    MenuID id[ItemID_Max];
} MenuItem;

typedef struct
{
    MenuID curMenu;
    uint8_t curItem;
    MenuID escIndex;
    MenuItem *enterIndex;
    void ( *operate )();
} MenuState;

/* Exported constants --------------------------------------------------------*/
/* timebase define */
#define SCREEN_DELAY           ( (uint16_t)400 )
#define SCREEN_PERIOD          ( (uint16_t)1 )
#define SCREEN_TIMER_ZERO      ( 0 )
#define SCREEN_TIMER_BASE      ( SCREEN_PERIOD*SYSTEM_TIMER_TICK )

/* 0 step Menu */
#define MAIN_MENU_CHARGE        0
#define MAIN_MENU_SUPPLY        1
#define MAIN_MENU_SET           2

/* 1 step Menu */
#define CHARGE_MENU_OPETION     10
#define CHARGE_MENU_SIZE        11

#define SUPPLY_MENU_VOLT        20
#define SUPPLY_MENU_CURRENT     21
#define SUPPLY_MENU_WORKSHOP    22
#define SUPPLY_MENU_START       23

#define SET_MENU_DISPLAY        30
#define SET_MENU_LANGUAGE       31
#define SET_MENU_BUZZER         32
#define SET_MENU_RESET          33

/* 2 step Menu */
#define VOLT_MENU               40
#define CURRENT_MENU            50


/* Exported macro ------------------------------------------------------------*/
/* Extern  variables ---------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/* key handle. */
extern void StartMenuTask( void );
extern void WelcomeMenu( void );
extern void MainMenu( void );



#ifdef __cplusplus
}/*extern C*/
#endif

#endif  /* __SCREEN_H */
/* ============================= End Of File ================================ */

