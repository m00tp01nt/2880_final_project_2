#ifndef FINAL_PROJECT_API_CLIFF_CLIFF_H_
#define FINAL_PROJECT_API_CLIFF_CLIFF_H_

typedef enum CLIFF {
    CLIFF_NONE,
    CLIFF_LEFT,
    CLIFF_FRONT_LEFT,
    CLIFF_FRONT_RIGHT,
    CLIFF_RIGHT
} CLIFF;

CLIFF checkCliff(void);

#endif
