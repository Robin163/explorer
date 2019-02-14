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
#include "FreeRTOS.h"
#include "uart_com.h"
#include "cmsis_os.h"

/* Private variables ---------------------------------------------------------*/
#ifdef __GNUC__
    /* With GCC, small printf (option LD Linker->Libraries->Small printf
    set to 'Yes') calls __io_putchar() */
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

UART_HandleTypeDef huart_com;

/* Private function prototypes -----------------------------------------------*/


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
void UartComConfig( void )
{
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */
    huart_com.Instance = USART_COM;
    huart_com.Init.BaudRate = 115200;
    huart_com.Init.WordLength = UART_WORDLENGTH_8B;
    huart_com.Init.StopBits = UART_STOPBITS_1;
    huart_com.Init.Parity = UART_PARITY_NONE;
    huart_com.Init.Mode = UART_MODE_TX_RX;
    huart_com.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart_com.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart_com) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief UART MSP Initialization
 * This function configures the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(huart->Instance==USART_COM)
    {
        /* USER CODE BEGIN USART3_MspInit 0 */

        /* USER CODE END USART3_MspInit 0 */
        /* Peripheral clock enable */
        USART_COM_CLK_ENABLE();

        USART_COM_RX_GPIO_CLK_ENABLE();
        USART_COM_TX_GPIO_CLK_ENABLE();
        /**USART_COM GPIO Configuration
          */
        GPIO_InitStruct.Pin = USART_COM_TX_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = USART_COM_TX_AF;
        HAL_GPIO_Init(USART_COM_TX_GPIO_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = USART_COM_RX_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = USART_COM_RX_AF;
        HAL_GPIO_Init(USART_COM_RX_GPIO_PORT, &GPIO_InitStruct);
        /* USER CODE BEGIN USART3_MspInit 1 */

        /* USER CODE END USART3_MspInit 1 */
    }

}

/**
 * @brief UART MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

    if(huart->Instance==USART_COM)
    {
        /* USER CODE BEGIN USART3_MspDeInit 0 */

        /* USER CODE END USART3_MspDeInit 0 */
        /* Peripheral clock disable */
        USART_COM_CLK_DISABLE();

        /**USART_COM GPIO Configuration
          */
        HAL_GPIO_DeInit(USART_COM_TX_GPIO_PORT, USART_COM_TX_PIN);
        HAL_GPIO_DeInit(USART_COM_TX_GPIO_PORT, USART_COM_TX_PIN);

        /* USER CODE BEGIN USART3_MspDeInit 1 */

        /* USER CODE END USART3_MspDeInit 1 */
    }

}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart_com, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
void  StartUartSendTask(  void const * argument )
{
    /* USER CODE BEGIN 5 */
    static uint16_t uartCnt = 0;
    /* Infinite loop */
    for(;;)
    {
        uartCnt++;
        printf("\r\n display uartCnt1: %d", uartCnt);
        osDelay(500);
    }
}

/** @} */

/* ================================ End Of File ====================================*/

