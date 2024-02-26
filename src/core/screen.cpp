#include <ncurses.h>
#include <iostream>

#include "lib/screen.h"

Screen::Screen() {
    initscr();

    noecho();
    curs_set(0);

    keypad(stdscr, TRUE);

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

Screen::~Screen() {
    endwin();
}

void Screen::listener() const {
    move(mainWindowHeight, 0);
    int t = getch();

    // std::cout<< t << std::endl;

    if (t == 113) {
        Screen::~Screen();
        exit(0);
    }

    if (t == 115 || t == 258) {
        leftWindow->moveCursor(1);
    } 
    if (t == 119 || t == 259) {
        leftWindow->moveCursor(-1);
    }
    if(t == 100 || t == 261) {
        leftWindow->changeDir();
    }
    if (t == 97 || t == 260) {
        leftWindow->leaveDir();
    }
}