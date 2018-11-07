/**
 ***********************************************************************************
 * @file  spi_oled.h
 * @brief Definitions for oled driver.
 * @author Robin Li
 * @version V1.0.0
 * @date 09/05/2018
 ************************************************************************************
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY (WUXI) CO.,LTD.</center></h2>
 ************************************************************************************
 */

#ifndef __SPI_LCD_H
#define __SPI_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint-gcc.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
/*=============================================================
**Constant vector define
**===========================================================*/
#define GPIO_INIT_SPI_SG { PIN_UART_TX, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define GPIO_INIT_SPI_CTRL { PIN_UART_RX, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING }
#define OLED_INIT_SPI_PARA { SPI_Direction_2Lines_FullDuplex, SPI_Mode_Master, \
        SPI_DataSize_8b, SPI_CPOL_High, SPI_CPHA_2Edge, SPI_NSS_Soft, SPI_BaudRatePrescaler_128, \
        SPI_FirstBit_MSB, 7 }

/*=============================================================
**Public function prototypes
**===========================================================*/
/*SPI configration.*/
extern void WriteCommand( uint8_t cmd );
extern void WriteData( uint8_t dat );
extern void TurnOnLcdReset( void );
extern void TurnOffLcdReset( void );

#ifdef __cplusplus
}/*extern C*/
#endif

#endif /* __SPI_LCD_H */
/* ================================ End Of File ====================================*/

