#include "all.h"

#include "obstacle.h"


#define MAX_POINT X_SIZE*Y_SIZE
#define MAX_EDGE  X_SIZE*Y_SIZE

//point 0为没有失误
ob_point my_point[X_SIZE*Y_SIZE]={0};
u8 point_cnt=0;

//edge
edge_o my_edge[X_SIZE*Y_SIZE]={0};
u8 edge_cnt=0;

//(x,y) = y*X_SIZE+x;

//认为阻塞是比较少的东西;所以直接存储阻塞点与阻塞边的信息,
//检测的时候直接遍历这些信息来查找,比另一种方式更快。也节省空间。

// void obstacle_init(void)
// {
// 	//point
// 	for(u16 cnt=0; cnt<MAX_POINT; cnt++){
// 		my_point[cnt].x=0xff;
// 		my_point[cnt].y=0xff;
// 	}
// 	
// 	point_cnt=0;
// 	
// 	//edge
// 	for(u16 cnt=0; cnt<MAX_EDGE; cnt++){
// 		my_edge[cnt].point_1.x=0xff;
// 		my_edge[cnt].point_1.y=0xff;
// 		my_edge[cnt].point_2.x=0xff;
// 		my_edge[cnt].point_2.y=0xff;
// 	}
// 	edge_cnt=0;
// }

//数据结构 内部的算法
//调用的函数都写好了



float search_road(void){
// 	//now_pos.x now_pos.y
// 	//target_pos.x target_pos.y
// 	//my_edge edge_cnt
// 	//my_point poing_cnt
// 	u8 min_x=0xff,max_x=0,min_y=0xff,max_y=0;
// 	
// 	if(now_pos.x>target_pos.x){
// 		max_x=now_pos.x;
// 		min_x=target_pos.x;
// 	}
// 	else{
// 		max_x=target_pos.x;
// 		min_x=now_pos.x;
// 	}
// 		
// 	if(now_pos.y>target_pos.y){
// 		max_y=now_pos.y;
// 		min_y=target_pos.y;
// 	}
// 	else{
// 		max_y=target_pos.y;
// 		min_y=now_pos.y;
// 	}
// 	//寻找是否有位于这个区域内边或点阻塞 并记录下来	
// 	
// 	u8 tmp_point_cnt;
// 	ob_point tmp_point[10];
// 	
// 	u8 tmp_edge_cnt;
// 	edge_o tmp_edge[10];
// 	
// // 	tmp_point_cnt=0;
// // 	for(u8 cnt=0; cnt<point_cnt; cnt++){
// // 		if(my_point[cnt].x<=max_x && my_point[cnt].x >=min_x && my_point[cnt].y <= max_y && my_point[cnt].y >= min_y){
// // 			tmp_point[tmp_point_cnt].x  = my_point[cnt].x;
// // 			tmp_point[tmp_point_cnt++].y= my_point[cnt].y;
// // 		}
// // 	}
// // 	
// // 	tmp_edge_cnt=0;
// // 	for(u8 cnt=0; cnt<edge_cnt; cnt++){
// // 		if(my_edge[cnt].point_1.x<=max_x && my_edge[cnt].point_1.x >=min_x && my_edge[cnt].point_1.y <= max_y && my_edge[cnt].point_1.y >= min_y
// // 			&&my_edge[cnt].point_2.x<=max_x && my_edge[cnt].point_2.x >=min_x && my_edge[cnt].point_2.y <= max_y && my_edge[cnt].point_2.y >= min_y
// // 		)
// // 		tmp_edge[tmp_edge_cnt].point_1   = my_edge[cnt].point_1;
// // 		tmp_edge[tmp_edge_cnt++].point_2 = my_edge[cnt].point_2;
// // 	}
// 	
// 	//得到这些数据以后，开始写算法
// 	//基本思想是什么呢
// 	//重新设计数据结构
	return 90;
}

// u8 is_in(ob_point jurge_point)
// {
// 		if(jurge_point.x<=max_x && jurge_point.x >=min_x && jurge_point.y <= max_y && jurge_point.y >= min_y){
// 			return 1;
// 		}
// 		else{
// 			return 0;
// 		}
// }

