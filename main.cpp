#include <ncurses.h>

#include <cassert>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <filesystem>

using namespace std;

struct file {
    string name, path;
    int size;
    bool isDirectory;

    string parseSize() {
        int type = 0;

        double lsize = size;
        while(lsize >= 1024) {
            type++;
            lsize /= 1024.;
        }

        // string rem;
        // rem += char((int)(lsize * 10) % 10 + '0');

        string humanSize = to_string((int)lsize) + " " + "BKMGTP"[type];
        while((int)humanSize.size() < 7) {
            humanSize = " " + humanSize;
        }

        return humanSize;
    }

    bool operator <(const file& X) const {
        if(isDirectory && X.isDirectory) {
            return name < X.name;
        }
        return isDirectory;
    }
};

struct window {
    int cursorPosition = 0;
    int topFile, bottomFile;
    int filesCount = 0;

    int y, x;
    int height, width;
    
    string path = "/";
    vector<string> stack;
    vector<file> dir;

    WINDOW* window;
    
    void init(int posY, int posX, int winHeight, int winWidth) {
        y = posY;
        x = posX;
        height = winHeight;
        width = winWidth;
    
        window = newwin(height, width, y, x);
    }
    void writeBox() {
        box(window, 0, 0);
        wrefresh(window);
    }
    void clear() {
        string clr;

        clr.resize(width - 10, ' ');
        clr += "|        ";
        int start = 0, end = height;
        for (int i = start; i < end; ++i) {
            mvwprintw(window, i, 0, clr.data());
        }
        
        writeBox();
        wrefresh(window);
    }
    void printName() {
        writeBox();
        mvwprintw(window, 0, 1, path.data());
        wrefresh(window);
    }
    void print() {
        int currentX = 1, currentY = 1;
        
        for (int i = topFile; i < bottomFile; ++i) {
            bool isPointer = (i == cursorPosition);

            if(isPointer) {
                wattron(window, A_REVERSE);
            } else {
                wattroff(window, A_REVERSE);
            }

            mvwprintw(window, currentY, currentX, dir[i].name.data());

            currentY++;
        }
        wattroff(window, A_REVERSE);
        wrefresh(window);
    }
    void getFiles() {
        dir.clear();

        for (const auto& files: filesystem::directory_iterator(path)) {
            file currentFile;
            
            string name = files.path().filename();
            bool isFolder = files.is_directory();

            if((int)name.size() >= width - 12) {
                string newName;

                for (int i = 0; i < width - 16; ++i) {
                    newName += name[i];
                }
                newName += "...";
                
                name = newName;
            }
            
            name = (isFolder ? "/" : " ") + name;

            currentFile.isDirectory = isFolder;
            currentFile.name = name;
            
            while((int)currentFile.name.size() < width - 11) {
                currentFile.name += " ";
            }

            try {
                currentFile.size = filesystem::file_size(files.path());
            } catch(filesystem::filesystem_error& e) {
                currentFile.size = 0;
            }
            
            if(!isFolder) {
                currentFile.name += "|" + currentFile.parseSize();
            } else {
                currentFile.name += "| Folder ";
            }

            currentFile.path = files.path();

            dir.push_back(currentFile);
        }
        filesCount = (int)dir.size();

        topFile = 0, bottomFile = min(filesCount, height - 2);

        sort(dir.begin(), dir.end());
    }
    void moveCursor(int direction) {
        cursorPosition = max(0, min(filesCount - 1, cursorPosition + direction));

        if(cursorPosition < topFile) {
            topFile--;
            bottomFile--;
            print();
            return;
        } 

        if(cursorPosition >= bottomFile) {
            topFile++;
            bottomFile++;
            print();
            return;
        }

        print();
    }
    void changeDir(string newPath) {
        cursorPosition = 0;

        stack.push_back(path);
        path = newPath;

        getFiles();

        clear();
        printName();

        print();
    }
    void leaveDir() {
        cursorPosition = 0;

        path = stack.back();
        stack.pop_back();

        getFiles();

        clear();
        printName();

        print();
    }
};

window leftWindow, rightWindow;

void init() {
    initscr();

    echo();
    curs_set(0);

    cbreak();


    int windowWidth = getmaxx(stdscr);
    int windowHeight = getmaxy(stdscr);

    int leftWindowWidth = (windowWidth + 1) / 2;        
    int rightWindowWidth = windowWidth / 2;

    leftWindow.init(0, 0, windowHeight, leftWindowWidth);
    rightWindow.init(0, leftWindowWidth, windowHeight, rightWindowWidth);
    refresh();


    leftWindow.clear();
    
    leftWindow.writeBox();
    rightWindow.writeBox();

    leftWindow.printName();

    leftWindow.getFiles();
    leftWindow.print();
}

int main() {
    init();
    
    while(1) {
        move(1, 1);
        char t = getch();

        leftWindow.print();
        rightWindow.print();

        // cout << (int)t << endl;
        if ((int)t == 115) {
            leftWindow.moveCursor(1);
        } 
        if ((int)t == 119) {
            leftWindow.moveCursor(-1);
        }
        if((int)t == 100) {
            if(leftWindow.dir[leftWindow.cursorPosition].isDirectory) {
                leftWindow.changeDir(leftWindow.dir[leftWindow.cursorPosition].path);
            }
        }
        if ((int)t == 97) {
            if(!leftWindow.stack.empty()) {
                leftWindow.leaveDir();
            }
        }
    }
    
    getch();
    endwin();
    return 0;
}