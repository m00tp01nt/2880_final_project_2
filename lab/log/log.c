#include <lab/log/log.h>
#include <string.h>

#define ENABLE_LOGS 1

void log_message(LOG_DEVICE device, char* message)
{
    if (!ENABLE_LOGS) return;

    if (device == LCD)
    {
        lcd_clear();
        lcd_printf(message);
    }
    else if (device == PUTTY)
    {
        uart_sendStr(message);
        uart_sendStr("\r\n");
    }
}

void log_putty(char* message) {
    log_message(PUTTY, message);
}

void log_lcd(char* message) {
    log_message(LCD, message);
}

// Could really screw things up if timing is super important
void loga(char* message)
{
    loglevel(PUTTY, INFO, message);
    loglevel(LCD, INFO, message);
}

void loglevel(LOG_DEVICE destination, LEVEL level, const char* message, ...) {
    char buffer[MAX_MESSAGE_LENGTH];

    // Adapted from lcd.c/.h >>>
    va_list arglist;
    va_start(arglist, message);
    vsnprintf(buffer, MAX_MESSAGE_LENGTH, message, arglist);
    // <<<

    if (level <= LOG_LEVEL) {
        if (destination == PUTTY) {
            char type[10 + MAX_MESSAGE_LENGTH];
            type[0] = '\0';
            if (level == ERROR)         strcat(type, "[ERROR]\t");
            else if (level == EVENT)    strcat(type, "[EVENT]\t");
            else if (level == INFO)     strcat(type, "[INFO]\t");
            else if (level == INIT)     strcat(type, "[INIT]\t");
            else if (level == DATA)     strcat(type, "[DATA]\t");
            strcat(type, buffer);
            log_message(PUTTY, type);
        }
        else {
            log_message(destination, buffer);
        }
    }
    va_end(arglist);
}
