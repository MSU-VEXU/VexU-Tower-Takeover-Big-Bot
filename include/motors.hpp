#pragma once
#include "main.h"

extern ChassisControllerIntegrated chassis;

extern ChassisControllerPID chassis2;

extern MotorGroup intake;
extern MotorGroup arms;
extern MotorGroup lift;

extern Motor holder;

/** Auton Stuff **/
#define AUTON_NONE  -1
#define AUTON_RED    1
#define AUTON_BLUE   2
#define AUTON_SKILLS 3

extern int selectedAuton;
extern bool auton_far;
extern bool auton_delay_switch;
extern bool auton_floor;
extern bool op_control;

void set_auton(int auton);
/** End Auton Stuff **/
