#include "image_process.h"

/*  Ĭ�Ϸֱ�����            188*120
 *  Ĭ��FPS                 50֡
 *  ���ʷ�ʽ�ǳ��򵥣�����ֱ��ʹ���±�ķ�ʽ����
 *  ������ʵ�10�� 50�еĵ㣬mt9v03x_csi_image[10][50]�Ϳ���
 */


/***************����ͷ��ض���*************/

uint8_t image_threshold;        //ͼ����ֵ
uint8_t mt9v03x_csi_image_after[MT9V03X_CSI_H][MT9V03X_CSI_W];//��Ŷ�ֵ��������ص�Ҷ�ֵ



int16_t first_left_point=nHeight;
int16_t first_right_point=nHeight;
int16_t last_mid_point;     //��ʾ���µ����е�
int mid_test;

Row_information normalrow;


/**********��ͨɨ��*************/
Point_info	left_point[nHeight],left[nHeight];		//���� �����
Point_info	right_point[nHeight],right[nHeight];		//���� �����
Point_info	mid_point[nHeight];			//���� �����


/*********************************************
 * @input:��ֵ��ͼ���׵�ַ
 * @output:��
 * @date:2020.09.08
 * @description:������ȡ
 * ******************************************/
void edge_line_search(uint8 *src)
{
    /*********�ֲ�������********/
    int16_t i,j;                        //��i��ʾ������ͼ��߶ȣ���j��ʾ������ͼ���ȣ�
    int16_t first_left_lose = nHeight;
    int16_t first_right_lose = nHeight;
    


    //**/
    uint8_t *pimg;								//����ɨ�ڶ��м�֮�����
    uint8_t *pimg_1;							//������һ��ɨ��
    uint8_t tmpsrc;
    uint8_t left_case,right_case;

    /********ȫ�ֱ�����ֵ******/
    first_left_point = nHeight;
    first_right_point = nHeight;

    RowinformationReset(&normalrow);


    /******��̬�ֲ�������******/
    static int16_t first_line_num,first_line_sum;
    static int16_t last_mid_first;

    /******����******/
    pimg_1 = src+((MT9V03X_CSI_H-1)*MT9V03X_CSI_W);   //ָ��ͼ�������

    for ( j = 0; j < nWidth; j++)
    {
        tmpsrc=*(pimg_1+j);
        if (tmpsrc==255)                         //�Ƿ�ֱ�ӽ���ֵ��ΪnWidth/2
        {
            first_line_num++;
            first_line_sum+=j;
        }
        
    }
	last_mid_first = first_line_sum / first_line_num ;   //�����������ֵ
    //mid_test=last_mid_first;
    //last_mid_first=nWidth/2;
    first_line_sum = 0;
    first_line_num = 0;
    last_mid_point = last_mid_first;
    if (last_mid_point == 0)
    {
		last_mid_point = nWidth/2;          //��������ȫΪ�ڵ㣬����ֵ��ΪnWidth/2������Ҳ���Դ�����
    }
    for ( i = 0; i < nHeight; i++)
        {
            pimg = src + ((MT9V03X_CSI_H - i-2)*nWidth);

            for(j = last_mid_point ; j>=0 ;j--)
            {
				
                if (*(pimg+j) == 0 && *(pimg+j+1) == 255 && j!=0)  //0�����ɫ��255�����ɫ�����˵��Ǻڵ�����Ҳ�Ϊ�׵㣬�Ҳ���ͼ��߽��
                {
					
                    left_point[i].x = j+1;
                    left_point[i].y = MT9V03X_CSI_H-i;
                    left_point[i].handle = j + 1;
                    left_point[i].state = NORMAL;
                    left_case = 1;
                    if(first_left_point == nHeight)      //����ҵ��ĵ�һ����,����
                    {
                        first_left_point = i;
                    }
					break;
                
                }      
                else if(j == 0)                     //����
                {
                    left_point[i].x = 0;
                    left_point[i].y = MT9V03X_CSI_H-i;
                    left_point[i].handle = j + 1;
                    left_point[i].state = LOSE;
                    left_case = 3;
                    if (first_left_lose == nHeight)
                    {
                        first_left_lose = i;
                    }
                    
                }              
                
            }

            for(j = last_mid_point ; j<nWidth ;j++)
            {
                if (*(pimg+j) == 0 &&*(pimg+j-1) == 255 && j!= (nWidth-1)) 
                {
                    right_point[i].x = j - 1;
                    right_point[i].y = MT9V03X_CSI_H-i;
                    right_point[i].handle = j - 1;
                    right_point[i].state = NORMAL;
                    right_case = 1;
                    if(first_right_point == nHeight)      //�ұ��ҵ��ĵ�һ����
                    {
                        first_right_point = i;
                    }
					break;  
                }      
                else if(j == nWidth - 1)                     //����
                {
                    right_point[i].x = nWidth - 1;
                    right_point[i].y = MT9V03X_CSI_H-i;
                    right_point[i].handle = nWidth -1;
                    right_point[i].state = LOSE;
                    right_case = 0;
                    if (first_right_lose == nHeight)
                    {
                        first_right_lose = i;
                    }
                }              
            }
            if(i > 10)
            {
                switch (left_case - right_case)
                {
                case ALL_NORMAL:
                    mid_point[i].x = (left_point[i].x + right_point[i].x)/2;
                    mid_point[i].y = MT9V03X_CSI_H-i;
                    /******other information*******/

                break;
                //case LEFT_NORMAL_RIGHT_LOSE:
                   // mid_point[i].x
                   // mid_point[i].y
                
                //case RIGHT_NORMAL_LEFT_LOSE:
                    /******details*****/
                
                //case ALL_LOSE:
                    /******details*****/

            
                }
            }
               
            
        }
}

