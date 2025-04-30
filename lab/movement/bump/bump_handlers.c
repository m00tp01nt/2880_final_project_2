#include <lab/movement/bump/bump_handlers.h>

void doNothing(oi_t* sensor_data, BUMP* bumpData, double* sum) { return; }

void handleBump(oi_t* sensor_data, BUMP* bumpData, double* sum) {

    *sum -= move(sensor_data, 15, BACKWARD, doNothing);

    if (*bumpData == BUMP_LEFT) {
        log_putty("Handling Left Bump\0");
        rotate(sensor_data, 90, RIGHT, doNothing);
        move(sensor_data, 25, FORWARD, doNothing);
        rotate(sensor_data, 90, LEFT, doNothing);
    }
    else if (*bumpData == BUMP_RIGHT) {
        log_putty("Handling Right Bump\0");
        rotate(sensor_data, 90, LEFT, doNothing);
        move(sensor_data, 25, FORWARD, doNothing);
        rotate(sensor_data, 90, RIGHT, doNothing);
    }

    oi_setWheels(0, 0);

    log_putty("Done Handling\0");
}

void moveToAnotherSpot(oi_t* sensor_data, BUMP* bumpData, double* sum)
{
    move(sensor_data, 25, BACKWARD, doNothing);

    if (*bumpData == BUMP_LEFT) {
        log_message(PUTTY, "Handling Left Bump\0");

        rotate(sensor_data, 90, RIGHT, doNothing);
        move(sensor_data, 250, FORWARD, doNothing);
        rotate(sensor_data, 90, LEFT, doNothing);
        move(sensor_data, 250, FORWARD, doNothing);
        rotate(sensor_data, 90, LEFT, doNothing);
        move(sensor_data, 250, FORWARD, doNothing);
        rotate(sensor_data, 90, LEFT, doNothing);
    }
    else if (*bumpData == BUMP_RIGHT) {
        log_message(PUTTY, "Handling Right Bump\0");

        rotate(sensor_data, 90, LEFT, doNothing);
        move(sensor_data, 250, FORWARD, doNothing);
        rotate(sensor_data, 90, RIGHT, doNothing);
        move(sensor_data, 250, FORWARD, doNothing);
        rotate(sensor_data, 90, RIGHT, doNothing);
        move(sensor_data, 250, FORWARD, doNothing);
        rotate(sensor_data, 90, RIGHT, doNothing);
    }
}
