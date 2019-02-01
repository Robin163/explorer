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
#include "main.h"
#include "lcd.h"
#include "fonts.h"

/* Private variables ---------------------------------------------------------*/
Font *g_nowFont = &font16x8;

/* Private function prototypes -----------------------------------------------*/
static void LcdConfigGPIO( void );
static void LcdConfigDisplay( void );

void DisplayGraph(uint8_t dot1,uint8_t dot2);
static void WriteCmd( uint8_t cmd );
static void WriteData( uint8_t dat );
static void DelayUs( uint8_t cnt );



/**
 * @brief   OLED configration.
 * @param   None.
 * @retval  None.
 * @sa
 *
 */
void LcdConfig( void )
{
	LcdConfigGPIO();
	LcdConfigDisplay();
}
/**
 * @brief   OLED configration.
 * @param   None.
 * @retval  None.
 * @sa
 *
 */
static void LcdConfigGPIO( void )
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE, CS1_Pin|RST_Pin|CS2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, EN_Pin|RW_Pin|RS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, DATA0_Pin|DATA1_Pin|DATA2_Pin|DATA3_Pin
				|DATA4_Pin|DATA5_Pin|DATA6_Pin|DATA7_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : CS1_Pin EN_Pin WR_Pin RS_Pin RST_Pin CS2_Pin */
	GPIO_InitStruct.Pin = CS1_Pin|RST_Pin|CS2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = EN_Pin|RW_Pin|RS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : DATA0_Pin DATA1_Pin DATA2_Pin DATA3_Pin
	  DATA4_Pin DATA5_Pin DATA6_Pin DATA7_Pin */
	GPIO_InitStruct.Pin = DATA0_Pin|DATA1_Pin|DATA2_Pin|DATA3_Pin
		|DATA4_Pin|DATA5_Pin|DATA6_Pin|DATA7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
/**
 * @brief   OLED configration.
 * @param   None.
 * @retval  None.
 * @sa
 *
 */
static void LcdConfigDisplay( void )
{
	HAL_GPIO_WritePin(RST_GPIO_PORT, RST_Pin, GPIO_PIN_RESET);
	DelayMs(5);
	HAL_GPIO_WritePin(RST_GPIO_PORT, RST_Pin, GPIO_PIN_SET);
	DelayMs(5);

	HAL_GPIO_WritePin(CS1_GPIO_PORT, CS1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS2_GPIO_PORT, CS2_Pin, GPIO_PIN_SET);
	DelayUs(2);
	WriteCmd(0x0C0);
	DelayUs(220);
	WriteCmd(0x3F);
	DelayUs(2);

}

void CleanScreen(void)
{
	HAL_GPIO_WritePin(CS1_GPIO_PORT, CS1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS2_GPIO_PORT, CS2_Pin, GPIO_PIN_SET);
	WriteCmd(0xb8+0);
	WriteCmd(0x40+0);

	for(uint8_t k=0;k<8;k++)
	{
		WriteCmd(0xb8+k);
		for(uint8_t j=0;j<64;j++)
		{
			WriteData(0x00);
		}
	}
}

/** @addtogroup Template_Project
 * @{
 */

void LcdDisplay(void)
{
	uint8_t aa1[]="   DEFA 2019";
	uint8_t aa2[]="     WCH125";
	uint8_t aa3[]="welcome to china";
	uint8_t aa4[]="         --robin";

	static uint8_t lcdStatus = 0;
	if(lcdStatus == 0)
	{
		lcdStatus = 1;
		DispOneLine(0, aa1);
		DispOneLine(1, aa2);
	}
	else
	{
		lcdStatus = 0;
		DispOneLine(2, aa3);
		DispOneLine(3, aa4);
	}
}

Font *GetNowFont( void )
{
    return g_nowFont;
}
void SetNowFont( Font *font )
{
    g_nowFont = font;
}

/**
 * @name   DispOneLine
 * @param  line.
 * @param  string.
 * @retval none.
 * @brief  display one line
 *         no change line when one line over.
 */
void DispOneLine( uint8_t line, uint8_t *str )
{
    uint16_t refcolumn = 0;
    uint8_t  refWidth = g_nowFont->width;

    /* Send the string character by character on lCD */
    while( ( ((refcolumn+1) * refWidth ) <= 128 )
        && ( *str != '\0' ) )
    {
        /* Display one character on LCD */
        DisplayChar( line, refcolumn, *str );
        /* Decrement the column position by 16 */
        refcolumn++;
        /* Point on the next character */
        str++;
    }
}
void CleanOneLine( uint8_t line )
{
    uint16_t refcolumn = 0;
    uint8_t  refWidth = g_nowFont->width;

    /* Send the string character by character on lCD */
    while( ((refcolumn+1) * refWidth ) <= 15 )
    {
        /* Display one character on LCD */
        CleanChar( line, refcolumn );
        /* Decrement the column position by 16 */
        refcolumn++;
    }
}
void DisplayChar( uint8_t line, uint8_t charCol, uint8_t ch )
{
	uint8_t refWidth = g_nowFont->width;
	uint8_t refScreen = charCol*refWidth;
	if(( charCol*refWidth ) < 64)
	{
		HAL_GPIO_WritePin(CS1_GPIO_PORT, CS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CS2_GPIO_PORT, CS2_Pin, GPIO_PIN_RESET);
		refScreen = charCol*refWidth;
	}
	else
	{
		HAL_GPIO_WritePin(CS1_GPIO_PORT, CS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CS2_GPIO_PORT, CS2_Pin, GPIO_PIN_SET);
		refScreen = charCol*refWidth - 64;
	}
	DelayUs(2);
	uint8_t refCharIndex = ch - 32;
	uint8_t lineOfOneChar  = g_nowFont->height >>3;
	for(uint8_t i=0;i<lineOfOneChar;i++)
	{
		WriteCmd(0x0b8+line*lineOfOneChar+i);  /* set line number */
		WriteCmd(0x40+refScreen); /* set column number */

		for(uint8_t column=0;column<refWidth;column++)	//32
		{
			uint16_t charPosition = refCharIndex*refWidth*lineOfOneChar;
			WriteData(g_nowFont->tableIndex[charPosition+refWidth*i+column]);
		}
	}
}

