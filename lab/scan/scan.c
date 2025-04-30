#include <string.h> // sprintf()

#include <math.h>  // abs()

#include <cyBot_Scan.h>
#include <lab/UART-interrupt/uart-interrupt.h>

#include <lab/scan/scan.h>
#include <lab/servo/ir/ir.h>
#include <lab/adc.h>
#include <lab/log/log.h>

#include <lab/movement/movement.h>
#include <lab/movement/bump/bump_handlers.h>

#define MAX 50
#define MIN -50

object findThinnestObject(cyBOT_Scan_t* scanner, int left, int right, int increment, int readsPerAngle) {

    char message[50];

    object objects[10];
    int objectCounter = 0;
    object smallestObject;
    smallestObject.startAngle = 99999;
    smallestObject.endAngle = 0;
    smallestObject.distance = 99999;

    int i;

    int scansPerAngle = 0;

    float pingDistance;
    float irDistance;

    // Bool
    int trackingObject = 0;

    cyBOT_Scan(right, scanner);
    timer_waitMillis(1000);

    for (i = 0; i < 100; i++) { adc_read(); }

    for (i = right; i < left; i += increment)
    {
        if (command_flag == 5) {
            return;
        }
        cyBOT_Scan(i, scanner);

        // Average the data over multiple reads
        pingDistance = 0;
        irDistance = 0;
        float buffer;
        for (scansPerAngle = 0; scansPerAngle < readsPerAngle; scansPerAngle++) {
            pingDistance += scanner->sound_dist;
            buffer = calculateDistance(adc_read());
            if (buffer < 75) {
                irDistance += buffer;
            }
            else {
                irDistance += 200;
            }
        }
        pingDistance /= readsPerAngle;
        irDistance /= readsPerAngle;

        sprintf(message, "DISTANCE : %f", irDistance);
        log_message(PUTTY, message);

        // If we see something between 50cm and 20cm, we've found an object and should start tracking it
        if ((irDistance < MAX) && (irDistance > MIN) && (!trackingObject)) {
            loga("Start Object");
            trackingObject = 1;
            objects[objectCounter].startAngle = i;
        }
        if (((irDistance > MAX) || (irDistance < MIN)) && (trackingObject)) {
            loga("End Object");
            trackingObject = 0;
            objects[objectCounter].endAngle = (i - 1);

            if (abs(objects[objectCounter].startAngle - objects[objectCounter].endAngle) > 5) {
                objectCounter++;
            }
        }
    }

    sprintf(message, "Found %d objects", objectCounter);
    loga(message);



    for (objectCounter--; objectCounter >= 0; objectCounter--)
    {
        if (command_flag == 5) {
            return;
        }
        sprintf(message, "Start : %d -- End : %d", objects[objectCounter].startAngle, objects[objectCounter].endAngle);
        log_message(PUTTY, message);
        objects[objectCounter].midpoint = ((objects[objectCounter].startAngle + objects[objectCounter].endAngle) / 2);

        cyBOT_Scan(objects[objectCounter].midpoint, scanner);
        timer_waitMillis(500);
        objects[objectCounter].distance = calculateDistance(adc_read());

        sprintf(message, "MIDPOINT : %d", objects[objectCounter].midpoint);
        log_message(PUTTY, message);
        sprintf(message, "WIDTH : %f", calculateAngularWidth(objects[objectCounter]));
        log_message(PUTTY, message);

        objects[objectCounter].width = calculateAngularWidth(objects[objectCounter]);

        if (calculateAngularWidth(objects[objectCounter]) < calculateAngularWidth(smallestObject)) {
            smallestObject.startAngle = objects[objectCounter].startAngle;
            smallestObject.endAngle = objects[objectCounter].endAngle;
            smallestObject.midpoint = objects[objectCounter].midpoint;
            smallestObject.distance = objects[objectCounter].distance;
            smallestObject.width = objects[objectCounter].width;
        }

        sprintf(message, "OBJECT #%d", objectCounter);
        log_message(PUTTY, message);

        printObject(&objects[objectCounter]);

        timer_waitMillis(1000);
    }


    sprintf(message, "MIDPOINT : %d", smallestObject.midpoint);
    log_message(PUTTY, message);
    cyBOT_Scan(smallestObject.midpoint, scanner);
    loga("Pointing at thinnest object\0");

    return smallestObject;
}


float calculateAngularWidth(object obj) {
    return 2 * 3.14159265358979323846 * obj.distance * (abs(obj.startAngle - obj.endAngle) / (float)360);
}

void initObject(object* o) {
    o->startAngle = 0;
    o->endAngle = 0;
    o->midpoint = 0;
    o->distance = 0.0;
    o->width = 0.0;
}

void printObject(object* o)
{
    char message[40];

    sprintf(message, "OBJECT DISTANCE : %f", o->distance);
    log_message(PUTTY, message);

    sprintf(message, "OBJECT WIDTH : %f", o->width);
    log_message(PUTTY, message);

    sprintf(message, "OBJECT MIDPOINT : %d", o->midpoint);
    log_message(PUTTY, message);

    return;
}
