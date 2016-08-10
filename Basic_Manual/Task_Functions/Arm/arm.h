#ifndef ARM_H
#define ARM_H

//armaction.h


extern void armaction1(void);
extern void armaction2(void);
extern void armaction3(void);
extern void armaction4(void);
extern void armaction5(void);
extern void armaction6(void);
extern void armaction7(void);
extern void armaction8(void);

extern int flag_RB;
extern int flag_Link;
extern uint8 lift_enable1;
extern uint8 lift_enable2;
extern uint8 FrontArm_Allowed;

extern void TaskArm(void *pdata);
extern uint8 Action_Num;
extern uint8  armaction_done;
#endif