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

unsigned char song_c_major_notes[SONG_LENGTH] = { 60, 64, 67, 72 };
unsigned char song_f_major_notes[SONG_LENGTH] = { 60, 65, 69, 72 };
unsigned char song_a_minor_notes[SONG_LENGTH] = { 64, 69, 72, 76 };
unsigned char song_g_major_notes[SONG_LENGTH] = { 62, 67, 71, 74 };
unsigned char song_note_length[SONG_LENGTH]   = { NOTE_LENGTH, NOTE_LENGTH, NOTE_LENGTH, NOTE_LENGTH };

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
    loglevel(PUTTY, INIT, "Initializing open interface...");
    oi = oi_alloc();
    oi_init(oi);
    loglevel(PUTTY, INIT, "Done");
    oi_setWheels(0, 0);
    oi_initialized = 1;
    loglevel(PUTTY, INFO, "Finished initializing open interface");
}

void api_init_oi_songs() {
    loglevel(PUTTY, INIT, "Loading the songs into open interface...");

    oi_loadSong(START, SONG_LENGTH, song_c_major_notes, song_note_length);
    oi_loadSong(ITEM_FOUND, SONG_LENGTH, song_f_major_notes, song_note_length);
    oi_loadSong(PROBLEM, SONG_LENGTH, song_a_minor_notes, song_note_length);
    oi_loadSong(ARRIVED, SONG_LENGTH, song_g_major_notes, song_note_length);

    loglevel(PUTTY, INIT, "Done");
    loglevel(PUTTY, INFO, "Songs are loaded into open interface");
}

void api_init() {
    api_init_mcu();
    api_init_oi();
    api_init_oi_songs();
}

void api_terminate() {
    oi_setWheels(0, 0);
    oi_free(oi);
    oi_initialized = 0;
    loglevel(PUTTY, INIT, "Freed open interface");

    servo_move(90);
    loglevel(PUTTY, INIT, "Done terminating");
}
