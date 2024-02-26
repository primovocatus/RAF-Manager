#include "core/lib/screen.h"

int main() {
    Screen mainScreen;

    while(1) {
        mainScreen.listener();
    }

    getch();

    return 0;
}