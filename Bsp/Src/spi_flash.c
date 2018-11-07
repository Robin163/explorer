/**
 * =============================================================================
 * @file    spi_flash.c
 * @author  Robin li, Robin.li@defatechnology.com
 * @version V1.0
 * @date    2018年08月02日 11时56分30秒
 * @brief
 *
 *
 * =============================================================================
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY CO.,LTD.</center></h2>
 * =============================================================================
 */
/* Includes ------------------------------------------------------------------*/
#include <stdint-gcc.h>
#include <stdio.h>

#include "board.h"
#include "stm32f10x.h"
#include "spi_flash.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define FlashEnableSpi() GPIO_ResetBits( PORTC_FLASH_SPI, PIN_FLASH_SPI_CE )
#define FlashDisableSpi() GPIO_SetBits( PORTC_FLASH_SPI, PIN_FLASH_SPI_CE )

/* Extern  variables ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t readArray[5] = { 0x00 };
/* Private functions ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void ConfigFlashSpiGpio( void );
static void ConfigFlashSpi( void );
static uint8_t FlashSendByte( uint8_t byte );
static void FlashEnableWrite( void );
static void FlashDisableWrite( void );
static void FlashWaitForWriteEnd( void );

/**
 * @name   SpiFlashConfig
 * @param  none.
 * @param  none.
 * @retval none.
 * @brief
 *
 */
void
SpiFlashConfig( void )
{
    ConfigFlashSpiGpio();
    ConfigFlashSpi();
}
/**
 * @name   ConfigFlashSpiGpio
 * @param  none.
 * @param  none.
 * @retval none.
 * @brief
 *
 */
static void
ConfigFlashSpiGpio( void )
{
    GPIO_InitTypeDef initStruct;
    /* Enable MISO GPIO */
    RCC_APB2PeriphClockCmd( RCC_GPIOB_FLASH_SPI, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_GPIOC_FLASH_SPI, ENABLE );
    initStruct.GPIO_Pin = PIN_FLASH_SPI_MISO;
    initStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( PORTB_FLASH_SPI, &initStruct );
    /* Enable MOSI&CLK GPIO */
    initStruct.GPIO_Pin = PIN_FLASH_SPI_MOSI | PIN_FLASH_SPI_CLK;
    initStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( PORTB_FLASH_SPI, &initStruct );
    /* Enable hold GPIO */
    initStruct.GPIO_Pin = PIN_FLASH_SPI_HOLD;
    initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init( PORTB_FLASH_SPI, &initStruct );
    GPIO_SetBits( PORTB_FLASH_SPI, PIN_FLASH_SPI_HOLD );
    /* Enable CE&WP GPIO */
    initStruct.GPIO_Pin = PIN_FLASH_SPI_CE | PIN_FLASH_SPI_WP;
    initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init( PORTC_FLASH_SPI, &initStruct );
    GPIO_SetBits( PORTC_FLASH_SPI, PIN_FLASH_SPI_CE );
    GPIO_SetBits( PORTC_FLASH_SPI, PIN_FLASH_SPI_WP );
}

/**
 * @name   ConfigFlashSpi
 * @param  None.
 * @param  None.
 * @retval None.
 * @brief  config flash spi
 *
 */
static void
ConfigFlashSpi( void )
{
    /* flash spi clock enable */
    RCC_APB1PeriphClockCmd( RCC_FLASH_SPI, ENABLE );
    /* SPI setting */
    SPI_InitTypeDef initFlashSpi = FLASH_INIT_SPI_PARA;
    SPI_Init( FLASH_SPI, &initFlashSpi );
    SPI_Cmd( FLASH_SPI, ENABLE );
    //    FlashEnableSpi();
    //	FlashSendByte( 0x05 );
    //    FlashDisableSpi();
    /*!< Send "EWSR/WRSR " instruction */
    FlashEnableSpi();
    FlashSendByte( 0x50 );
    FlashDisableSpi();
    /*!< Disable write protection for all blocks. */
    FlashEnableSpi();
    FlashSendByte( 0x01 );
    FlashSendByte( 0x80 );
    FlashDisableSpi();
}

/**
 * @brief   spi send data.
 * @param   None.
 * @retval  None.
 * @sa
 *
 */
static uint8_t FlashSendByte( uint8_t byte )
{
    /* waitting previous send data finished! */
    while( SPI_I2S_GetFlagStatus( FLASH_SPI, SPI_I2S_FLAG_TXE ) == RESET );

    /* send data to flash */
    SPI_I2S_SendData( FLASH_SPI, byte );

    /* waitting for get data finished! */
    while( SPI_I2S_GetFlagStatus( FLASH_SPI, SPI_I2S_FLAG_RXNE ) == RESET );

    uint8_t spiData = SPI_I2S_ReceiveData( FLASH_SPI );
    return spiData;
}

/**
  * @brief  Enables the write access to the FLASH.
  * @param  None
  * @retval None
  */
static void FlashEnableWrite( void )
{
    /*!< Send "Write Enable" instruction */
    FlashEnableSpi();
    FlashSendByte( FLASH_CMD_WREN );
    FlashDisableSpi();
}

/**
  * @brief  Disable the write access to the FLASH.
  * @param  None
  * @retval None
  */
static void FlashDisableWrite( void )
{
    /*!< Select the FLASH: Chip Select low */
    FlashEnableSpi();
    /*!< Send "Write Enable" instruction */
    FlashSendByte( FLASH_CMD_WRDI );
    /*!< Deselect the FLASH: Chip Select high */
    FlashDisableSpi();
}



