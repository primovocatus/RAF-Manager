#include <ncurses.h>

#include "window.h"

class Screen {
    private:
        int mainWindowWidth;
        int mainWindowHeight;

        int leftWindowWidth;
        int rightWindowWidth;

        Window* leftWindow;
        Window* rightWindow;

    public:
        Screen();

        void listener() const;
};