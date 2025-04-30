#ifndef API_INIT_H_
#define API_INIT_H_

#include <stdint.h>
#include <lab/oi/open_interface.h>

extern volatile int mcu_initialized;
extern volatile int oi_initialized;

extern oi_t* oi;

void api_init_mcu(void);
void api_init_oi(void);
void api_init(void);

void api_terminate();

#endif