/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *         status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
static void FlashWaitForWriteEnd( void )
{
    uint8_t flashstatus = 0;
    /*!< Select the FLASH: Chip Select low */
    FlashEnableSpi();
    /*!< Send "Read Status Register" instruction */
    FlashSendByte( FLASH_CMD_RDSR );

    /*!< Loop as long as the memory is busy with a write cycle */
    do
    {
        /*!< Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
        flashstatus = FlashSendByte( FLASH_DUMMY_BYTE );
    } while( ( flashstatus & sFLASH_STATUS_BUSY ) == SET ); /* Write in progress */

    /*!< Deselect the FLASH: Chip Select high */
    FlashDisableSpi();
}

uint32_t FlashReadID( void )
{
    /*!< Send "Read-ID " instruction */
    FlashEnableSpi();
    FlashSendByte( FLASH_CMD_RDID );
    uint32_t sst25ID = FlashSendByte( 0x00 );
    sst25ID = ( sst25ID << 8 ) + FlashSendByte( 0x00 );
    sst25ID = ( sst25ID << 8 ) + FlashSendByte( 0x00 );
    FlashDisableSpi();
    //    printf ( "%X%X \n\r", (uint16_t)(sst25ID>>16),(uint16_t)(sst25ID) );
    return ( sst25ID );
}

/**
 * @name   FlashEraseSector
 * @param  addr
 * @param  none.
 * @retval none
 * @brief  erase flash one sector (4k Bytes)
 *
 */
void FlashEraseSector( uint32_t addr )
{
    FlashEnableWrite();
    FlashEnableSpi();
    FlashSendByte( FLASH_CMD_SE_4K );
    FlashSendByte( ( addr >> 16 ) & 0xFF );
    FlashSendByte( ( addr >> 8 ) & 0xFF );
    FlashSendByte( addr & 0xFF );
    FlashDisableSpi();
    /*!< Wait the end of Flash writing */
    FlashWaitForWriteEnd();
}

/**
 * @name   FlashEraseChip
 * @param  addr
 * @param  none.
 * @retval none
 * @brief  erase flash the whole chip (1M Bytes)
 *
 */
void FlashEraseChip( void )
{
    FlashEnableWrite();
    FlashEnableSpi();
    FlashSendByte( FLASH_CMD_CHIP );
    FlashDisableSpi();
    /*!< Wait the end of Flash writing */
    FlashWaitForWriteEnd();
}


/**
 * @name   FlashRead
 * @param  addr.
 * @param  length.
 * @retval none.
 * @brief  flash read a string
 *
 */
void FlashReadString( uint32_t addr, uint16_t *pBuf, uint16_t length )
{
    /*!< Select the FLASH: Chip Select low */
    FlashEnableSpi();
    /*!< Send "Read from Memory " instruction */
    FlashSendByte( FLASH_CMD_READ );
    FlashSendByte( ( addr >> 16 ) & 0xFF );
    FlashSendByte( ( addr >> 8 ) & 0xFF );
    FlashSendByte( addr & 0xFF );
    uint16_t byteIndex = 0x00;

    while( byteIndex < length )
    {
        /*!< Read a byte from the FLASH */
        *( pBuf + byteIndex ) = FlashSendByte( FLASH_DUMMY_BYTE );
        *( pBuf + byteIndex ) += FlashSendByte( FLASH_DUMMY_BYTE ) << 8;
        byteIndex++;
    }

    /*!< Deselect the FLASH: Chip Select high */
    FlashDisableSpi();
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
  *         or less than "sFLASH_PAGESIZE" value.
  * @retval None
  */
void FlashWriteString( uint32_t addr, uint16_t *pBuf, uint16_t length )
{
    /*!< Enable the write access to the FLASH */
    FlashEnableWrite();

    for( uint16_t writeCnt = 0; writeCnt < length; writeCnt++ )
    {
        /*!< Select the FLASH: Chip Select low */
        FlashEnableSpi();
        /*!< Send "Auto Address Increase " instruction */
        FlashSendByte( FLASH_CMD_WAAIE );

        if( writeCnt == 0 )
        {
            FlashSendByte( ( addr >> 16 ) & 0xFF );
            FlashSendByte( ( addr >> 8 ) & 0xFF );
            FlashSendByte( addr & 0xFF );
        }

        /*!< Send first 2 bytes to write to */
        FlashSendByte( ( uint8_t ) * ( pBuf + writeCnt ) );
        FlashSendByte( ( uint8_t )( *( pBuf + writeCnt ) >> 8 ) );
        /*!< Deselect the FLASH: Chip Select high */
        FlashDisableSpi();
        /*!< Wait the end of Flash writing */
        FlashWaitForWriteEnd();
    }

    /*!< Disable the write access to the FLASH */
    FlashDisableWrite();
    /*!< Wait the end of Flash writing */
    FlashWaitForWriteEnd();
}


/**
 * @name   FlashTest
 * @param
 * @param
 * @retval
 * @brief
 *
 */
void
FlashTest( void )
{
    FlashReadString( 0x800, readArray, 5 );
    printf( "Flash read data: %X \n\r", readArray[0] );
    printf( "Flash read data: %X \n\r", readArray[1] );
    printf( "Flash read data: %X \n\r", readArray[2] );
    printf( "Flash read data: %X \n\r", readArray[3] );
    printf( "Flash read data: %X \n\r", readArray[4] );
}
/**
  *@}
  */
/* ============================= End Of File ================================ */