/*********************************************
 * @input:ͼ�����أ�����
 * @output:��ֵ
 * @date:2020.09.05
 * @description:����Ż���������ֵ
 * ******************************************/

#ifdef youhua
uint8_t get_threshold(uint8_t *image,uint16_t col,uint16_t row)
{
    #define Grayscale 256
    uint16_t width=col;     //ͼ����
    uint16_t height=row;    //ͼ��߶�

    int pixelCount[Grayscale];              //��ͬ����ֵ�����ص�ֱ����
    float pixelPro[Grayscale];              //ÿ���Ҷ�ֵ��Ӧ�����ص�������ͼ��1/4���ı���
    int i,j,pixelSum=width*height/4;
    uint8_t threshold=0;
    uint8_t *data=image;      //ָ���������ݵ�ָ��
    for(i=0;i<Grayscale;i++)
    {
        pixelCount[i]=0;
        pixelPro[i]=0;
    }

    uint32_t gray_sum=0;
    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for(i=0;i<height;i+=2)     //�����������ص�ĻҶ�ֵ��ʵ�����������ȵģ������Ƿֱ��ʸߵ�ʱ�򣬾�����Կ�ʵ��Ч��
    {
        for(j=0;j<width;j+=2)
        {
            pixelCount[(int)data[i*width+j]]++;//����ǰ�ĵ������ֵ��Ϊ����������±�  ��ÿ���Ҷ�ֵ��ռ����
            gray_sum+=(int)data[i*width+j];//�Ҷ�ֵ���ܺ�
        }
    }

    //����ÿ������ֵ�ĵ�������ͼ���еı���
    for(i=0;i<Grayscale;i++)
    {
        pixelPro[i]=(float)pixelCount[i]/pixelSum;
    }
    //�����Ҷȼ�[0,255]
    float w0,w1,u0tmp,u1tmp,u0,u1,u,deltaTmp,deltaMax=0;
    w0=w1=u0tmp=u1tmp=u0=u1=u=deltaTmp=0;
    for(j=0;j<Grayscale;j++)
    {
        w0+=pixelPro[j];            //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
        u0tmp+=j*pixelPro[j];       //�������� ÿ���Ҷ�ֵ�ĵ�ı���*�Ҷ�ֵ

        w1=1-w0;                    //ǰ������
        u1tmp=gray_sum/pixelSum-u0tmp;  

        u0=u0tmp/w0;                //����ƽ���Ҷ�
        u1=u1tmp/w1;                //ǰ��ƽ���Ҷ�
        u=u0tmp+u1tmp;              //ȫ��ƽ���Ҷ�
        deltaTmp=w0*pow((u0-u),2)+w1*pow((u1-u),2);     //���෽�ʽ

        if(deltaTmp>deltaMax)
        {
            deltaMax=deltaTmp;
            threshold=j;
        }
        if(deltaTmp<deltaMax)//�������䷽������ֵ�Ĺ�ϵ��Ϊ�����ͣ�����֤��������Ѱ����ֵ��ʱ����һ����ֵ�����䷽���С
        {
            break;
        }
    }

    return threshold;
}
#endif

#ifdef changgui         //�����򷨣����㷱��
uint8 get_threshold(uint8 *image, uint16 col, uint16 row)
{
    #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height;
    uint8 threshold = 0;
    uint8* data = image;  //ָ���������ݵ�ָ��
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++;  //������ֵ��Ϊ����������±�
        }
    }

    //����ÿ������ֵ�ĵ�������ͼ���еı���  
    float maxPro = 0.0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
        if (pixelPro[i] > maxPro)
        {
            maxPro = pixelPro[i];
        }
    }

    //�����Ҷȼ�[0,255]  
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    for (i = 0; i < GrayScale; i++)     // i��Ϊ��ֵ
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {
            if (j <= i)   //��������  
            {
                w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
                u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ*����  
            }
            else   //ǰ������  
            {
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }
        u0 = u0tmp / w0;              //����ƽ���Ҷ�
        u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
        u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = i;
        }
    }

    return threshold;
}
#endif


/*********************************************
 * @input:��
 * @output:��
 * @date:2020.09.06
 * @description:�Ҷ�ͼ���ֵ��
 * ******************************************/
void image_binaryzation(void)
{
    image_threshold=get_threshold(mt9v03x_csi_image[0],MT9V03X_CSI_W,MT9V03X_CSI_H);
    for (uint8_t i = 0; i < MT9V03X_CSI_H; i++)
    {
        for(uint8_t j = 0; j< MT9V03X_CSI_W; j++)
        {
            if (mt9v03x_csi_image[i][j] > image_threshold)
            {
                mt9v03x_csi_image_after[i][j]=255;
            }
            else
            {
                mt9v03x_csi_image_after[i][j]=0;
            }
        }
    }
    
}
/*********************************************
 * @input:
 * @output:
 * @date:2020.09.08
 * @description:ͼ����ʾ����ֵ�������ߣ�
 * ******************************************/
site_t canny_site;
void pic_display(void)
{
    int i;
    ips200_displayimage032_zoom1(mt9v03x_csi_image_after[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 0, 0, MT9V03X_CSI_W, MT9V03X_CSI_H);//��ʾ��ֵ��
    for(i = 0;i < nHeight; i++)
    {
        canny_site.x = mid_point[i].x;
        canny_site.y = mid_point[i].y;          
        ips200_drawpoint(canny_site.x,canny_site.y,RED);
    }
}

/****
 * 
 * **/
void RowinformationReset(Row_information *row)      //�������Ϣ
{
    row->all_lose_count = row->all_normal_count = row->left_lose_count = row->right_lose_count = 0;
}














