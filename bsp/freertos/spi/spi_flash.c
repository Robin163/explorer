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
    hspi_flash.Init.Mode=SPI_MODE_MASTER;             //����SPI����ģʽ������Ϊ��ģʽ
    hspi_flash.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    hspi_flash.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    hspi_flash.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    hspi_flash.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    hspi_flash.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    hspi_flash.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    hspi_flash.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    hspi_flash.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    hspi_flash.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    hspi_flash.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    if(HAL_SPI_Init(&hspi_flash) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }
}

//SPI5�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_SPI_Init()����
//hspi:SPI���
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;

    SPI_FLASH_CLK_ENABLE();        //ʹ��SPI1ʱ��
    SPI_FLASH_SCK_GPIO_CLK_ENABLE();       //ʹ��GPIOBʱ��
    SPI_FLASH_MISO_GPIO_CLK_ENABLE();       //ʹ��GPIOBʱ��
    SPI_FLASH_MOSI_GPIO_CLK_ENABLE();       //ʹ��GPIOBʱ��

    //PB3,4,5
    GPIO_Initure.Pin= SPI_FLASH_SCK_PIN;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
    GPIO_Initure.Pull=GPIO_PULLUP;                  //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����
    GPIO_Initure.Alternate=SPI_FLASH_SCK_AF;           //����ΪSPI1
    HAL_GPIO_Init(SPI_FLASH_SCK_GPIO_PORT,&GPIO_Initure);

    GPIO_Initure.Pin= SPI_FLASH_MISO_PIN;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
    GPIO_Initure.Pull=GPIO_PULLUP;                  //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����
    GPIO_Initure.Alternate=SPI_FLASH_MISO_AF;           //����ΪSPI1
    HAL_GPIO_Init(SPI_FLASH_MISO_GPIO_PORT,&GPIO_Initure);

    GPIO_Initure.Pin= SPI_FLASH_MOSI_PIN;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
    GPIO_Initure.Pull=GPIO_PULLUP;                  //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����
    GPIO_Initure.Alternate=SPI_FLASH_MOSI_AF;           //����ΪSPI1
    HAL_GPIO_Init(SPI_FLASH_MOSI_GPIO_PORT,&GPIO_Initure);
}

//SPI�ٶ����ú���
//SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1ʱ��һ��Ϊ42Mhz��
void SpiFlashSetSpeed(uint8_t SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&hspi_flash);            //�ر�SPI
    hspi_flash.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    hspi_flash.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&hspi_flash);             //ʹ��SPI

}

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8_t SpiFlashReadWriteByte(uint8_t TxData)
{
    uint8_t Rxdata;
    HAL_SPI_TransmitReceive(&hspi_flash,&TxData,&Rxdata,1, 1000);
    return Rxdata;          		    //�����յ�������
}
