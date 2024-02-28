#include <ncurses.h>

#include <string>
#include <vector>
#include <algorithm>

#include "directory.h"

class Window {
    private:
        int coordinateY;
        int coordinateX;

        int width;
        int height;

        int cursorPosition = 0;
        int topFile = 0, bottomFile = 0;
        int filesCount = 0;

        std::string path = "/";
        std::vector<std::string> stack;
        std::vector<file> dir;

        WINDOW* window;

        friend screen;

    public:
        Window();
        Window(const int& y, const int& x, const int& width, 
                const int& height);

        bool isFocused = 0;

        void clearWindow() const;
        void refreshWindow() const;

        void writeBox() const;
        void printName() const;
        void printFiles();

        void moveCursor(const int);

        void changeDir();
        void leaveDir();

        void findFile(const std::string& name);
};