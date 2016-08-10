#ifndef _BREAK_H
#define _BREAK_H
extern int Stop_Point;
void stop(void);

extern float stop_engine;
extern float stop_engine_angle;
extern int stopCount;
extern int stopStatus;
void brake_timer_cfg(void);
extern int brakesec;
extern int release;

#endif
