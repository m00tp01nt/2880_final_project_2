/*
 * movement.c
 *
 *  Created on: Feb 4, 2025
 *      Author: tek
 */

#include "cyBot_Scan.h"


#include <lab/movement/movement.h>

// Would love to put this in 'movement.h', but the compiler throws a fit.
#include <lab/movement/bump/bump_handlers.h>

#define ROTATE_SPEED 50
#define LATERAL_SPEED 200

#define ROTATE_CORRECTION_FACTOR_DEGREES (double)360

double doubleMin(double a, double b) {
    if (a < b)
        return a;
    return b;
}
double doubleMax(double a, double b) {
    if (a > b)
        return a;
    return b;
}
int doubleIsBetween(double lower, double upper, double value) {
    return ((lower < value) && (value < upper));
}

double move(oi_t *sensor_data, double distance_mm, LINEAR_MOVEMENT DIRECTION, void (*handler)(oi_t*, BUMP*, double*))
{
    log_lcd("Moving\0");

    double lowerBound;
    double upperBound;
    
    double sum = 0;

    int leftWheelSpeed;
    int rightWheelSpeed;

    // Make sure we're going to go the right way
    if (DIRECTION == BACKWARD) {
        distance_mm *= -1;
    }

    lowerBound = doubleMin(distance_mm, distance_mm * -1);
    upperBound = doubleMax(distance_mm, distance_mm * -1);

    // Should we be moving Forwards or Backwards?
    if (distance_mm > 0) {
        rightWheelSpeed = LATERAL_SPEED;
        leftWheelSpeed = LATERAL_SPEED;
    }
    else if (distance_mm < 0) {
        rightWheelSpeed = -LATERAL_SPEED;
        leftWheelSpeed = -LATERAL_SPEED;
    }

    oi_setWheels(rightWheelSpeed, leftWheelSpeed);

    BUMP data;

    while (doubleIsBetween(lowerBound, upperBound, sum)) {
        oi_update(sensor_data);
        data = bumpData(sensor_data);
        if (data != BUMP_NONE) {
            handler(sensor_data, &data, &sum);
            log_lcd("Moving\0");
            oi_setWheels(rightWheelSpeed, leftWheelSpeed);
        }
        sum += sensor_data->distance;
    }

    oi_setWheels(0,0);
    log_lcd("Done Moving\0");
    return sum;
}

double move_dieOnBump(oi_t *sensor_data, double distance_mm, LINEAR_MOVEMENT DIRECTION)
{
    log_lcd("Moving\0");

    double lowerBound;
    double upperBound;

    double sum = 0;

    int leftWheelSpeed;
    int rightWheelSpeed;

    // Make sure we're going to go the right way
    if (DIRECTION == BACKWARD) {
        distance_mm *= -1;
    }

    lowerBound = doubleMin(distance_mm, distance_mm * -1);
    upperBound = doubleMax(distance_mm, distance_mm * -1);

    // Should we be moving Forwards or Backwards?
    if (distance_mm > 0) {
        rightWheelSpeed = LATERAL_SPEED;
        leftWheelSpeed = LATERAL_SPEED;
    }
    else if (distance_mm < 0) {
        rightWheelSpeed = -LATERAL_SPEED;
        leftWheelSpeed = -LATERAL_SPEED;
    }

    oi_setWheels(rightWheelSpeed, leftWheelSpeed);

    BUMP data;

    while (doubleIsBetween(lowerBound, upperBound, sum)) {
        oi_update(sensor_data);
        data = bumpData(sensor_data);
        if (data != BUMP_NONE) {
            oi_setWheels(0, 0);
            log_lcd("Bump! Returning...\0");
            return 0;
        }
        sum += sensor_data->distance;
    }

    oi_setWheels(0,0);
    log_lcd("Done Moving\0");
    return 1;
}


// Positive angles are to the LEFT, negative angles are to the RIGHT
double rotate(oi_t *sensor_data, double angle, ROTATE_MOVEMENT direction, void(*handler)(oi_t*, BUMP*, double*))
{
    log_lcd("Turning\0");

    double sum = 0;

    double lowerBound;
    double upperBound;

    int rightWheelSpeed;
    int leftWheelSpeed;

    BUMP data;

    lowerBound = doubleMin(angle, (angle * -1));
    upperBound = doubleMax(angle, (angle * -1));

    if (direction == RIGHT) {
        angle *= -1;
    }

    if (angle > 0) {
        rightWheelSpeed = ROTATE_SPEED;
        leftWheelSpeed = -ROTATE_SPEED;
    }
    else if (angle < 0) {
        rightWheelSpeed = -ROTATE_SPEED;
        leftWheelSpeed = ROTATE_SPEED;
    }

    oi_setWheels(rightWheelSpeed, leftWheelSpeed);

    while (doubleIsBetween(lowerBound, upperBound, sum)) {
        oi_update(sensor_data);
        data = bumpData(sensor_data);
        if (data != BUMP_NONE) {
            handler(sensor_data, &data, &sum);
            log_lcd("Turning\0");
            oi_setWheels(rightWheelSpeed, leftWheelSpeed);
        }
        sum += sensor_data->angle;
    }

    oi_setWheels(0,0);
    log_lcd("Done Turning\0");
    return sum;
}



