

/**
 * main.c
 */

#include <lab/log/log.h>
#include <lab/log/progressbar/progressbar.h>

#include <final_project/api/init/init.h>
#include <final_project/api/button/button.h>
#include <final_project/api/move/move.h>

// Callback
void buttonpress(uint8_t button) {
    if (button == 2) {
        oi_setWheels(-50, 50);
    }
    else if (button == 3) {
        oi_setWheels(100, 100);
    }
    else if (button == 4) {
        oi_setWheels(50, -50);
    }
}

void bumpHandler(BUMP* data) {
    loglevel(PUTTY, INFO, "Inside bump handler");
    oi_setWheels(0, 0);
    return;
}

int main(void)
{
    api_init();

    api_button_onpress(1, buttonpress);

    api_move(250, FORWARD, bumpHandler);

    api_terminate();
    loglevel(LCD, EVENT, "Done!");
    loglevel(PUTTY, EVENT, "Terminated");
    return 0;
}
