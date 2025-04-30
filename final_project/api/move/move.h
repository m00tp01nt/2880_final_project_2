#ifndef FINAL_PROJECT_API_MOVE_H_
#define FINAL_PROJECT_API_MOVE_H_

#define API_LATERAL_SPEED 250
#define API_ANGULAR_SPEED 150

#include <lab/movement/movement.h>          // Movement enums

double api_move(double, LINEAR_MOVEMENT, void(*handler)(BUMP*));

#endif
