/**
 * =============================================================================
 * @file    screen.c
 * @author  Robin li, Robin.li@defatechnology.com
 * @version V1.0
 * @date    2018年08月07日 08时28分44秒
 * @brief   key signal change to screen message
 *
 *
 * =============================================================================
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY CO.,LTD.</center></h2>
 * =============================================================================
 */
/* Includes ------------------------------------------------------------------*/
#include <stdint-gcc.h>
#include "encode.h"
#include "fonts.h"
#include "lcd.h"
#include "menu.h"

/** @addtogroup Template_Project
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Extern  variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void KeyDownHandle( MenuID *menuIndex, uint8_t *itemIndex );
static void KeyUpHandle( MenuID *menuIndex, uint8_t *itemIndex );
static void KeyEntryHandle( MenuID *menuIndex, uint8_t *itemIndex );
static void KeyEscHandle( MenuID *menuIndex, uint8_t *itemIndex );
static void ChargeMenu( void );
static void SupplyMenu( void );
static void SetMenu( void );
static void BuzzerMenu( void );

/* Private variables ---------------------------------------------------------*/
MenuItem mainEnter = { 3, {MenuID_Charge, MenuID_Supply, MenuID_Settings, MenuID_None} };
MenuItem chargeEnter = { 2, {MenuID_ChargeOpetion, MenuID_BatterySize, MenuID_None, MenuID_None} };
MenuItem supplyEnter = { 3, {MenuID_Voltage, MenuID_Current, MenuID_Workshop, MenuID_Start} };
MenuItem setEnter = { 3, {MenuID_Display, MenuID_Language, MenuID_Buzzer, MenuID_FactoryReset} };
MenuItem noEnter = { 255, {MenuID_None,  MenuID_None, MenuID_None, MenuID_None} };

static MenuState g_menuState[] =
{
	/*   menu                   item           esc             enter       operate */
	{ MenuID_Main,          ItemID_Default, MenuID_None,     &mainEnter,   MainMenu },
	{ MenuID_Charge,        ItemID_Default, MenuID_Main,     &chargeEnter, ChargeMenu },
	{ MenuID_Supply,        ItemID_Default, MenuID_Main,     &supplyEnter, SupplyMenu },
	{ MenuID_Settings,      ItemID_Default, MenuID_Main,     &setEnter,    SetMenu },
    { MenuID_ChargeOpetion, ItemID_Default, MenuID_Charge,   &noEnter,     BuzzerMenu },
    { MenuID_BatterySize,   ItemID_Default, MenuID_Charge,   &noEnter,     BuzzerMenu },
    { MenuID_Voltage,       ItemID_Default, MenuID_Supply,   &noEnter,     BuzzerMenu },
    { MenuID_Current,       ItemID_Default, MenuID_Supply,   &noEnter,     BuzzerMenu },
    { MenuID_Workshop,      ItemID_Default, MenuID_Supply,   &noEnter,     BuzzerMenu },
    { MenuID_Start,         ItemID_Default, MenuID_Supply,   &noEnter,     BuzzerMenu },
    { MenuID_Display,       ItemID_Default, MenuID_Settings, &noEnter,     BuzzerMenu },
    { MenuID_Language,      ItemID_Default, MenuID_Settings, &noEnter,     BuzzerMenu },
    { MenuID_Buzzer,        ItemID_Default, MenuID_Settings, &noEnter,     BuzzerMenu },
    { MenuID_FactoryReset,  ItemID_Default, MenuID_Settings, &noEnter,     BuzzerMenu },
};

/**
 * @name   KeyHandle
 * @param  none.
 * @retval none.
 * @brief  key handle
 *
 */
void
StartMenuTask( void )
{
	static MenuID menuIndex = 0;
	static uint8_t itemIndex = 0;

	KeyCode_E keyCode = Key_None;
	keyCode = GetKeyCodeFromBuf();
	if(keyCode != Key_None)
	{
		switch( keyCode )
		{
			case Key1_Pressed:
				KeyEntryHandle( &menuIndex, &itemIndex );
				break;
			case Key1_Double:
				KeyEscHandle( &menuIndex, &itemIndex );
				break;
			case Encode_CW:
				KeyDownHandle( &menuIndex, &itemIndex );
				break;
			case Encode_CCW:
				KeyUpHandle( &menuIndex, &itemIndex );
				break;
			default:
				break;
		}
	}
}

/**
 * @name   MainMenu
 * @param  none.
 * @retval none.
 * @brief  display main menu
 *
 */
static void
KeyDownHandle( MenuID *menuIndex, uint8_t *itemIndex )
{
	MenuState *menu = &g_menuState[*menuIndex];

	if( menu->enterIndex->max < ItemID_None )
	{
		CleanChar( *itemIndex + 1, 14 );
		CleanChar( *itemIndex + 1, 15 );

		if( *itemIndex < menu->enterIndex->max - 1 )
		{
			( *itemIndex )++;
		}
		else
		{
//			*itemIndex = 0;
		}

		menu->curItem = *itemIndex;
		DisplayChar( *itemIndex + 1, 14, '<' );
		DisplayChar( *itemIndex + 1, 15, '-' );
	}
}

/**
 * @name   MainMenu
 * @param  none.
 * @retval none.
 * @brief  display main menu
 *
 */
