#ifndef API_INIT_H_
#define API_INIT_H_

#include <stdint.h>
#include <lab/oi/open_interface.h>

extern volatile int mcu_initialized;
extern volatile int oi_initialized;

extern oi_t* oi;

#define SONG_BLOCKING 0

#define SONG_LENGTH 4
#define NOTE_LENGTH 8

extern unsigned char song_c_major_notes[SONG_LENGTH];
extern unsigned char song_f_major_notes[SONG_LENGTH];
extern unsigned char song_a_minor_notes[SONG_LENGTH];
extern unsigned char song_g_major_notes[SONG_LENGTH];
extern unsigned char song_note_length[SONG_LENGTH];

typedef enum SONG {
    START = 0,
    ITEM_FOUND = 1,
    PROBLEM = 2,
    ARRIVED = 3
} SONG;

void api_init_mcu(void);
void api_init_oi(void);
void api_init_oi_songs(void);
void api_init(void);

void api_terminate();

#endif