double getRealDegreeTarget(double targetDegrees)
{
    double answer;
    double ratio = ((double)360 / ROTATE_CORRECTION_FACTOR_DEGREES);
    answer = (ratio * targetDegrees);
    return answer;
}

BUMP bumpData (oi_t *sensor_data)
{
    if (sensor_data->bumpLeft == 1)
    {
        return BUMP_LEFT;
    }
    else if (sensor_data->bumpRight == 1)
    {
        return BUMP_RIGHT;
    }
    else
    {
        return BUMP_NONE;
    }
}

double just_move(oi_t *sensor_data, double distance_mm)
{
    return move(sensor_data, distance_mm, FORWARD, doNothing);
}

double just_turn(oi_t *sensor_data, double angle)
{
    return rotate(sensor_data, angle, LEFT, doNothing);
}

void grobro_scan_and_drive(oi_t *sensor_data)
{
    cyBOT_Scan_t scan;
    int object_found = 0;
    double distance_traveled = 0;

//    Replaced with loglevel()
//    uart_sendStr("GroBro: Scanning LEFT side while moving forward...\r\n");
//    lcd_printf("Scan Left");

    loglevel(PUTTY, EVENT, "GroBro: Scanning LEFT side while moving forward...");
    loglevel(LCD, EVENT, "Scan Left");

    oi_setWheels(100, 100); // Move forward slowly

    while (distance_traveled < 2000)
    {
        cyBOT_Scan(180, &scan);  // Fixed angle = 0° for left scan // CORRECTION - 180° to face left

//        Replaced with loglevel()
//        char buffer[64];
//        sprintf(buffer, "LEFT | IR: %d | Ping: %.2f\r\n", scan.IR_raw_val, scan.sound_dist);
//        uart_sendStr(buffer);
        loglevel(PUTTY, DATA, "LEFT | IR: %d | Ping: %.2f\r\n", scan.IR_raw_val, scan.sound_dist);

        if (scan.sound_dist > 0 && scan.sound_dist < 30.0)
        {
            object_found = 1;
            oi_setWheels(0, 0);
//            Replaced with loglevel()
//            lcd_printf("Left: Object!");
//            uart_sendStr("GroBro: Object detected on LEFT. Stopping.\r\n");
            loglevel(LCD, EVENT, "Left: Object!");
            loglevel(PUTTY, EVENT, "Object detected on LEFT. Stopping.");
            return;
        }

        timer_waitMillis(250);
        oi_update(sensor_data);
        distance_traveled += sensor_data->distance;
    }

    oi_setWheels(0, 0); // Stop forward motion

    if (!object_found)
    {
//        Replaced with loglevel()
//        uart_sendStr("GroBro: No object on left. Backing up and scanning right...\r\n");
//        lcd_printf("Scan Right\nBacking Up");
        loglevel(PUTTY, EVENT, "GroBro: No object on left. Backing up and scanning right...");
        loglevel(LCD, EVENT, "Scan Right\nBacking Up");

        double reversed = 0;
        oi_setWheels(-100, -100); // Back up slowly

        while (reversed > -distance_traveled)
        {
            cyBOT_Scan(180, &scan); // Fixed angle = 180° for right scan
//            Replaced with loglevel()
//            char buffer[64];
//            sprintf(buffer, "RIGHT | IR: %d | Ping: %.2f\r\n", scan.IR_raw_val, scan.sound_dist);
//            uart_sendStr(buffer);
            loglevel(PUTTY, DATA, "RIGHT | IR: %d | Ping: %.2f\r\n", scan.IR_raw_val, scan.sound_dist);

            if (scan.sound_dist > 0 && scan.sound_dist < 30.0)
            {
                object_found = 1;
                oi_setWheels(0, 0);
//                Replaced with loglevel()
//                lcd_printf("Right: Object!");
//                uart_sendStr("GroBro: Object detected on RIGHT. Stopping.\r\n");
                loglevel(LCD, EVENT, "Right: Object!");
                loglevel(PUTTY, EVENT, "GroBro: Object detected on RIGHT. Stopping.");
                return;
            }

            timer_waitMillis(250);
            oi_update(sensor_data);
            reversed += sensor_data->distance; // note: distance is negative while reversing
        }

        oi_setWheels(0, 0);
//        Replaced with loglevel()
//        lcd_printf("No object found.");
//        uart_sendStr("GroBro: No object found in aisle.\r\n");
        loglevel(LCD, EVENT, "No object found.");
        loglevel(PUTTY, EVENT, "GroBro: No object found in aisle.");
    }
}

