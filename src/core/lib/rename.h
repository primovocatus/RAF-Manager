#include <ncurses.h>
#include <string>
#include <vector>

class Rename {
    private:
        int coordinateY;
        int coordinateX;

        int width;
        int height;

        WINDOW* window;

    public:
        Rename(const int& windowSize, const int& windowHeight);

        void clear() const;
        void print() const;

        std::string getName();
};