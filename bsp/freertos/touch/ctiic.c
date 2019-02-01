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
#include "ctiic.h"
#include "cmsis_os.h"

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

//控制I2C速度的延时
void CT_Delay(uint32_t nus)
{
	volatile uint32_t uc_dly;
	volatile uint16_t uc_dly1;

	uc_dly =nus;

	while (uc_dly --)
	{
		for (uc_dly1=0;uc_dly1<20;uc_dly1++)
        {
        };
	};
}
//电容触摸芯片IIC接口初始化
void CT_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE();   		//使能GPIOB时钟
	__HAL_RCC_GPIOF_CLK_ENABLE();   		//使能GPIOF时钟

    //PB0,PF11
    GPIO_Initure.Pin=GPIO_PIN_0;			//PB0设置为推挽输出
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_11;			//PF11设置推挽输出
	HAL_GPIO_Init(GPIOF,&GPIO_Initure);

    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//    CT_IIC_SDA=1;
//	CT_IIC_SCL=1;
}
//产生IIC起始信号
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda线输出
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_SET);
//	CT_IIC_SDA=1;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//	CT_IIC_SCL=1;
	CT_Delay(30);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_RESET);
// 	CT_IIC_SDA=0;//START:when CLK is high,DATA change form high to low
	CT_Delay(2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	CT_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();//sda线输出
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//	CT_IIC_SCL=1;
	CT_Delay(30);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_RESET);
//	CT_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	CT_Delay(2);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_SET);
//	CT_IIC_SDA=1;//发送I2C总线结束信号
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t CT_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	CT_SDA_IN();      //SDA设置为输入
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_SET);
//	CT_IIC_SDA=1;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//	CT_IIC_SCL=1;
	CT_Delay(2);
	while(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_11))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CT_IIC_Stop();
			return 1;
		}
		CT_Delay(2);
	}
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	CT_IIC_SCL=0;//时钟输出0
	return 0;
}
//产生ACK应答
void CT_IIC_Ack(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	CT_IIC_SCL=0;
	CT_SDA_OUT();
	CT_Delay(2);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_RESET);
//	CT_IIC_SDA=0;
	CT_Delay(2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//	CT_IIC_SCL=1;
	CT_Delay(2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	CT_IIC_SCL=0;
}
//不产生ACK应答
void CT_IIC_NAck(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	CT_IIC_SCL=0;
	CT_SDA_OUT();
	CT_Delay(2);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_SET);
//	CT_IIC_SDA=1;
	CT_Delay(2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//	CT_IIC_SCL=1;
	CT_Delay(2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	CT_IIC_SCL=0;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void CT_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
	CT_SDA_OUT();
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//    CT_IIC_SCL=0;//拉低时钟开始数据传输
	CT_Delay(2);
	for(t=0;t<8;t++)
    {
        uint8_t temp=(txd&0x80)>>7;
        if(temp == 0x01)
        {
            HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_RESET);
        }
        txd<<=1;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//		CT_IIC_SCL=1;
		CT_Delay(2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//		CT_IIC_SCL=0;
		CT_Delay(2);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t CT_IIC_Read_Byte(unsigned char ack)
{
	uint8_t i,receive=0;
 	CT_SDA_IN();//SDA设置为输入
	CT_Delay(30);
	for(i=0;i<8;i++ )
	{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//		CT_IIC_SCL=0;
		CT_Delay(2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//		CT_IIC_SCL=1;
		receive<<=1;
		if(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_11))receive++;
	}
	if (!ack)CT_IIC_NAck();//发送nACK
	else CT_IIC_Ack(); //发送ACK
 	return receive;
}

