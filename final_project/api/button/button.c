#include <final_project/api/button/button.h>

#include <lab/log/log.h>
#include <lab/buttons/button.h>

void api_button_await(uint8_t button) {
    loglevel(PUTTY, EVENT, "Waiting for button %d...", button);
    waitForButton(button);
    loglevel(PUTTY, DATA, "Button %d pressed", button);
}

void api_button_onpress(uint8_t exit, void (*event)(uint8_t)) {
    int button;
    loglevel(PUTTY, INFO, "Waiting for button press (Exiting on %d)", exit);
    while (1) {
        button = button_getButton();
        if (button == exit) break;
        else if (button) {
            loglevel(PUTTY, DATA, "Button %d pressed", button);
            event(button);
            while (button_getButton() == button) {}
        }
    }
}
