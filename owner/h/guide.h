#ifndef _GUIDE_H
#define _GUIDE_H

void move_car(float speed);
void keep_line(float dis_err, float ang_err, float forward_speed);
void keep_point(float x_err, float y_err,float ang_err);
void stop_car(void);

enum robot_status{unloacted,see_point,adjust_pos,on_road, arrive_target,leave_point,on_road_slow, back_slow};

typedef struct {
		unsigned char x;
		unsigned char y;
}point_pos;

extern enum robot_status my_robot;

extern point_pos start_pos;
extern point_pos target_pos;
extern point_pos now_pos;


extern float move_distance,AGV_speed;

#endif
