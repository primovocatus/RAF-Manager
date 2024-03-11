#include <ncurses.h>

#include "window.h"
#include "rename.h"
#include "delete.h"

class Screen {
    private:
        int mainWindowWidth;
        int mainWindowHeight;

        int leftWindowWidth;
        int rightWindowWidth;

        bool currentWindow;

        Window* leftWindow;
        Window* rightWindow;

        Delete* del;
        Rename* rename;

    public:
        Screen();
        ~Screen();

        bool listener();
};