#include <final_project/api/init/init.h>

#include <lab/log/log.h>
#include <lab/UART-interrupt/uart-interrupt.h>
#include <lab/adc.h>
#include <lab/buttons/button.h>
#include <lab/servo/ping/ping.h>
#include <lab/servo/servo.h>

volatile int mcu_initialized = 0;
volatile int oi_initialized = 0;
oi_t* oi;

void api_init_mcu()
{
    // UART
    uart_interrupt_init();
    // LCD
    lcd_init();
    loglevel(PUTTY, INIT, "Initialized UART and LCD");

    // Timer
    loglevel(PUTTY, INIT, "Initializing Timer...");
    timer_init();
    loglevel(PUTTY, INIT, "Done");

    // Button
    loglevel(PUTTY, INIT, "Initializing button...");
    button_init();
    loglevel(PUTTY, INIT, "Done");

    // ADC
    loglevel(PUTTY, INIT, "Initializing ADC...");
    adc_init();
    loglevel(PUTTY, INIT, "Done");

    // PING
    loglevel(PUTTY, INIT, "Initializing PING...");
    ping_init();
    loglevel(PUTTY, INIT, "Done");

    // SERVO
    loglevel(PUTTY, INIT, "Initializing servo...");
    servo_init();
    servo_move(90);
    loglevel(PUTTY, INIT, "Done");

    mcu_initialized = 1;
    loglevel(PUTTY, INFO, "Finished initializing mcu");
    log_lcd("Finished initializing mcu");
}

void api_init_oi() {
    oi = oi_alloc();
    oi_init(oi);
    loglevel(PUTTY, INIT, "Initialized open interface");
    oi_setWheels(0, 0);
    oi_initialized = 1;
    loglevel(PUTTY, INFO, "Done initializing open interface");
}

void api_init() {
    api_init_mcu();
    api_init_oi();
}

void api_terminate() {
    oi_setWheels(0, 0);
    oi_free(oi);
    oi_initialized = 0;
    loglevel(PUTTY, INIT, "Freed open interface");

    servo_move(90);
    loglevel(PUTTY, INIT, "Done terminating");
}
