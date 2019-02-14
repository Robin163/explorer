/**
 ***********************************************************************************
 * @weakgroup bsp_driver
 * @{
 * @brief Device drivers module.
 * @file  spi_flash.c
 * @brief spi flash driver.
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
#include "FreeRTOS.h"
#include "spi_flash.h"
#include "cmsis_os.h"

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi_flash;

/* Private function prototypes -----------------------------------------------*/


/** @addtogroup Template_Project
 * @{
 */

/**
 * @brief   spi flash configration.
 * @param   None.
 * @retval  None.
 * @sa
 *
 */
void SpiFlashConfig(void)
{
    /*##-1- Configure the SPI peripheral #######################################*/
    /* Set the SPI parameters */
    hspi_flash.Instance=SPI_FLASH;                         //SPI1
    hspi_flash.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    hspi_flash.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
    hspi_flash.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    hspi_flash.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    hspi_flash.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    hspi_flash.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    hspi_flash.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    hspi_flash.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    hspi_flash.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    hspi_flash.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    hspi_flash.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    if(HAL_SPI_Init(&hspi_flash) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }
}

//SPI5底层驱动，时钟使能，引脚配置
//此函数会被HAL_SPI_Init()调用
//hspi:SPI句柄
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;

    SPI_FLASH_CLK_ENABLE();        //使能SPI1时钟
    SPI_FLASH_SCK_GPIO_CLK_ENABLE();       //使能GPIOB时钟
    SPI_FLASH_MISO_GPIO_CLK_ENABLE();       //使能GPIOB时钟
    SPI_FLASH_MOSI_GPIO_CLK_ENABLE();       //使能GPIOB时钟

    //PB3,4,5
    GPIO_Initure.Pin= SPI_FLASH_SCK_PIN;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速
    GPIO_Initure.Alternate=SPI_FLASH_SCK_AF;           //复用为SPI1
    HAL_GPIO_Init(SPI_FLASH_SCK_GPIO_PORT,&GPIO_Initure);

    GPIO_Initure.Pin= SPI_FLASH_MISO_PIN;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速
    GPIO_Initure.Alternate=SPI_FLASH_MISO_AF;           //复用为SPI1
    HAL_GPIO_Init(SPI_FLASH_MISO_GPIO_PORT,&GPIO_Initure);

    GPIO_Initure.Pin= SPI_FLASH_MOSI_PIN;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速
    GPIO_Initure.Alternate=SPI_FLASH_MOSI_AF;           //复用为SPI1
    HAL_GPIO_Init(SPI_FLASH_MOSI_GPIO_PORT,&GPIO_Initure);
}

//SPI速度设置函数
//SPI速度=fAPB1/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1时钟一般为42Mhz：
void SpiFlashSetSpeed(uint8_t SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&hspi_flash);            //关闭SPI
    hspi_flash.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    hspi_flash.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&hspi_flash);             //使能SPI

}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SpiFlashReadWriteByte(uint8_t TxData)
{
    uint8_t Rxdata;
    HAL_SPI_TransmitReceive(&hspi_flash,&TxData,&Rxdata,1, 1000);
    return Rxdata;          		    //返回收到的数据
}
