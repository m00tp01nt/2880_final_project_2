#include <final_project/api/music/music.h>
#include <final_project/api/init/init.h>

#include <lab/log/log.h>
#include <lab/oi/open_interface.h>

void api_song(SONG song) {
    oi_play_song(song);
}

