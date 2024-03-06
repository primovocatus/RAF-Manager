#include <ncurses.h>

#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <algorithm>

#include "directory.h"
#include "find.h"

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
        std::vector<file> dir;

        Find* find;

        WINDOW* window;

        friend screen;

    public:
        Window();
        Window(const int& y, const int& x, const int& width, 
                const int& height, const std::string homeDir);


        bool isFocused = 0;

        void clearWindow() const;
        void refreshWindow() const;
        void setCursor(const int position);

        void writeBox() const;
        void printName() const;
        void printFiles();

        void moveCursor(const int position);

        void changeDir();
        void leaveDir();

        void findFile();
        bool findFiles(std::string& name);

        void renameFile(const std::string& newName);

        void toHome();
        void toEnd();
};