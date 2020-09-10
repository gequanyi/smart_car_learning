#include "image_process.h"

/*  默认分辨率是            188*120
 *  默认FPS                 50帧
 *  访问方式非常简单，可以直接使用下标的方式访问
 *  例如访问第10行 50列的点，mt9v03x_csi_image[10][50]就可以
 */


/***************摄像头相关定义*************/

uint8_t image_threshold;        //图像阈值
uint8_t mt9v03x_csi_image_after[MT9V03X_CSI_H][MT9V03X_CSI_W];//存放二值化后的像素点灰度值



int16_t first_left_point=nHeight;
int16_t first_right_point=nHeight;
int16_t last_mid_point;     //表示最新底行中点
int mid_test;

Row_information normalrow;


/**********普通扫线*************/
Point_info	left_point[nHeight],left[nHeight];		//左线 点情况
Point_info	right_point[nHeight],right[nHeight];		//右线 点情况
Point_info	mid_point[nHeight];			//中线 点情况


/*********************************************
 * @input:二值化图像首地址
 * @output:无
 * @date:2020.09.08
 * @description:边线提取
 * ******************************************/
void edge_line_search(uint8 *src)
{
    /*********局部变量区********/
    int16_t i,j;                        //设i表示行数（图像高度），j表示列数（图像宽度）
    int16_t first_left_lose = nHeight;
    int16_t first_right_lose = nHeight;
    


    //**/
    uint8_t *pimg;								//用来扫第二行及之后的线
    uint8_t *pimg_1;							//用来第一行扫线
    uint8_t tmpsrc;
    uint8_t left_case,right_case;

    /********全局变量赋值******/
    first_left_point = nHeight;
    first_right_point = nHeight;

    RowinformationReset(&normalrow);


    /******静态局部变量区******/
    static int16_t first_line_num,first_line_sum;
    static int16_t last_mid_first;

    /******代码******/
    pimg_1 = src+((MT9V03X_CSI_H-1)*MT9V03X_CSI_W);   //指向图像最低行

    for ( j = 0; j < nWidth; j++)
    {
        tmpsrc=*(pimg_1+j);
        if (tmpsrc==255)                         //是否直接将中值设为nWidth/2
        {
            first_line_num++;
            first_line_sum+=j;
        }
        
    }
	last_mid_first = first_line_sum / first_line_num ;   //计算最低行中值
    //mid_test=last_mid_first;
    //last_mid_first=nWidth/2;
    first_line_sum = 0;
    first_line_num = 0;
    last_mid_point = last_mid_first;
    if (last_mid_point == 0)
    {
		last_mid_point = nWidth/2;          //如果最低行全为黑点，将中值设为nWidth/2，这样也可以处理急弯
    }
    for ( i = 0; i < nHeight; i++)
        {
            pimg = src + ((MT9V03X_CSI_H - i-2)*nWidth);

            for(j = last_mid_point ; j>=0 ;j--)
            {
				
                if (*(pimg+j) == 0 && *(pimg+j+1) == 255 && j!=0)  //0代表黑色，255代表白色，即此点是黑点而其右侧为白点，且不是图像边界点
                {
					
                    left_point[i].x = j+1;
                    left_point[i].y = MT9V03X_CSI_H-i;
                    left_point[i].handle = j + 1;
                    left_point[i].state = NORMAL;
                    left_case = 1;
                    if(first_left_point == nHeight)      //左边找到的第一个点,行数
                    {
                        first_left_point = i;
                    }
					break;
                
                }      
                else if(j == 0)                     //丢线
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
                    if(first_right_point == nHeight)      //右边找到的第一个点
                    {
                        first_right_point = i;
                    }
					break;  
                }      
                else if(j == nWidth - 1)                     //丢线
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
 * @input:图像像素，宽，高
 * @output:阈值
 * @date:2020.09.05
 * @description:大津法优化，计算阈值
 * ******************************************/

#ifdef youhua
uint8_t get_threshold(uint8_t *image,uint16_t col,uint16_t row)
{
    #define Grayscale 256
    uint16_t width=col;     //图像宽度
    uint16_t height=row;    //图像高度

    int pixelCount[Grayscale];              //不同像素值的像素点分别计数
    float pixelPro[Grayscale];              //每个灰度值对应的像素点在整幅图（1/4）的比例
    int i,j,pixelSum=width*height/4;
    uint8_t threshold=0;
    uint8_t *data=image;      //指向像素数据的指针
    for(i=0;i<Grayscale;i++)
    {
        pixelCount[i]=0;
        pixelPro[i]=0;
    }

    uint32_t gray_sum=0;
    //统计灰度级中每个像素在整幅图像中的个数
    for(i=0;i<height;i+=2)     //相邻两个像素点的灰度值其实是相近甚至相等的，尤其是分辨率高的时候，具体可以看实验效果
    {
        for(j=0;j<width;j+=2)
        {
            pixelCount[(int)data[i*width+j]]++;//将当前的点的像素值作为计数数组的下标  ，每个灰度值所占比例
            gray_sum+=(int)data[i*width+j];//灰度值的总和
        }
    }

    //计算每个像素值的点在整幅图像中的比例
    for(i=0;i<Grayscale;i++)
    {
        pixelPro[i]=(float)pixelCount[i]/pixelSum;
    }
    //遍历灰度级[0,255]
    float w0,w1,u0tmp,u1tmp,u0,u1,u,deltaTmp,deltaMax=0;
    w0=w1=u0tmp=u1tmp=u0=u1=u=deltaTmp=0;
    for(j=0;j<Grayscale;j++)
    {
        w0+=pixelPro[j];            //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp+=j*pixelPro[j];       //背景部分 每个灰度值的点的比例*灰度值

        w1=1-w0;                    //前景比例
        u1tmp=gray_sum/pixelSum-u0tmp;  

        u0=u0tmp/w0;                //背景平均灰度
        u1=u1tmp/w1;                //前景平均灰度
        u=u0tmp+u1tmp;              //全局平均灰度
        deltaTmp=w0*pow((u0-u),2)+w1*pow((u1-u),2);     //间类方差公式

        if(deltaTmp>deltaMax)
        {
            deltaMax=deltaTmp;
            threshold=j;
        }
        if(deltaTmp<deltaMax)//把最大类间方差与阈值的关系认为二次型（可以证明），则当寻到极值点时，下一个阈值最大类间方差将减小
        {
            break;
        }
    }

    return threshold;
}
#endif

#ifdef changgui         //常规大津法，运算繁琐
uint8 get_threshold(uint8 *image, uint16 col, uint16 row)
{
    #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height;
    uint8 threshold = 0;
    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //统计灰度级中每个像素在整幅图像中的个数  
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
        }
    }

    //计算每个像素值的点在整幅图像中的比例  
    float maxPro = 0.0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
        if (pixelPro[i] > maxPro)
        {
            maxPro = pixelPro[i];
        }
    }

    //遍历灰度级[0,255]  
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    for (i = 0; i < GrayScale; i++)     // i作为阈值
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {
            if (j <= i)   //背景部分  
            {
                w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
                u0tmp += j * pixelPro[j];  //背景部分 每个灰度值*比例  
            }
            else   //前景部分  
            {
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }
        u0 = u0tmp / w0;              //背景平均灰度
        u1 = u1tmp / w1;              //前景平均灰度
        u = u0tmp + u1tmp;            //全局平均灰度
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
 * @input:无
 * @output:无
 * @date:2020.09.06
 * @description:灰度图像二值化
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
 * @description:图像显示（二值化、边线）
 * ******************************************/
site_t canny_site;
void pic_display(void)
{
    int i;
    ips200_displayimage032_zoom1(mt9v03x_csi_image_after[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 0, 0, MT9V03X_CSI_W, MT9V03X_CSI_H);//显示二值化
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
void RowinformationReset(Row_information *row)      //清除行信息
{
    row->all_lose_count = row->all_normal_count = row->left_lose_count = row->right_lose_count = 0;
}














