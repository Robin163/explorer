/**
 ***********************************************************************************
 * @weakgroup bsp_driver
 * @{
 * @brief Device drivers module.
 * @file  oled.c
 * @brief OLED driver.
 * @author Robin Li
 * @version V1.0.0
 * @date 09/05/2018
 ************************************************************************************
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY (WUXI) CO.,LTD.</center></h2>
 ************************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint-gcc.h>

#include "stm32f1xx_hal.h"
#include "spi_lcd.h"
#include "fonts.h"
#include "lcd.h"

uint8_t defa[] = "  DEFA Tech";
uint8_t wch125[] = "   wch125";

/* Private variables ---------------------------------------------------------*/
#define LINE(x) ( (x) * (((Font *)GetNowFont())->height) )
#define CLMN(y) ( (y) * (((Font *)GetNowFont())->width)/4 + 35 )
Font *g_nowFont = &DISP_DEFAULT_FONT;

/* Private function prototypes -----------------------------------------------*/
static void DelayUs( uint8_t cnt );
static void WriteByte( uint8_t byte );


/** @addtogroup Template_Project
  * @{
  */

/**
 * @brief   OLED configration.
 * @param   None.
 * @retval  None.
 * @sa
 *
 */
void DisplayConfig( void )
{
    TurnOnLcdReset();
    DelayUs( 200 );
    TurnOffLcdReset();
    DelayUs( 100 );
    WriteCommand( 0xFD );                       //Command lock setting
    WriteData( 0x12 );
    WriteCommand( 0xAE );                       //Display OFF
    WriteCommand( 0x15 );                       //Column address setting
    WriteData( 0x18 );
    WriteData( 0x53 );
    WriteCommand( 0x75 );                       //row address setting
    WriteData( 0x00 );
    WriteData( 0x7F );
    WriteCommand( 0xA0 );                       //re_map&dual com mode
    WriteData( 0x36 );
    WriteData( 0x01 );                       //Disable dual com mode
    WriteCommand( 0xA1 );                       //display start line
    WriteData( 0x00 );
    WriteCommand( 0xA2 );                       //display offset
    WriteData( 0x00 );
    WriteCommand( 0xA6 );                       //normal display
    WriteCommand( 0xA8 );                       //partial display setting
    WriteData( 0x00 );                       //start row
    WriteData( 0x7F );                       //end row
    //WriteCommand(0xA9);			//exit partial display mode
    WriteCommand( 0xAB );                       //function selection
    WriteData( 0x01 );                       //enable internal VDD
    WriteCommand( 0xB1 );                       //phase length setting
    WriteData( 0xD2 );                       //
    WriteCommand( 0xB3 );                       //front clock divider&oscillator freq
    WriteData( 0xE1 );
    WriteCommand( 0xB4 );                       //display enhancement A
    WriteData( 0xA0 );                       //external VSL
    WriteData( 0xFD );                       //normal or 11111101 to enhance low GS
    WriteCommand( 0xB6 );                       //second precharge period setting
    WriteData( 0x08 );
    WriteCommand( 0xBB );                       //set precharge voltage
    WriteData( 0x1F );                          //0x22
    WriteCommand( 0xBE );                       //set VCOMH voltage
    WriteData( 0x06 );                       //0.80*VCC
    WriteCommand( 0xB5 );                       //GPIO SETTING
    WriteData( 0x0A );
    WriteCommand( 0xB8 );                       //Gray scale setting
    WriteData( 0x00 );                          //GS1=0
    WriteData( 0x08 );                          //GS2=12
    WriteData( 0x10 );                          //GS3=24
    WriteData( 0x18 );                          //GS4=36
    WriteData( 0x20 );                          //GS5=48
    WriteData( 0x28 );                          //GS6=60
    WriteData( 0x30 );                          //GS7=72
    WriteData( 0x38 );                          //GS8=84
    WriteData( 0x40 );                          //GS9=96
    WriteData( 0x48 );                          //GS10=108
    WriteData( 0x50 );                          //GS11=120
    WriteData( 0x58 );                          //GS12=132
    WriteData( 0x60 );                          //GS13=144
    WriteData( 0x68 );                          //GS14
    WriteData( 0x70 );                          //GS15
    WriteCommand( 0x00 );                       //Enable gray scale setting
    //WriteCommand(0xB9);//gray scale setting
    WriteCommand( 0xC1 );                       //contrast set
    WriteData( 0xDF );
    WriteCommand( 0xC7 );                       //master current set
    WriteData( 0x0F );
    WriteCommand( 0xCA );                       //mux set
    WriteData( 0x7F );
    WriteCommand( 0xD1 );                       //display enhancement B
    WriteData( 0xA2 );
    WriteData( 0x20 );
    WriteCommand( 0xAF );                       //Display ON
}

