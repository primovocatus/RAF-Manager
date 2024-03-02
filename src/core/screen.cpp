#include <ncurses.h>
#include <iostream>

#include "lib/screen.h"

void clearMain(Window* win) {
    win->writeBox();
    win->printName();
    win->printFiles(); 
    win->refreshWindow();
}

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
    
    find = new Find(mainWindowWidth, mainWindowHeight);
    rename = new Rename(mainWindowWidth, mainWindowHeight);
    
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

    std::string key = keyname(t);

    if (key == "q") {
        Screen::~Screen();
        exit(0);
    }
    
    if (key == "^F") {
        find->print();
        
        std::string query = find->getName();

        if(currentWindow) {
            leftWindow->findFile(query);
        } else {
            rightWindow->findFile(query);
        }

        clearMain(leftWindow);
        clearMain(rightWindow);
    }
    
    if (key == "^R") {
        rename->print();

        std::string query = rename->getName();

        if(!query.empty()) {
            if(currentWindow) {
                leftWindow->renameFile(query);
            } else {
                rightWindow->renameFile(query);
            }
        }

        clearMain(leftWindow);
        clearMain(rightWindow);
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

    if (key == "s" || key == "KEY_DOWN") {
        if(currentWindow) {
            leftWindow->moveCursor(1);
        } else {
            rightWindow->moveCursor(1);
        }
    } 
    if (key == "w" || key == "KEY_UP") {
        if(currentWindow) {
            leftWindow->moveCursor(-1);
        } else {
            rightWindow->moveCursor(-1);
        }
    }
    if(key == "d" || key == "KEY_RIGHT") {
        if(currentWindow) {
            leftWindow->changeDir();
        } else {
            rightWindow->changeDir();
        }
    }
    if (key == "a" || key == "KEY_LEFT") {
        if(currentWindow) {
            leftWindow->leaveDir();
        } else {
            rightWindow->leaveDir();
        }
    }
}