enum b_status p_status[X_SIZE*Y_SIZE];

signed char x_direction=0;  //0:不变， 1 为正 -1 为负
signed char y_direction=0;

u8 x;
u8 y;
void obstacle_init(void)
{
	//point
	
	if(target_pos.x > now_pos.x)
		x_direction=1;
	else if(target_pos.x < now_pos.x)
		x_direction=-1;
	else
		x_direction=0;
	
	if(target_pos.y > now_pos.y)
		y_direction=1;
	else if(target_pos.y < now_pos.y)
		y_direction=-1;
	else
		y_direction=0;
	
	for(u16 cnt=0; cnt<MAX_POINT; cnt++){
		p_status[cnt]=all_dir;
	}

	//与目标点在一行上的 只能走y方向
	if(x_direction == 1)
	{
		for( x=now_pos.x; x<=target_pos.x; x++)
		{
			p_status[target_pos.y*X_SIZE+x]=x_dir;
		}
	}
	else
	{
		for( x=target_pos.x; x<=now_pos.x; x++)
		{
			p_status[target_pos.y*X_SIZE+x]=x_dir;
		}
	}

	//与目标点在一列上的 只能走x方向
	if(y_direction == 1)
	{
		for( y=now_pos.y; y<=target_pos.y; y++)
		{
			p_status[target_pos.x+y*X_SIZE]=y_dir;
		}
	}
	else
	{
		for( y=target_pos.y; y<=now_pos.y; y++)
		{
			p_status[target_pos.x+y*X_SIZE]=y_dir;
		}
	}
		
	
	//将一些点弄进去
	for(u8 cnt=0; cnt<point_cnt; cnt++){
		p_status[ (my_point[cnt].x-x_direction)  +  my_point[cnt].y*X_SIZE]= y_dir;
		p_status[my_point[cnt].x + (my_point[cnt].y-y_direction)   *X_SIZE]= x_dir;
	}
	//将一些边弄进去
	for(u8 cnt=0; cnt<edge_cnt; cnt++){
// 		my_edge[cnt].point_1;
// 		my_edge[cnt].point_2;
		//水平的边
		if(my_edge[cnt].dirction == 0)
			if(x_direction == 1)
				p_status[my_edge[cnt].start_p]=y_dir;
			else
				p_status[my_edge[cnt].stop_p]=y_dir;
		//竖直的边
		else
			if(y_direction == 1)
				p_status[my_edge[cnt].start_p]=x_dir;
			else
				p_status[my_edge[cnt].stop_p]=x_dir;
	}
}


void add_edge(edge_o tmp_edg)
{
		if(tmp_edg.dirction == 0)
			if(x_direction == 1){
				if(p_status[tmp_edg.start_p]==all_dir)
					p_status[tmp_edg.start_p]=y_dir;
				else if(p_status[tmp_edg.start_p]==x_dir){
					p_status[tmp_edg.start_p]=zero_dir;
					//更新点
				}
			}
			else{
				if(p_status[tmp_edg.stop_p]==all_dir)
					p_status[tmp_edg.stop_p]=y_dir;
				else if(p_status[tmp_edg.stop_p]==x_dir){
					p_status[tmp_edg.stop_p]=zero_dir;
					//更新点
				}
			}
		//竖直的边
		else
			if(y_direction == 1){
				if(p_status[tmp_edg.start_p] == all_dir){
					p_status[tmp_edg.start_p]=x_dir;
				}
				else if(p_status[tmp_edg.start_p] == y_dir){
					p_status[tmp_edg.start_p]=zero_dir;
				}
				
			}
			else{
				if(p_status[tmp_edg.stop_p] == all_dir){
					p_status[tmp_edg.stop_p]=x_dir;
				}
				else if(p_status[tmp_edg.stop_p] == y_dir){
					p_status[tmp_edg.stop_p] = zero_dir;
				}
			}
}

void add_point()
{
		
}



