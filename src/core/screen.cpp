#include <ncurses.h>
#include <iostream>

#include "lib/screen.h"

struct passwd *pw = getpwuid(getuid());
const char *homeDir = pw->pw_dir;

void clearMain(Window* win) {
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

    rename = new Rename(mainWindowWidth, mainWindowHeight);
    del = new Delete(mainWindowWidth, mainWindowHeight);

    refresh();

    currentWindow = 1;
    leftWindow->isFocused = 1;

    clearMain(leftWindow);
    clearMain(rightWindow);
}

Screen::~Screen() {
    endwin();
}

bool Screen::listener() {
    move(mainWindowHeight, 0);
    int t = getch();
    std::string key = keyname(t);

    switch (t) {
        //tab key
        case 9:
            currentWindow = !currentWindow;
            leftWindow->isFocused = !leftWindow->isFocused;
            rightWindow->isFocused = !rightWindow->isFocused;

            leftWindow->printFiles();

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
        case KEY_HOME:
            if(currentWindow) {
                leftWindow->toHome();
            } else {
                rightWindow->toHome();
            }
            break;
        
        case KEY_END:
            if(currentWindow) {
                leftWindow->toEnd();
            } else {
                rightWindow->toEnd();
            }
            break;

        case KEY_DC:
            file currentFile;

            if(currentWindow) {
                currentFile = leftWindow->getFile();
            } else {
                currentFile = rightWindow->getFile();
            }

            if(del->print(currentFile.isDirectory, currentFile.name, currentFile.path)) {
                if(currentWindow) {
                    leftWindow->removeFile(currentFile);
                } else {
                    rightWindow->removeFile(currentFile);
                }
            }

            leftWindow->clearWindow();
            clearMain(leftWindow);

            rightWindow->clearWindow();
            clearMain(rightWindow);
            break;
    }

    if (key == "q") {
        Screen::~Screen();
        exit(0);
    }
    
    if (key == "^F") { 
        if(currentWindow) {
            leftWindow->findFile();
        } else {
            rightWindow->findFile();
        }
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

    if (key == "KEY_RESIZE") {
        return 0;
    }
    return 1;
}