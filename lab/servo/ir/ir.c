#include <string.h>

#include <lab/servo/ir/ir.h>
#include <lab/servo/servo.h>
#include <lab/log/log.h>
#include <lab/buttons/button.h>
#include <lab/adc.h>
#include <math.h>

#include "cyBot_Scan.h"

#define LCD_WIDTH 20

//float normalizeData(int value) { return (value / (float)4095) * 3.3; }

float calculateDistance(float data) {
//    return (472.6954 - (60.7899 * log(data)));
//    return (118.766 - (14.7909 * log(data - 1000)));
//    return (127.1268 - (16.1713 * log(data-1000)));
    return (119.1899 - (14.7527* log(data - 1000)));
}

void calibrate_IR() {

    char message[50];
    char loadingBar[25];

    int i;

    long total = 0;
    int sampleSize = 100000;

    cyBOT_Scan_t scanner;

    cyBOT_Scan(90, &scanner);

    int current = 0;
    int samples = 9;

    sprintf(loadingBar, "#-------------------");
    int loadingBarTracker = 0;

    for (; current < samples; current++)
    {
        sprintf(loadingBar, "#-------------------");
        loadingBarTracker = 0;

        sprintf(message, "%dcm\nPress Button 1\0", (current * 5) + 10);
        log_message(LCD, message);
        waitForButton(1);

        for (i = 0; i < sampleSize; i++)
        {
            total += adc_read();

            // Loading bar
            if ((loadingBarTracker != round(((float)i / sampleSize) * LCD_WIDTH)) && (loadingBarTracker < LCD_WIDTH))
            {
                loadingBarTracker = round((((float)i / sampleSize) * LCD_WIDTH));
                loadingBar[loadingBarTracker] = '#';
                log_message(LCD, loadingBar);
            }
        }
        total = (double)total / sampleSize;

        sprintf(message, "%dcm average : %ld", (current * 5) + 10, total);
        log_message(PUTTY, message);
    }

    loga("Calibration complete\nPress Button 4\nto exit\0");
    waitForButton(4);

}
