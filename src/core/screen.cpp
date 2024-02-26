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

    currentWindow = 1;
    leftWindow->isFocused = 1;

    leftWindow->writeBox();
    leftWindow->printName();
    leftWindow->printFiles();
    leftWindow->refreshWindow();

    rightWindow->writeBox();
    rightWindow->printName();
    rightWindow->printFiles();
    rightWindow->refreshWindow();
}

Screen::~Screen() {
    endwin();
}

void Screen::listener() {
    move(mainWindowHeight, 0);
    int t = getch();

    // std::cout<< t << std::endl;

    if (t == 113) {
        Screen::~Screen();
        exit(0);
    }

    if (t == 9) {
        currentWindow = !currentWindow;
        leftWindow->isFocused = !leftWindow->isFocused;
        rightWindow->isFocused = !rightWindow->isFocused;

        leftWindow->printName();
        leftWindow->printFiles();

        rightWindow->printName();
        rightWindow->printFiles();
    }

    if (t == 115 || t == 258) {
        if(currentWindow) {
            leftWindow->moveCursor(1);
        } else {
            rightWindow->moveCursor(1);
        }
    } 
    if (t == 119 || t == 259) {
        if(currentWindow) {
            leftWindow->moveCursor(-1);
        } else {
            rightWindow->moveCursor(-1);
        }
    }
    if(t == 100 || t == 261) {
        if(currentWindow) {
            leftWindow->changeDir();
        } else {
            rightWindow->changeDir();
        }
    }
    if (t == 97 || t == 260) {
        if(currentWindow) {
            leftWindow->leaveDir();
        } else {
            rightWindow->leaveDir();
        }
    }
}