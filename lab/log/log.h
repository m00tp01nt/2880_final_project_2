
#ifndef _LOG_H_

#define _LOG_H_

#include <lab/lcd/lcd.h>
#include <lab/UART-interrupt/uart-interrupt.h>
#include <string.h>

#include <stdio.h>

#define MAX_MESSAGE_LENGTH 64

typedef enum LOG_DEVICE {
    LCD,
    PUTTY
} LOG_DEVICE;

typedef enum LEVEL {
    ERROR = 0,
    EVENT = 1,
    INFO = 2,
    INIT = 3,
    DATA = 4
} LEVEL;

#define LOG_LEVEL DATA

void log_message(LOG_DEVICE device, char* message);
void log_putty(char* message);
void log_lcd(char* message);
void loga(char* message);

void loglevel(LOG_DEVICE, LEVEL, const char*, ...);

#endif
