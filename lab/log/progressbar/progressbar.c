#include <lab/log/progressbar/progressbar.h>

#include <lab/lcd/lcd.h>
#include <lab/log/log.h>

#define LCD_WIDTH 20

void progressbar_show(double progress) {

    static int previousSize = -1;
    static char progressbar[LCD_WIDTH + 1];

    if ((progress < 0) || (progress > 1)) {
        loglevel(PUTTY, ERROR, "Received a bad progress value");
        return;
    }

    int barSize = (progress * LCD_WIDTH);

    if (barSize == previousSize) return;
    else previousSize = barSize;

    int i = 0;
    for (; i < LCD_WIDTH; i++) {
        if (i < barSize) progressbar[i] = '#';
        else progressbar[i] = '-';
    }

    progressbar[LCD_WIDTH] = '\0';

    log_message(LCD, progressbar);
}
