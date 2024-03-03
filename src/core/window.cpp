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
    topFile = 0, bottomFile = std::min(filesCount, height - 2);

    window = newwin(height, width, y, x);

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

    for (int i = coordinateY + 1; i < coordinateY + height - 1; ++i) {
        mvwaddch(window, i, width - 10, '|');
    }

    refreshWindow();
}

void Window::printName() const {
    writeBox();

    if(isFocused) {
        wattron(window, A_REVERSE);
    }

    mvwprintw(window, 0, 1, path.data());
    wattroff(window, A_REVERSE);

    refreshWindow();
}

void Window::printFiles() {
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
            name += "|" + dir[i].humanSize();
        } else {
            name += "| Folder ";
        }

        mvwprintw(window, currentY, currentX, name.data());

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
            topFile = 0, bottomFile = std::min(filesCount, height - 2);

            cursorPosition = 0;

            clearWindow();

            writeBox();
            printName();
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
    topFile = 0, bottomFile = std::min(filesCount, height - 2);
        
    clearWindow();

    writeBox();
    printName();
    printFiles();
}

void Window::findFile(const std::string& name) {
    for (int i = 0; i < (int)dir.size(); ++i) {
        if(dir[i].name == name) {
            cursorPosition = i;
            topFile = i;
            bottomFile = filesCount;

            if(filesCount < topFile + height - 2) {
                topFile = std::max(0, filesCount - height + 2);
            } else {
                bottomFile = topFile + height - 2;
            }
        }
    }
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