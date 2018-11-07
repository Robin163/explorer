/**
 * ============================================================================
 * @file    spi_flash.h
 * @author  Robin li, Robin.li@defatechnology.com
 * @version V1.0
 * @date    2018年08月02日 13时07分44秒
 * @brief   Definitions for spi flash driver.
 *
 * ============================================================================
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY CO.,LTD.</center></h2>
 * ============================================================================
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __SPI_FLASH_H
#define  __SPI_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint-gcc.h>

/* Includes ------------------------------------------------------------------*/
#define FLASH_DUMMY_BYTE         0xA5
#define FLASH_SPI_PAGESIZE       0x1000
#define FLASH_SST25_ID           0xBF258E

/* Exported types ------------------------------------------------------------*/
/**
 * @brief SST25 commands.
 */
enum sst25_commands
{
    FLASH_CMD_WRITE    = 0x02,  /*!< Write to Memory instruction */
    FLASH_CMD_WAAIE    = 0xAD,  /*!< Write to Memory instruction */
    FLASH_CMD_WRSR     = 0x01,  /*!< Write Status Register instruction */
    FLASH_CMD_WREN     = 0x06,  /*!< Write enable instruction */
    FLASH_CMD_WRDI     = 0x04,  /*!< Write disable instruction */
    FLASH_CMD_READ     = 0x03,  /*!< Read from Memory instruction */
    FLASH_CMD_RDSR     = 0x05,  /*!< Read Status Register instruction  */
    FLASH_CMD_RDID     = 0x9F,  /*!< Read identification */
    FLASH_CMD_SE_4K    = 0x20,  /*!< 4K Sector Erase instruction */
    FLASH_CMD_BLK_32K  = 0x52,  /*!< 32K Sector Erase instruction */
    FLASH_CMD_BLK_64K  = 0xD8,  /*!< 64K Sector Erase instruction */
    FLASH_CMD_CHIP     = 0xC7,  /*!< chip Erase instruction */
    FLASH_CMD_EWSR	   = 0x50,  /*!< Enable write status register */
};

/**
 * @brief SST25 status register masks.
 */
enum sst25_status_register
{
    sFLASH_STATUS_BUSY   = 0x01, //< Write operation in progress.
    sFLASH_STATUS_WEL    = 0x02, //< Write enabled.
    sFLASH_STATUS_BP0    = 0x04, //< Block write protection.
    sFLASH_STATUS_BP1    = 0x08, //< Block write protection.
    sFLASH_STATUS_BP2    = 0x10, //< Block write protection.
    sFLASH_STATUS_BP3    = 0x20, //< Block write protection.
    sFLASH_STATUS_AAI    = 0x40, //< Auto address increment programming.
    sFLASH_STATUS_BPL    = 0x80  //< BPx read only (use /WP HW pin instead).
};
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/*=============================================================
**Constant vector define
**===========================================================*/
#define FLASH_INIT_SPI_PARA { SPI_Direction_2Lines_FullDuplex, SPI_Mode_Master, \
        SPI_DataSize_8b, SPI_CPOL_High, SPI_CPHA_2Edge, SPI_NSS_Soft, SPI_BaudRatePrescaler_4, \
        SPI_FirstBit_MSB, 7 }

/* Exported functions --------------------------------------------------------*/
/*SPI configration.*/
extern void SpiFlashConfig( void );
extern uint32_t FlashReadID( void );
extern void FlashEraseSector( uint32_t addr );
extern void FlashEraseChip( void );
extern void FlashReadString( uint32_t addr, uint16_t *pBuf, uint16_t length );
extern void FlashWriteString( uint32_t addr, uint16_t *pBuf, uint16_t length );
extern void FlashTest( void );




#ifdef __cplusplus
}/*extern C*/
#endif

#endif  /* __SPI_FLASH_H */
/* ============================= End Of File ================================ */

