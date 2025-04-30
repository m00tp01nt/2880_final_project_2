#ifndef SERVO_H_
#define SERVO_H_

//#include "cyBot_Scan.h"
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <lab/timer/timer.h>
#include <math.h>
#include <lab/log/log.h>

void servo_init(void);
void servo_move(uint16_t);

void servo_move_degrees(int);

#endif
