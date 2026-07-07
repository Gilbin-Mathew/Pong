#include <stdio.h>
#include "pong.h"

/* This file is just a helper for the gameover matrix generation, to make things easy */

int main(void)
{
    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {
            int index = (y * WIDTH + x);
            if (x == 0 || x == WIDTH - 1) {
                printf("%d, ", VBORDERCHARTYPE);
            }
            else if (y == 0) {
                printf("%d, ", TBORDERCHARTYPE);
            }
            else if (y == HEIGHT - 1) {
                printf("%d, ", BBORDERCHARTYPE);
            }
            else {
                printf("%d, ", SPACECHARTYPE);
            }
        }
        putchar('\n');
    }
}
