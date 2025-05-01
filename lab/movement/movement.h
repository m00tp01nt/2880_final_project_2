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

double doubleMin(double, double);
double doubleMax(double, double);
int doubleIsBetween(double, double, double);

double getRealDegreeTarget(double);

double move(oi_t*, double, LINEAR_MOVEMENT, void(oi_t*, BUMP*, double*));
double move_dieOnBump(oi_t*, double, LINEAR_MOVEMENT);

double rotate(oi_t*, double, ROTATE_MOVEMENT, void(oi_t*, BUMP*, double*));

double just_move(oi_t*, double);
double just_turn(oi_t*, double);

BUMP bumpData(oi_t*);

void grobro_scan_and_drive(oi_t*);

#endif
