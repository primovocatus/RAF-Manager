#include "core/lib/screen.h"

int main() {

    while(1) {
        Screen mainScreen;
        while(mainScreen.listener());
        mainScreen.~Screen();
    }

    getch();

    return 0;
}