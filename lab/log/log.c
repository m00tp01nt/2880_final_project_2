#include <lab/log/log.h>

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
//    log_message(PUTTY, message);
//    log_message(LCD, message);
}

void loglevel(LOG_DEVICE detination, LEVEL level, const char* message, ...) {
    static char buffer[MAX_MESSAGE_LENGTH];
    va_list arglist;
    va_start(arglist, message);
    vsnprintf(buffer, MAX_MESSAGE_LENGTH, message, arglist);
    if (level <= LOG_LEVEL) {
        log_message(detination, buffer);
    }
    va_end(arglist);
}
