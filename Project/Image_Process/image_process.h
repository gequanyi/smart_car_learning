#ifndef _IMAGE_PROCESS_H_
#define _IMAGE_PROCESS_H_

#include "headfile.h"
#define youhua
#define nHeight 120
#define nWidth 188

typedef enum
{
	NORMAL,
	LOSE,
	ERROR,
	L_LOSE,//只给mid_point用
	R_LOSE,//只给mid_point用
	//十字等特殊元素未加入
	cross,
	fill,
	HUANDAO,
	SHIZI,
}	point_case;//点情况

typedef enum      //第一个枚举成员的默认值为整型的 0，后续枚举成员的值在前一个成员上加 1
{
	ALL_NORMAL = 0,
	LEFT_NORMAL_RIGHT_LOSE,
	RIGHT_NORMAL_LEFT_LOSE,
	ALL_LOSE,
}	scan_line_case;//扫线情况


typedef struct
{
    uint16_t all_lose_count;
    uint16_t left_lose_count;
    uint16_t right_lose_count;
    uint16_t all_normal_count;
} Row_information;//行信息

typedef struct
{
    int16_t x;
	int16_t	y;
	int16_t handle;
	uint8_t repiar_flag;
	uint16_t rode_width;
	int16_t	mid_line_new;
	point_case state;//
	scan_line_case	line_case;	//扫线的状态
} Point_info;

typedef struct
{
	uint16 x;
	uint16 y;
} site_t;


extern uint8_t image_threshold;        //图像阈值
extern uint8_t mt9v03x_csi_image_after[MT9V03X_CSI_H][MT9V03X_CSI_W];

extern uint8_t get_threshold(uint8_t *image,uint16_t col,uint16_t row);
extern void image_binaryzation(void);
extern void edge_line_search(uint8 *src);
extern void pic_display(void);
extern void RowinformationReset(Row_information *row);












#endif

