#ifndef API_BUTTON_H_
#define API_BUTTON_H_

#include <stdint.h>

void api_button_await(uint8_t);
void api_button_onpress(uint8_t exit, void(*callback)(uint8_t));

#endif
