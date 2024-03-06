#include "lib/window.h"

Window::Window() {
    
}

Window::Window(const int& y, const int& x, const int& width, const int& height, const std::string homeDir) {
    coordinateY = y;
    coordinateX = x;

    Window::width = width;
    Window::height = height;

    path = homeDir;

    getFiles(path, dir);

    filesCount = (int)dir.size();
    topFile = 0, bottomFile = std::min(filesCount, height - 4);

    window = newwin(height, width, y, x);

    find = new Find(width, height - 2);

    init_pair(1, 5, 0);

    wattron(window, COLOR_PAIR(1));
}

void Window::clearWindow() const {
    std::string clr;

    clr.resize(width, ' ');
    for (int i = coordinateY; i < coordinateY + height; ++i) {
        mvwprintw(window, i, 0, clr.data());
    }

    refreshWindow();
}

void Window::refreshWindow() const {
    wrefresh(window);
}

void Window::writeBox() const {
    box(window, 0, 0);

    for (int i = coordinateY + 1; i < coordinateY + height - 3; ++i) {
        mvwaddch(window, i, width - 10, ACS_VLINE);
    }

    for (int i = coordinateX + 1; i < coordinateX + width - 1; ++i) {
        mvwaddch(window, height - 3, i - coordinateX, ACS_HLINE);
    }

    refreshWindow();
}

void Window::printName() const {
    if(isFocused) {
        wattron(window, A_REVERSE);
    }

    mvwprintw(window, 0, 1, path.data());
    wattroff(window, A_REVERSE);

    refreshWindow();
}

void Window::printFiles() {
    writeBox();
    printName();

    int currentX = 1;
    int currentY = 1;

    for (int i = topFile; i < bottomFile; ++i) {
        bool isPointer = (i == cursorPosition && isFocused);

        if(isPointer) {
            wattron(window, A_REVERSE);
        } else {
            wattroff(window, A_REVERSE);
        }

        std::string name = dir[i].name;

        if((int)name.size() >= width - 12) {
            std::string newName;

            for (int i = 0; i < width - 16; ++i) {
                newName += name[i];
            }
            newName += "...";
                
            name = newName;
        }

        name = (dir[i].isDirectory ? "/" : " ") + name;

        while((int)name.size() < width - 11) {
            name += " ";
        }

        if(!dir[i].isDirectory) {
            name += " " + dir[i].humanSize() + " ";
        } else {
            name += "  Folder ";
        }

        mvwprintw(window, currentY, currentX, name.data());
        mvwaddch(window, currentY, width - 10, ACS_VLINE);

        currentY++;
    }
    
    wattroff(window, A_REVERSE);
    
    refreshWindow();
}

void Window::moveCursor(const int direction) {
    cursorPosition = std::max(0, std::min(filesCount - 1, cursorPosition + direction));

    if(cursorPosition < topFile) {
        topFile--;
        bottomFile--;
        printFiles();
        return;
    } 

    if(cursorPosition >= bottomFile) {
        topFile++;
        bottomFile++;
        printFiles();
        return;
    }

    printFiles();
}

void Window::changeDir() {
    if(dir[cursorPosition].isDirectory) {
        if(dir[cursorPosition].name == "..") {
            leaveDir();
            return;
        }

        try {
            path = dir[cursorPosition].path;

            getFiles(path, dir);
            filesCount = (int)dir.size();
            topFile = 0, bottomFile = std::min(filesCount, height - 4);

            cursorPosition = 0;

            clearWindow();

            printFiles();
        } catch (std::filesystem::filesystem_error& e) {
            return;
        }
    }
}

void Window::leaveDir() {
    cursorPosition = 0;

    path = std::filesystem::path(path).parent_path();

    getFiles(path, dir);
    filesCount = (int)dir.size();
    topFile = 0, bottomFile = std::min(filesCount, height - 4);
        
    clearWindow();

    printFiles();
}

bool Window::findFiles(std::string& name) {
    int filesCount = (int)dir.size();

    for (int i = 0; i < filesCount; ++i) {
        if(dir[i].name.substr(0, (int)name.size()) == name) {
            cursorPosition = i;
            topFile = i;
            bottomFile = filesCount;

            if(filesCount < topFile + height - 4) {
                topFile = std::max(0, filesCount - height + 4);
            } else {
                bottomFile = topFile + height - 4;
            }

            return 1;
        }
    }

    return 0;
}

void Window::findFile() {
    mvwaddch(window, height - 2, 1, '/');
    refreshWindow();

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

        mvwprintw(window, height - 2, 2, currentQuery.data());
        refreshWindow();

        int t = getch();

        if (t == 10) {
            break;
        } else if (t == 27) {
            break;
        } else if (t == KEY_BACKSPACE) {
            if(!query.empty()) {
                query.erase(query.size() - 1);
            }

            findFiles(query);

            printFiles();

            continue;
        }

        query += keyname(t);
        if(findFiles(query)) {
            printFiles();
        } else {
            query.erase(query.size() - 1);
        }
    }

    find->clear(window);
}

void Window::renameFile(const std::string& newName) {
    try {
        std::filesystem::rename(path + "/" + dir[cursorPosition].name, 
                                path + "/" + newName);\

        dir[cursorPosition].path = path + "/" + newName;
        dir[cursorPosition].name = newName;    
    } catch (std::filesystem::filesystem_error& e) {
        return;
    }   
}