#include <ncurses.h>
#include <iostream>

#include "lib/screen.h"

struct passwd *pw = getpwuid(getuid());
const char *homeDir = pw->pw_dir;

void clearMain(Window* win) {
    win->writeBox();
    win->printName();
    win->printFiles(); 
    win->refreshWindow();
}

Screen::Screen() {
    initscr();
    start_color();

    noecho();
    curs_set(0);

    keypad(stdscr, TRUE);

    cbreak();

    mainWindowWidth = getmaxx(stdscr);
    mainWindowHeight = getmaxy(stdscr) - 1;

    leftWindowWidth = (mainWindowWidth + 1) / 2;
    rightWindowWidth = mainWindowWidth / 2;

    leftWindow = new Window(0, 0, leftWindowWidth, mainWindowHeight, homeDir);
    rightWindow = new Window(0, leftWindowWidth, rightWindowWidth, mainWindowHeight, homeDir);

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

    switch (t) {
        //tab key
        case 9:
            currentWindow = !currentWindow;
            leftWindow->isFocused = !leftWindow->isFocused;
            rightWindow->isFocused = !rightWindow->isFocused;

            leftWindow->printName();
            leftWindow->printFiles();

            rightWindow->printName();
            rightWindow->printFiles();
            break;

        //enter key
        case 10:
            if(currentWindow) {
                leftWindow->changeDir();
            } else {
                rightWindow->changeDir();
            }
            break;

        case KEY_DOWN:
            if(currentWindow) {
                leftWindow->moveCursor(1);
            } else {
                rightWindow->moveCursor(1);
            }
            break;

        case KEY_UP:
            if(currentWindow) {
                leftWindow->moveCursor(-1);
            } else {
                rightWindow->moveCursor(-1);
            }
            break;
    }

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
    }
}