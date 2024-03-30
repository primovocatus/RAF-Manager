#include "lib/delete.h"

Delete::Delete(const int& windowWidth, const int& windowHeight) {
    int leftShift = (windowWidth + 2) / 3;
    int topShift = (windowHeight - 5) / 2;
    
    coordinateY = topShift;
    coordinateX = leftShift;

    width = windowWidth / 3;
    height = 6;
    
    window = newwin(height, width, coordinateY, coordinateX);
}

void Delete::printButton(const int& positionX, const int& positionY, 
            const bool& buttonType, const bool& isPointer) {
    std::string button = "[ ";

    if(buttonType) {
        button += "Yes";
    } else {
        button += "No";
    }

    button += " ]";

    if(isPointer) {
        attron(A_REVERSE);
    }

    mvwprintw(stdscr, positionY, positionX, button.data());

    if(isPointer) {
        attroff(A_REVERSE);
    }
}

#include <iostream>
bool Delete::print(const bool& isDirectory, const std::string& fileName, 
    const std::string& path) {
    if(fileName == "..") {
        return 0;
    }

    box(window, 0, 0);

    std::string name = " Delete ";
    mvwprintw(stdscr, coordinateY, coordinateX + 1, name.data());

    std::string deleteName = "Delete ";

    if(isDirectory) {
        deleteName += "directory";
    } else {
        deleteName += "file";
    }

    int positionX = (width - (int)deleteName.size()) / 2;
    mvwprintw(stdscr, coordinateY + 1, coordinateX + positionX, deleteName.data());

    deleteName = "\"" + fileName + "\"?";

    if((int)deleteName.size() > width - 6) {
        std::string newName;

        for (int i = 0; i < width - 10; ++i) {
            newName += deleteName[i];
        }
        newName += "...\?";

        deleteName = newName;
    }

    positionX = (width - (int)deleteName.size()) / 2;
    mvwprintw(stdscr, coordinateY + 2, coordinateX + positionX, deleteName.data());

    for (int i = 1; i < width - 1; ++i) {
        mvwaddch(stdscr, coordinateY + 3, coordinateX + i, ACS_HLINE);
    }

    int buttonX = coordinateX + (width - 14) / 2;

    button = 1;
    printButton(buttonX, coordinateY + 4, 1, 1);
    printButton(buttonX + 9, coordinateY + 4, 0, 0);
    wrefresh(window);

    while(1) {
        int t = getch();

        bool ex = 0;
        switch (t) {
            case KEY_LEFT:
                button ^= 1;
                break;
            case KEY_RIGHT:
                button ^= 1;
                break;
        }
    
        if(t == 10) {
            break;
        }

        if(t == 27) {
            button = 0;
            break;
        }

        printButton(buttonX, coordinateY + 4, 1, button);
        printButton(buttonX + 9, coordinateY + 4, 0, !button);
    }

    if(button) {
        try {
            std::filesystem::remove_all(path);
            return 1;
        } catch (std::filesystem::filesystem_error& e) {
            return 0;
        }
    }

    return 0;
}