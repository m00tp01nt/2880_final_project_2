#include <stdint.h> // uint8_t

#include <final_project/api/cliff/cliff.h>

#include <lab/log/log.h>
#include <final_project/api/init/init.h> // Get the global OI object
#include <lab/oi/open_interface.h> // oi_update()
#include <final_project/api/music/music.h> // Songs

CLIFF api_checkCliff(uint8_t oiUpdated, uint8_t playSong) {
    if (!oiUpdated) {
        loglevel(PUTTY, INFO, "Updating open interface...");
        oi_update(oi);
        loglevel(PUTTY, INFO, "Done");
    }
    if (oi->cliffLeft) {
        api_song_problem();
        loglevel(PUTTY, EVENT, "Cliff Left detected");
        return CLIFF_LEFT;
    }
    else if (oi->cliffFrontLeft) {
        api_song_problem();
        loglevel(PUTTY, EVENT, "Cliff Front Left detected");
        return CLIFF_FRONT_LEFT;
    }
    else if (oi->cliffFrontRight) {
        api_song_problem();
        loglevel(PUTTY, EVENT, "Cliff Front Right detected");
        return CLIFF_FRONT_RIGHT;
    }
    else if (oi->cliffRight) {
        api_song_problem();
        loglevel(PUTTY, EVENT, "Cliff Right detected");
        return CLIFF_RIGHT;
    }
    loglevel(PUTTY, DATA, "No Cliff detected");
    return CLIFF_NONE;
}

CLIFF api_just_checkCliff() {
    api_checkCliff(0, 1);
}