static void DelayUs( uint8_t cnt )
{
    for( ; cnt != 0; cnt-- );
}

Font *GetNowFont( void )
{
    return g_nowFont;
}
void SetNowFont( Font *font )
{
    g_nowFont = font;
}



void DisplayToggle( void )
{
    static uint16_t cnt = 0;
    cnt++;

    if( cnt == 1 )
    {
        SetNowFont( &font32x16 );
        //        DispOneChar2( 12, 1, 'A');
        //        DispOneChar2( 13, 2, 'B');
        //        DispOneChar2( 14, 3, 'C');
        //        DispOneChar2( 15, 4, 'D');
        DispOneLine( 1, defa );
        DispOneLine( 2, wch125 );
    }
    else if( cnt == 500 )
    {
        //        CleanOneChar2( 12,1 );
        //        CleanOneChar2( 13,2 );
        //        CleanOneChar( 88,00 );
        //        CleanOneChar( 92,00 );
        CleanOneLine( 1 );
        CleanOneLine( 2 );
    }
    else if( cnt == 501 )
    {
        SetNowFont( &font16x8 );
        //        DispOneChar2( 1, 1, 'A' );
        //        DispOneChar2( 2, 1, 'B' );
        //        DispOneChar2( 3, 1, 'C' );
        //        DispOneChar2( 4, 1, 'D' );
        DispOneLine( 5, defa );
    }
    else if( cnt == 1000 )
    {
        cnt = 0;
        //        CleanOneChar2( 1,1 );
        //        CleanOneChar2( 2,1 );
        //        CleanOneChar( 40,00 );
        //        CleanOneChar( 42,00 );
        CleanOneLine( 5 );
    }
    else
    {
    }
}


/**
 * @name   DispOneLine
 * @param  line.
 * @param  string.
 * @retval none.
 * @brief  display one line
 *         no change line when one line over.
 */
void
DispOneLine( uint8_t line, uint8_t *str )
{
    uint16_t refcolumn = 1;
    uint8_t  refWidth = g_nowFont->width / 4;

    /* Send the string character by character on lCD */
    while( ( ( START_COLUMN + refcolumn * refWidth - 1 ) <= END_COLUMN )
        && ( *str != '\0' ) )
    {
        /* Display one character on LCD */
        DispOneChar2( refcolumn, line, *str );
        /* Decrement the column position by 16 */
        refcolumn++;
        /* Point on the next character */
        str++;
    }
}


void DispOneChar2( uint8_t charCol, uint8_t line, uint8_t ch )
{
    WriteCommand( SET_COLUMN_ADDR );    /*SET Column address*/
    WriteData( ( charCol - 1 ) * ( g_nowFont->width / 4 ) + START_COLUMN );
    WriteData( charCol * ( g_nowFont->width / 4 ) +  START_COLUMN - 1 );
    WriteCommand( SET_ROW_ADDR );		/*SET ROW address*/
    WriteData( ( line - 1 )*g_nowFont->height );
    WriteData( line * g_nowFont->height - 1 );
    WriteCommand( WRITE_RAM_COM );
    uint8_t chIndex = ch - 32;
    uint8_t dataLegth = g_nowFont->height * g_nowFont->width / 8;

    for( uint8_t data = 0; data < dataLegth; data++ )
    {
        WriteByte( g_nowFont->tableIndex[chIndex * dataLegth + data] );
    }
}

