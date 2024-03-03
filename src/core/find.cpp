#include "lib/find.h"

Find::Find(const int& windowWidth, const int& windowHeight) {
    int leftShift = (windowWidth + 2) / 3;
    int topShift = (windowHeight - 5) / 2;
    
    coordinateY = topShift;
    coordinateX = leftShift;

    width = windowWidth / 3;
    height = 5;
    
    window = newwin(height, width, coordinateY, coordinateX);
}

void Find::clear() const {
    std::string clr;

    clr.resize(width, ' ');
    for (int i = coordinateY; i < coordinateY + 5; ++i) {
        mvwprintw(stdscr, i, coordinateX, clr.data());
    }

    wrefresh(window);
}

void Find::print() const {
    box(window, 0, 0);
    
    std::string name = " Find file ";
    mvwprintw(stdscr, coordinateY, coordinateX + 1, name.data());

    wrefresh(window);   
}

std::string Find::getName() {
    std::string query;

    while(1) {
        std::string currentQuery = query;
        
        if((int)currentQuery.size() > width - 5) {
            std::string temp;
            for (int i = (int)currentQuery.size() - width + 4; i < (int)currentQuery.size(); ++i) {
                temp += currentQuery[i];        
            }
            currentQuery = temp;
        }

        while((int)currentQuery.size() < width - 5) {
            currentQuery += " ";
        }

        mvwprintw(stdscr, coordinateY + 2, coordinateX + 2, currentQuery.data());

        int t = getch();

        if (t == 10) {
            return query;
        } else if (t == 27) {
            return "";
        } else if (t == KEY_BACKSPACE) {
            if(!query.empty()) {
                query.erase(query.size() - 1);
            }

            continue;
        }

        query += keyname(t);
    }

    return query;
}