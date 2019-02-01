/**
 ***********************************************************************************
 * @file  led_drv.h
 * @brief Definitions for led driver.
 * @author Robin Li
 * @version V1.0.0
 * @date 09/05/2018
 ************************************************************************************
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY (WUXI) CO.,LTD.</center></h2>
 ************************************************************************************
 */

#ifndef __TOUCH_H
#define __TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint-gcc.h>
#include "gt9147.h"


#define TP_PRES_DOWN 0x80  //����������
#define TP_CATH_PRES 0x40  //�а���������
#define CT_MAX_TOUCH  5    //������֧�ֵĵ���,�̶�Ϊ5��

//������������
typedef struct
{
	uint16_t x[CT_MAX_TOUCH]; 		//��ǰ����
	uint16_t y[CT_MAX_TOUCH];		//�����������5������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
								//x[4],y[4]�洢��һ�ΰ���ʱ������.
	uint8_t  sta;					//�ʵ�״̬
								//b4~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
//�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
//b0:0,����(�ʺ�����ΪX����,����ΪY�����TP)
//   1,����(�ʺ�����ΪY����,����ΪX�����TP)
//b7:0,������ 1,������
	uint8_t touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//������������touch.c���涨��

extern void  StartTouchTask(  void const * argument );


#ifdef __cplusplus
}/*extern C*/
#endif

#endif /* __TOUCH_H */
/* ================================ End Of File ====================================*/

















