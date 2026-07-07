#include <stdbool.h>

/* Window configs */
#define HEIGHT              21
#define WIDTH               60

/* Chars config */
#define BALLCHARTYPE        48
#define SPACECHARTYPE       32
#define RBORDERCHARTYPE     33
#define LBORDERCHARTYPE     124
#define TBORDERCHARTYPE     45
#define BBORDERCHARTYPE     126

/* Paddle cnfig */
#define PADDLECHARTYPE      61
#define PADDLEWIDTH         07

/* Controls config */

/** Uses vi motions like 'l' for right and 'h' for left **/

#define TORIGHT           108
#define TOLEFT            104

#define REFRESHTERM() \
    (printf("\033[H"))

#define __plotObj__(x, y, type) \
   (Window[(y * WIDTH) + (x)] = (type))

void createWindow(void);
void renderWindow(void);
void initPong(void);
bool isNextValid(void);
bool isCollided(void);
void moveBall(void);
void processInputs(char input);

__attribute__((always_inline)) static inline int fractionOf(int length, float limit)
{
    return (int) length * limit;
}
