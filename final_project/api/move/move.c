#include <final_project/api/move/move.h>

#include <final_project/api/init/init.h>    // Gloabl oi struct
#include <lab/movement/movement.h>          // Movement enums, bumpData()
#include <lab/log/log.h>

double api_move(double distance_mm, LINEAR_MOVEMENT direction, void (*handler)(BUMP* bumpDirection)) {
    loglevel(PUTTY, EVENT, "Moving %lf mm", distance_mm);

    double lowerBound;
    double upperBound;

    double sum = 0;

    int leftWheelSpeed;
    int rightWheelSpeed;

    // Make sure we're going to go the right way
    if (direction == BACKWARD) {
        distance_mm *= -1;
    }

    lowerBound = doubleMin(distance_mm, distance_mm * -1);
    upperBound = doubleMax(distance_mm, distance_mm * -1);

    // Should we be moving Forwards or Backwards?
    if (distance_mm > 0) {
        rightWheelSpeed = API_LATERAL_SPEED;
        leftWheelSpeed = API_LATERAL_SPEED;
    }
    else if (distance_mm < 0) {
        rightWheelSpeed = -API_LATERAL_SPEED;
        leftWheelSpeed = -API_LATERAL_SPEED;
    }

    oi_setWheels(rightWheelSpeed, leftWheelSpeed);

    BUMP data;

    while (doubleIsBetween(lowerBound, upperBound, sum)) {
        oi_update(oi);
        data = bumpData(oi);
        if (data != BUMP_NONE) {
            loglevel(PUTTY, EVENT, "Bump detected! Code %d", data);
            handler(&data);
            return sum;
        }
        sum += oi->distance;
    }
    oi_setWheels(0, 0);

    loglevel(PUTTY, EVENT, "Done moving");
    return sum;
}
