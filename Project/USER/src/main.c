/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.24
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		
					���߶��壺
					------------------------------------ 
					2.0��IPSģ��ܽ�            ��Ƭ���ܽ�
					D0                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D0_PIN 	�궨��
					D1                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D1_PIN 	�궨��
					D2                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D2_PIN 	�궨��
					D3                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D3_PIN 	�궨��
					D4                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D4_PIN 	�궨��
					D5                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D5_PIN 	�궨��
					D6                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D6_PIN 	�궨��
					D7                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D7_PIN 	�궨��
																						
					BL                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_BL_PIN 	�궨��
					CS                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_CS_PIN 	�궨��
					RD                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_RD_PIN 	�궨��
					WR                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_WR_PIN 	�궨��
					RS                  �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_RS_PIN 	�궨��
					RST                 �鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_RST_PIN 	�궨��

					��Դ����
					VCC 3.3V��Դ
					GND ��Դ��
					���ֱ���240*320
					------------------------------------  
					ģ��ܽ�         	��Ƭ���ܽ�
					SDA(51��RX)         �鿴SEEKFREE_MT9V03X_CSI.h�ļ��е�MT9V03X_CSI_COF_UART_TX	�궨��
					SCL(51��TX)         �鿴SEEKFREE_MT9V03X_CSI.h�ļ��е�MT9V03X_CSI_COF_UART_RX	�궨��
					���ж�(VSY)         �鿴SEEKFREE_MT9V03X_CSI.h�ļ��е�MT9V03X_CSI_VSYNC_PIN		�궨��
					���ж�(HREF)	    ����Ҫʹ��
					�����ж�(PCLK)      �鿴SEEKFREE_MT9V03X_CSI.h�ļ��е�MT9V03X_CSI_PCLK_PIN		�궨��
					���ݿ�(D0-D7)		B31-B24 B31��Ӧ����ͷ�ӿ�D0
					Ĭ�Ϸֱ�����        188*120
					Ĭ��FPS           	50֡
					------------------------------------ 
********************************************************************************************************************/


//�����Ƽ�IO�鿴Projecct�ļ����µ�TXT�ı�



//���µĹ��̻��߹����ƶ���λ�����ִ�����²���
//��һ�� �ر��������д򿪵��ļ�
//�ڶ��� project  clean  �ȴ��·�����������



							// ������ �ؿ� ��������
							// ������ �ؿ� ��������
							// ������ �ؿ� ��������
//*********************************************************************************
//*********************************************************************************
//*********************************************************************************
//
// ��ע�ⱾRT1064�������̵�2.0����Ļ�����뿪Դ���е����Ŷ��岢����ͬ��
// ����������Ϊ�˷���ʹ��˫�㣬�����ݶ˿ڷ���Ϊ���鲻ͬIO�ڵ�����4�����š�
// ���Լ��������壬��IO��������ʱ���뾡��ʹ��ͬһ��IO������8�����ţ����� C8-C15�����������ʾ���ܡ�
//
//*********************************************************************************
//*********************************************************************************
//*********************************************************************************



#include "headfile.h"

int main(void)
{
    DisableGlobalIRQ();
    board_init();   		//��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
    
	mt9v03x_csi_init();		//��ʼ������ͷ	ʹ��CSI�ӿ�
	ips200_init();			//��ʼ��2.0��IPS��Ļ
   
    EnableGlobalIRQ(0);
    while(1)
    {
        if(mt9v03x_csi_finish_flag)			//ͼ��ɼ����
        {
			mt9v03x_csi_finish_flag = 0;	//����ɼ���ɱ�־λ
			
			//ʹ��������ʾ����������ԭʼͼ���С �Լ�������Ҫ��ʾ�Ĵ�С�Զ��������Ż��߷Ŵ���ʾ
			//�����ɼ�����ͼ��ֱ���Ϊ 188*120 ��2.0��IPS����ʾ�ֱ���Ϊ 320*240 ��ͼ������ȫ����ʾ��

			get_threshold(mt9v03x_csi_image[0],MT9V03X_CSI_W,MT9V03X_CSI_H);
			image_binaryzation();
            //ips200_displayimage032_zoom(mt9v03x_csi_image_after[0], MT9V03X_CSI_W, MT9V03X_CSI_H, MT9V03X_CSI_W, MT9V03X_CSI_H);	//��ʾ��ֵ��ͼ��
			edge_line_search(mt9v03x_csi_image_after[0]);
			//ips200_displayimage032_zoom(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 320, 240);//֮ǰ320
			ips200_displayimage032_zoom1(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 0, 120, MT9V03X_CSI_W, MT9V03X_CSI_H);
			
			pic_display();
        }
    }
}





