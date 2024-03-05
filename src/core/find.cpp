#include "lib/find.h"

Find::Find(const int& windowWidth, const int& coordinateY) {
    Find::coordinateY = coordinateY;
    coordinateX = 0;

    width = windowWidth - 2;
    height = 1;
}

void Find::clear(WINDOW* win) const {
    for (int i = coordinateX + 1; i < coordinateX + width + 1; ++i) {
        mvwaddch(win, coordinateY, i, ' ');
    }
    wrefresh(win);
}