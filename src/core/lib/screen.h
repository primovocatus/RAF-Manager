#include <ncurses.h>

#include "window.h"
#include "find.h"
#include "rename.h"

class Screen {
    private:
        int mainWindowWidth;
        int mainWindowHeight;

        int leftWindowWidth;
        int rightWindowWidth;

        bool currentWindow;

        Window* leftWindow;
        Window* rightWindow;

        Find* find;
        Rename* rename;

    public:
        Screen();
        ~Screen();

        void listener();
};