void DispOneChar( uint8_t xPos, uint8_t yPos, uint8_t ch )
{
    WriteCommand( SET_COLUMN_ADDR );    /*SET Column address*/
    WriteData( xPos );
    WriteData( xPos - 1 + g_nowFont->width / 4 );
    WriteCommand( SET_ROW_ADDR );		/*SET ROW address*/
    WriteData( yPos );
    WriteData( yPos + g_nowFont->height );
    WriteCommand( WRITE_RAM_COM );
    uint8_t chIndex = ch - 32;
    uint8_t dataLegth = g_nowFont->height * g_nowFont->width / 8;

    for( uint8_t data = 0; data < dataLegth; data++ )
    {
        WriteByte( g_nowFont->tableIndex[chIndex * dataLegth + data] );
    }
}

void CleanOneLine( uint8_t line )
{
    uint16_t refcolumn = 1;
    uint8_t  refWidth = g_nowFont->width / 4;

    /* Send the string character by character on lCD */
    while( ( START_COLUMN + refcolumn * refWidth - 1 ) <= END_COLUMN )
    {
        /* Display one character on LCD */
        CleanOneChar2( refcolumn, line );
        /* Decrement the column position by 16 */
        refcolumn++;
    }
}
void CleanOneChar2( uint8_t charCol, uint8_t line )
{
    WriteCommand( SET_COLUMN_ADDR );    /*SET Column address*/
    WriteData( ( charCol - 1 ) * ( g_nowFont->width / 4 ) + START_COLUMN );
    WriteData( charCol * ( g_nowFont->width / 4 ) + START_COLUMN - 1 );
    WriteCommand( SET_ROW_ADDR );		/*SET ROW address*/
    WriteData( ( line - 1 )*g_nowFont->height );
    WriteData( line * g_nowFont->height - 1 );
    WriteCommand( WRITE_RAM_COM );
    uint8_t dataLegth = g_nowFont->height * g_nowFont->width / 8;

    for( uint8_t data = 0; data < dataLegth; data++ )
    {
        WriteByte( 0x00 );
    }
}

void CleanOneChar( uint8_t xPos, uint8_t yPos )
{
    WriteCommand( SET_COLUMN_ADDR );    /*SET Column address*/
    WriteData( xPos );
    WriteData( xPos - 1 + g_nowFont->width / 4 );
    WriteCommand( SET_ROW_ADDR );		/*SET ROW address*/
    WriteData( yPos );
    WriteData( yPos + g_nowFont->height );
    WriteCommand( WRITE_RAM_COM );
    uint8_t dataLegth = g_nowFont->height * g_nowFont->width / 8;

    for( uint8_t data = 0; data < dataLegth; data++ )
    {
        WriteByte( 0x00 );
    }
}

void CleanScreen( void )
{
    WriteCommand( SET_COLUMN_ADDR );    /*SET Column address*/
    WriteData( START_COLUMN );
    WriteData( END_COLUMN );
    WriteCommand( SET_ROW_ADDR );		/*SET ROW address*/
    WriteData( START_ROW );
    WriteData( END_ROW );
    WriteCommand( WRITE_RAM_COM );

    for( uint8_t row = 0; row < 128; row++ )
    {
        for( uint8_t column = 0; column < 120; column++ )
        {
            WriteData( 0x00 );
        }
    }
}

static void WriteByte( uint8_t byte )
{
    uint8_t byteTemp = byte;
    uint8_t dataTemp = 0x00;

    for( uint8_t loopCnt = 0; loopCnt < 4; loopCnt++ )
    {
        if( ( byteTemp & 0x80 ) == 0x80 )
        {
            dataTemp = 0xF0;
        }
        else
        {
            dataTemp = 0x00;
        }

        byteTemp <<= 1;

        if( ( byteTemp & 0x80 ) == 0x80 )
        {
            dataTemp |= 0x0F;
        }
        else
        {
            dataTemp &= 0xF0;
        }

        byteTemp <<= 1;
        WriteData( dataTemp );
    }
}

/** @} */

/* ================================ End Of File ====================================*/

