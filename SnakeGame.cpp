#include <iostream>
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
#include <sys/ioctl.h>

bool gameOver;
const int width = 20;
const int height = 10;
int x, y, targetx, targety, score;
enum direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
direction dir;

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

int setup() {
    gameOver = false;
    dir = STOP;
    targetx = rand() % width;
    targety = rand() % height;
    x = width/2;
    y = height/2;
    score = 0;
}

int draw() {
    system("clear");
    for(int i=0; i<width; i++)
        std::cout<<'#';
    std::cout<<std::endl;

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            if(j==0)
                std::cout<<'#';
            
            if(j==x && i==y)
                std::cout<<'o';

            if(j==targetx && i==targety)
                std::cout<<'F';

            if(j==width-2)
                std::cout<<'#';
            
            std::cout<<' ';
        }
        std::cout<<std::endl;
    }

    for(int i=0; i<width; i++)
        std::cout<<'#';
    std::cout<<std::endl;
}

void input() {
    if(_kbhit()) {
        switch(getchar()) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

void logic() {
    switch (dir) {
        case RIGHT:
            x++;
            break;
        case LEFT:
            x--;
            break;
        case UP:
            y++;
            break;
        case DOWN:
            y--;
            break;
        default:
            break;
    }
    if(x>width || x<0 || y>height || y<0) {
        gameOver = true;
    }

    if(x==targetx && y==targety) {
        score +=10;
            targetx = rand() % width;
            targety = rand() % height;
    }
}

int main() {
    setup();
    while(!gameOver) {
        draw();
        input();
        logic();
    }
}