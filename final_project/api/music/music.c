#include <final_project/api/music/music.h>
#include <final_project/api/init/init.h>

#include <lab/log/log.h>
#include <lab/oi/open_interface.h>
#include <lab/timer/Timer.h>

#include <math.h> // round()

void api_song(SONG song) {
    loglevel(PUTTY, INFO, "Playing song %d", song);
    oi_play_song(song);
    if (SONG_BLOCKING) {
        loglevel(PUTTY, INFO, "Waiting for song to finish...");
        timer_waitMillis(SONG_LENGTH * round((float)NOTE_LENGTH / 64) * 1000);
        loglevel(PUTTY, INFO, "Done");
    }
}

void api_song_start() {
    api_song(START);
}
void api_song_item_found() {
    api_song(ITEM_FOUND);
}
void api_song_problem() {
    api_song(PROBLEM);
}
void api_song_arrived() {
    api_song(ARRIVED);
}
