#include <ncurses.h>

#include "lib/screen.h"

Screen::Screen() {
    initscr();

    echo();
    curs_set(0);

    cbreak();

    mainWindowWidth = getmaxx(stdscr);
    mainWindowHeight = getmaxy(stdscr) - 1;

    leftWindowWidth = (mainWindowWidth + 1) / 2;
    rightWindowWidth = mainWindowWidth / 2;

    leftWindow = new Window(0, 0, leftWindowWidth, mainWindowHeight);
    rightWindow = new Window(0, leftWindowWidth, rightWindowWidth, mainWindowHeight);
    refresh();

    leftWindow->writeBox();
    leftWindow->printName();
    leftWindow->printFiles();
    leftWindow->refreshWindow();

    rightWindow->writeBox();
    rightWindow->printName();
    rightWindow->refreshWindow();
}

void Screen::listener() const {
    move(mainWindowHeight, 0);
    char t = getch();

    if ((int)t == 115) {
        leftWindow->moveCursor(1);
    } 
    if ((int)t == 119) {
        leftWindow->moveCursor(-1);
    }
    if((int)t == 100) {
        leftWindow->changeDir();
    }
    if ((int)t == 97) {
        leftWindow->leaveDir();
    }
}