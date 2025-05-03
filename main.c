/**
 * main.c
 */

#include <lab/log/log.h>
#include <lab/log/progressbar/progressbar.h>

#include <final_project/api/init/init.h>
#include <final_project/api/button/button.h>
#include <final_project/api/music/music.h>

#include <lab/movement/movement.h>

// Callback
void buttonpress(uint8_t button) {
    api_song((SONG)(button - 1));
}

int main(void)
{
    api_init();

    api_button_onpress(1, buttonpress);

    grobro_scan_and_drive(oi);

    api_terminate();
    loglevel(LCD, EVENT, "Done!");
    loglevel(PUTTY, EVENT, "Terminated");
    return 0;
}
