#include "pong.h"

#define PONG_GAME_OVER
#include "gameover.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

struct termios PreTermState;

int16_t CurrentDirection;
int16_t DeltaX;
int16_t DeltaY;
int32_t CurrentBallYIndex;
int32_t CurrentBallXIndex;
bool GameOver;
int16_t PaddleIndex;

char Window[HEIGHT * WIDTH];

__attribute__((always_inline)) static inline int nextIndex(void)
{
    int nextXIndex = CurrentBallXIndex + DeltaX;
    int nextYIndex = CurrentBallYIndex + DeltaY;
    return nextYIndex * WIDTH + nextXIndex;
}

static void setTerm(void)
{
    struct termios CustomTermState;

      int fd=fileno(stdin);
      tcgetattr(fd, &PreTermState);

      CustomTermState=PreTermState;
      CustomTermState.c_lflag &= ~(ICANON|ECHO);

      tcsetattr(fd,TCSANOW,&CustomTermState);
      fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0)|O_NONBLOCK);

      return;
}

static void quiteTerm(void)
{
    printf("\033[?25h");
    fflush(stdout);

    tcsetattr(STDOUT_FILENO, TCSAFLUSH, &PreTermState);
}

static char pressedKey(void)
{
    char keyin = 0;
    char discard;
    
    while(read(STDIN_FILENO, &discard, 1) > 0) {
        keyin = discard;
    }
    return keyin;
}

int main(void)
{
    initPong();
    while (!GameOver) {
        processInputs(pressedKey());
        moveBall();
        createWindow();
        renderWindow();
        REFRESHTERM();
        usleep(100000);
    }
    if (GameOver) {
        REFRESHTERM();
        for(int y = 0; y < HEIGHT; y++) {
            for(int x = 0; x < WIDTH; x++) {
                putchar(GameOverMatrix[(y * WIDTH) + x]);
            }
            putchar('\n');
        }
    }
    quiteTerm();
    return 0;
}

void initPong(void)
{
    printf("\033[2J \033[H \033[?25l");      /* clear terminal && moves the cursor to home && hides the cursor*/
    fflush(stdout);
    setTerm();
    GameOver = false;
    PaddleIndex = ((HEIGHT - 2) * WIDTH) + ((WIDTH / 2) - PADDLEWIDTH);
    CurrentDirection = -1;
    DeltaX = -1;
    DeltaY = -1;
    CurrentBallYIndex = HEIGHT - 3;
    CurrentBallXIndex = WIDTH / 2;
    return;
}

void processInputs(char input)
{
    switch (input) {
        case TORIGHT:
            if ( Window[PaddleIndex + PADDLEWIDTH + 2] == LBORDERCHARTYPE){
                PaddleIndex = PaddleIndex;
            }
            else {
                PaddleIndex = PaddleIndex + 2;
            }
            break;
        case TOLEFT:
            if (Window[PaddleIndex - 2] == RBORDERCHARTYPE) {
                PaddleIndex = PaddleIndex - 2;
            }
            else {
                PaddleIndex = PaddleIndex - 2;
            }
            break;
        case 'q':
            GameOver = true;
        default:
            break;
    }
}

void createWindow(void)
{
    for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < WIDTH; x++){
            int index = (y * WIDTH + x);
            int ballIndex = CurrentBallYIndex * WIDTH + CurrentBallXIndex;
            if (x == 0) {
                __plotObj__(x,  y, LBORDERCHARTYPE);
            }
            else if (x == WIDTH - 1) {
                __plotObj__(x,  y, RBORDERCHARTYPE);
            }
            else if (y == 0) {
                    __plotObj__(x,  y, TBORDERCHARTYPE);
            }
            else if (y == HEIGHT - 1) {
                __plotObj__(x, y, BBORDERCHARTYPE);
            }
            else if ( index >= PaddleIndex && index <= PaddleIndex + PADDLEWIDTH) {
                    __plotObj__(x,  y, PADDLECHARTYPE);
            }
            else if (ballIndex == index) {
                __plotObj__(x, y, BALLCHARTYPE);
            }
            else {
                Window[index] = SPACECHARTYPE;
            }
        }
    }
    return;
}

void renderWindow(void)
{
    for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < WIDTH; x++){
                putchar(Window[y * WIDTH + x]);
        }
        putchar('\n');
    }
    return;
}

void moveBall(void)
{
    if (isNextValid()) {
        CurrentBallXIndex += DeltaX;
        CurrentBallYIndex += DeltaY;

    }
    else {
        isCollided();
    }
    return;
}

bool isOutOfBound(void)
{
    int nextX = CurrentBallXIndex + DeltaX;
    int nextY = CurrentBallYIndex + DeltaY;

    if (nextX < 0 || nextX >= WIDTH || nextY < 0 || nextY >= HEIGHT) {
        return true;
    }
    return false;
}

bool isNextValid(void)
{
    if (isOutOfBound()) {
        return false;
    }

    int index = nextIndex();
    uint8_t block = Window[index];

    return block != LBORDERCHARTYPE &&
        block != RBORDERCHARTYPE &&
        block != TBORDERCHARTYPE &&
        block != BBORDERCHARTYPE &&
        block != PADDLECHARTYPE;
}

bool isCollided(void)
{
    if (isNextValid()) {
        return false;
    }

    int index = nextIndex();
    uint8_t block = Window[index];

    switch (block) {
        case LBORDERCHARTYPE:
            DeltaX = -(DeltaX);
            return true;
            break;
        case RBORDERCHARTYPE:
            DeltaX = -(DeltaX);
            return true;
            break;
        case TBORDERCHARTYPE:
            DeltaY = -(DeltaY);
            return true;
            break;
        case BBORDERCHARTYPE:
            GameOver = true;
            break;
        case PADDLECHARTYPE:
            DeltaY = -(DeltaY);
            return true;
            break;
        default:
            return false;
            break;
    }
    return false;
}