void CleanChar( uint8_t line, uint8_t charCol )
{
	uint8_t refWidth = g_nowFont->width;
	uint8_t refScreen = charCol*refWidth;
	if(( charCol*refWidth ) < 64)
	{
		HAL_GPIO_WritePin(CS1_GPIO_PORT, CS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CS2_GPIO_PORT, CS2_Pin, GPIO_PIN_RESET);
		refScreen = charCol*refWidth;
	}
	else
	{
		HAL_GPIO_WritePin(CS1_GPIO_PORT, CS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CS2_GPIO_PORT, CS2_Pin, GPIO_PIN_SET);
		refScreen = charCol*refWidth - 64;
	}
	DelayUs(2);
	uint8_t lineOfOneChar  = g_nowFont->height >>3;
	for(uint8_t i=0;i<lineOfOneChar;i++)
	{
		WriteCmd(0x0b8+line*lineOfOneChar+i);  /* set line number */
		WriteCmd(0x40+refScreen); /* set column number */

		for(uint8_t column=0;column<refWidth;column++)	//32
		{
			WriteData(0x00);
		}
	}
}
void DisplayGraph(uint8_t dot1,uint8_t dot2)
{
	uint8_t cnt;
	register uint8_t cnt1;

	HAL_GPIO_WritePin(CS1_GPIO_PORT, CS1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CS2_GPIO_PORT, CS2_Pin, GPIO_PIN_SET);
	DelayUs(2);
	for(cnt=0;cnt<8;cnt++)
	{
		WriteCmd(0x0b8+cnt);  //Set Page Address 0~7
		WriteCmd(0x40);        //Set Y address
		for(cnt1=0;cnt1<32;cnt1++)	//32
		{
			WriteData(dot1);
			WriteData(dot2);
		}
	}
	HAL_GPIO_WritePin(CS1_GPIO_PORT, CS1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS2_GPIO_PORT, CS2_Pin, GPIO_PIN_RESET);
	DelayUs(2);
	for(cnt=0;cnt<8;cnt++)
	{
		WriteCmd(0x0b8+cnt);  //Set Page Address 0~7
		WriteCmd(0x40);        //Set Y address
		for(cnt1=0;cnt1<32;cnt1++)	//32
		{
			WriteData(dot1);
			WriteData(dot2);
		}
	}
}

//write command
static void WriteCmd(uint8_t cmd)
{
	HAL_GPIO_WritePin(EN_GPIO_PORT, EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RS_GPIO_PORT, RS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RW_GPIO_PORT, RW_Pin, GPIO_PIN_RESET);

	GPIOD->ODR = 0x00FF&cmd;
	DelayUs(2);
	HAL_GPIO_WritePin(EN_GPIO_PORT, EN_Pin, GPIO_PIN_SET);
	DelayUs(2);
	HAL_GPIO_WritePin(EN_GPIO_PORT, EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RW_GPIO_PORT, RW_Pin, GPIO_PIN_SET);
	DelayUs(50);
}

//Write Display RAM Data
static void WriteData(uint8_t dat)
{
	HAL_GPIO_WritePin(EN_GPIO_PORT, EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RS_GPIO_PORT, RS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RW_GPIO_PORT, RW_Pin, GPIO_PIN_RESET);

	GPIOD->ODR = 0x00FF&dat;
	DelayUs(2);
	HAL_GPIO_WritePin(EN_GPIO_PORT, EN_Pin, GPIO_PIN_SET);
	DelayUs(2);
	HAL_GPIO_WritePin(EN_GPIO_PORT, EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RW_GPIO_PORT, RW_Pin, GPIO_PIN_SET);
	DelayUs(50);
}

static void DelayUs( volatile uint8_t cnt )
{
	for( ; cnt != 0; cnt-- );
}

void DelayMs( volatile uint8_t cnt )
{
	volatile uint16_t uc_dly,uc_dly1,uc_dly2;

	uc_dly =cnt;

	while (uc_dly --)
	{
		for (uc_dly1=0;uc_dly1<200;uc_dly1++)
		  for (uc_dly2=0;uc_dly2<200;uc_dly2++);
	};
}

/** @} */

/* ================================ End Of File ====================================*/

