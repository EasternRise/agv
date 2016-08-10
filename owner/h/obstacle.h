#ifndef __OBSTACLE_H
#define __OBSTACLE_H

#include "stm32f10x.h"

#define X_SIZE 2
#define Y_SIZE 3
#define MAX_POINT X_SIZE*Y_SIZE
#define MAX_EDGE  X_SIZE*Y_SIZE

enum b_status{zero_dir,x_dir,y_dir,all_dir};

void obstacle_init(void);
extern enum b_status p_status[X_SIZE*Y_SIZE];

extern signed char x_direction;  //0:不变， 1 为正 -1 为负
extern signed char y_direction;

typedef struct
{
	u8 x;
	u8 y;
}ob_point;

extern ob_point my_point[X_SIZE*Y_SIZE];
extern u8 point_cnt;
//edge
typedef struct 
{
	u8 start_p;
	u8 stop_p;
	u8 dirction; //1表示竖直方向 y值改变的方向
	
}edge_o;

extern edge_o my_edge[X_SIZE*Y_SIZE];
extern u8 edge_cnt;

void add_edge(edge_o tmp_edg);

#endif