static void
KeyUpHandle( MenuID *menuIndex, uint8_t *itemIndex )
{
	MenuState *menu = &g_menuState[*menuIndex];

	if( menu->enterIndex->max < ItemID_None )
	{
		CleanChar( *itemIndex + 1, 14 );
		CleanChar( *itemIndex + 1, 15 );

		if( *itemIndex > 0 )
		{
			( *itemIndex )--;
		}
		else
		{
//			*itemIndex = menu->enterIndex->max - 1;
		}

		menu->curItem = *itemIndex;
		DisplayChar( *itemIndex + 1, 14, '<' );
		DisplayChar( *itemIndex + 1, 15, '-' );
	}
}

/**
 * @name   MainMenu
 * @param  none.
 * @retval none.
 * @brief  display main menu
 *
 */
static void
KeyEntryHandle( MenuID *menuIndex, uint8_t *itemIndex )
{
	MenuState *menu = &g_menuState[*menuIndex];

	if( menu->enterIndex->max < ItemID_None )
	{
		*menuIndex = menu->enterIndex->id[*itemIndex];
		menu = &g_menuState[*menuIndex];
		menu->operate();
	}

	if( menu->enterIndex->max < ItemID_None )
	{
		*itemIndex = ItemID_Default;
		menu->curItem = ItemID_Default;
		DisplayChar( *itemIndex + 1, 14, '<' );
		DisplayChar( *itemIndex + 1, 15, '-' );
	}
}
/**
 * @name   MainMenu
 * @param  none.
 * @retval none.
 * @brief  display main menu
 *
 */
static void
KeyEscHandle( MenuID *menuIndex, uint8_t *itemIndex )
{
	MenuState *menu = &g_menuState[*menuIndex];

	if( menu->escIndex < MenuID_None )
	{
		*menuIndex = menu->escIndex;
		menu = &g_menuState[*menuIndex];
		menu->operate();
		*itemIndex = menu->curItem;
		DisplayChar( *itemIndex + 1, 14, '<' );
		DisplayChar( *itemIndex + 1, 15, '-' );
	}
}
/**
 * @name   MainMenu
 * @param  none.
 * @retval none.
 * @brief  display main menu
 *
 */
void
WelcomeMenu( void )
{
	uint8_t line1Letter[] = "Welcome to DEFA ";
	uint8_t line2Letter[] = "Charge wch125   ";
	uint8_t line3Letter[] = "                ";
	uint8_t line4Letter[] = "    --2018.08.07";
	DispOneLine( 0, line1Letter );
	DispOneLine( 1, line2Letter );
	DispOneLine( 2, line3Letter );
	DispOneLine( 3, line4Letter );
}
/**
 * @name   MainMenu
 * @param  none.
 * @retval none.
 * @brief  display main menu
 *
 */
void
MainMenu( void )
{
	uint8_t line1Letter[] = "   Main Menu    ";
	uint8_t line2Letter[] = "Charger         ";
	uint8_t line3Letter[] = "Power Supply    ";
	uint8_t line4Letter[] = "Settings        ";
	DispOneLine( 0, line1Letter );
	DispOneLine( 1, line2Letter );
	DispOneLine( 2, line3Letter );
	DispOneLine( 3, line4Letter );
}
/**
 * @name   SupplySelect
 * @param  none.
 * @retval none.
 * @brief
 *
 */
static void
ChargeMenu( void )
{
	uint8_t line1Letter[] = "  Charge Menu   ";
	uint8_t line2Letter[] = "Normale         ";
	uint8_t line3Letter[] = "AGM             ";
	uint8_t line4Letter[] = "                ";
	DispOneLine( 0, line1Letter );
	DispOneLine( 1, line2Letter );
	DispOneLine( 2, line3Letter );
	DispOneLine( 3, line4Letter );
}

/**
 * @name   SupplySelect
 * @param  none.
 * @retval none.
 * @brief
 *
 */
static void
SupplyMenu( void )
{
	uint8_t line1Letter[] = "  Supply Menu   ";
	uint8_t line2Letter[] = "13.6V           ";
	uint8_t line3Letter[] = "60A             ";
	uint8_t line4Letter[] = "125A            ";
	DispOneLine( 0, line1Letter );
	DispOneLine( 1, line2Letter );
	DispOneLine( 2, line3Letter );
	DispOneLine( 3, line4Letter );
}

/**
 * @name   SupplySelect
 * @param  none.
 * @retval none.
 * @brief
 *
 */
static void
SetMenu( void )
{
	uint8_t line1Letter[] = "    Settings    ";
	uint8_t line2Letter[] = "Display         ";
	uint8_t line3Letter[] = "Language        ";
	uint8_t line4Letter[] = "Factory Reset   ";
	DispOneLine( 0, line1Letter );
	DispOneLine( 1, line2Letter );
	DispOneLine( 2, line3Letter );
	DispOneLine( 3, line4Letter );
}

static void
BuzzerMenu( void )
{
	uint8_t line1Letter[] = "    Settings    ";
    uint8_t line2Letter[] = "Warnning Buzzer ";
    uint8_t line3Letter[] = "      60A       ";
    uint8_t line4Letter[] = "                ";
    DispOneLine( 0, line1Letter );
    DispOneLine( 1, line2Letter );
    DispOneLine( 2, line3Letter );
    DispOneLine( 3, line4Letter );
}
/**
 *@}
 */
/* ============================= End Of File ================================ */

