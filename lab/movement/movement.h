/*
 * movement.h
 *
 *  Created on: Feb 4, 2025
 *      Author: tek
 */

#ifndef MOVEMENT_H_

#define MOVEMENT_H_

#include <lab/oi/open_interface.h>
#include <lab/oi/open_interface.h>
#include <lab/log/log.h>

typedef enum LINEAR_MOVEMENT {
    FORWARD,
    BACKWARD
} LINEAR_MOVEMENT;

typedef enum ROTATE_MOVEMENT {
    LEFT,
    RIGHT
} ROTATE_MOVEMENT;

typedef enum BUMP {
    BUMP_NONE,
    BUMP_LEFT,
    BUMP_RIGHT
} BUMP;

double doubleMin(double a, double b);
double doubleMax(double a, double b);
int doubleIsBetween(double lower, double upper, double value);

double getRealDegreeTarget(double targetDegrees);

double move(oi_t*, double, LINEAR_MOVEMENT, void(oi_t*, BUMP*, double*));
double move_dieOnBump(oi_t*, double, LINEAR_MOVEMENT);

double rotate(oi_t *sensor_data, double angle, ROTATE_MOVEMENT direction, void(oi_t*, BUMP*, double*));

double just_move(oi_t *sensor_data, double distance_mm);
double just_turn(oi_t *sensor_data, double distance_mm);

BUMP bumpData(oi_t *sensor_data);

#endif
