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
#include "queue.h"
#include "touch.h"
#include "bsp_lcd.h"
#include "cmsis_os.h"

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
_m_tp_dev tp_dev={
    { 0 },
    { 0 },
    0,
    0
};

//�����Ļ�������Ͻ���ʾ"RST"
void Load_Drow_Dialog(void)
{
    LCD_Clear(WHITE);//����
    POINT_COLOR=BLUE;//��������Ϊ��ɫ
    LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//��ʾ��������
    POINT_COLOR=RED;//���û�����ɫ
}
////////////////////////////////////////////////////////////////////////////////
//���ݴ�����ר�в���
//��ˮƽ��
//x0,y0:����
//len:�߳���
//color:��ɫ
void gui_draw_hline(uint16_t x0,uint16_t y0,uint16_t len,uint16_t color)
{
    if(len==0)return;
    LCD_Fill(x0,y0,x0+len-1,y0,color);
}
//��ʵ��Բ
//x0,y0:����
//r:�뾶
//color:��ɫ
void gui_fill_circle(uint16_t x0,uint16_t y0,uint16_t r,uint16_t color)
{
    uint32_t i;
    uint32_t imax = ((uint32_t)r*707)/1000+1;
    uint32_t sqmax = (uint32_t)r*(uint32_t)r+(uint32_t)r/2;
    uint32_t x=r;
    gui_draw_hline(x0-r,y0,2*r,color);
    for (i=1;i<=imax;i++)
    {
        if ((i*i+x*x)>sqmax)// draw lines from outside
        {
            if (x>imax)
            {
                gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
                gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
            }
            x--;
        }
        // draw lines from inside (center)
        gui_draw_hline(x0-x,y0+i,2*x,color);
        gui_draw_hline(x0-x,y0-i,2*x,color);
    }
}
//������֮��ľ���ֵ
//x1,x2����ȡ��ֵ��������
//����ֵ��|x1-x2|
uint16_t my_abs(uint16_t x1,uint16_t x2)
{
    if(x1>x2)return x1-x2;
    else return x2-x1;
}
//��һ������
//(x1,y1),(x2,y2):��������ʼ����
//size�������Ĵ�ϸ�̶�
//color����������ɫ
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint8_t size,uint16_t color)
{
    uint16_t t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    if(x1<size|| x2<size||y1<size|| y2<size)return;
    delta_x=x2-x1; //������������
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;
    if(delta_x>0)incx=1; //���õ�������
    else if(delta_x==0)incx=0;//��ֱ��
    else {incx=-1;delta_x=-delta_x;}
    if(delta_y>0)incy=1;
    else if(delta_y==0)incy=0;//ˮƽ��
    else{incy=-1;delta_y=-delta_y;}
    if( delta_x>delta_y)distance=delta_x; //ѡȡ��������������
    else distance=delta_y;
    for(t=0;t<=distance+1;t++ )//�������
    {
        gui_fill_circle(uRow,uCol,size,color);//����
        xerr+=delta_x ;
        yerr+=delta_y ;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}
const uint16_t POINT_COLOR_TBL[5]={RED,GREEN,BLUE,BROWN,GRED};
void  StartTouchTask(  void const * argument )
{
    /* USER CODE BEGIN 5 */
    GT9147_Init();	//��������ʼ��
    tp_dev.touchtype|=0X80;	//������
    tp_dev.touchtype|=lcddev.dir&0X01;//������������
    Load_Drow_Dialog();
    uint8_t t=0;
    uint16_t lastpos[5][2];		//���һ�ε�����
    /* Infinite loop */
    for(;;)
    {
        GT9147_Scan(0);
        for(t=0;t<5;t++)
        {
            if((tp_dev.sta)&(1<<t))
            {
                //printf("X����:%d,Y����:%d\r\n",tp_dev.x[0],tp_dev.y[0]);
                if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)
                {
                    if(lastpos[t][0]==0XFFFF)
                    {
                        lastpos[t][0] = tp_dev.x[t];
                        lastpos[t][1] = tp_dev.y[t];
                    }

                    lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//����
                    lastpos[t][0]=tp_dev.x[t];
                    lastpos[t][1]=tp_dev.y[t];
                    if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<20)
                    {
                        Load_Drow_Dialog();//���
                    }
                }
            }
            else
            {
                lastpos[t][0]=0XFFFF;
            }
        }

        osDelay(10);
    }
}
