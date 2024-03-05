#include <ncurses.h>
#include <string>
#include <vector>

class Find {
    private:
        int coordinateY;
        int coordinateX;

        int width;
        int height;

    public:
        Find(const int& windowSize, const int& coordinateY);

        void clear(WINDOW* win) const;
};