#include <ncurses.h>
#include <string>
#include <filesystem>

class Delete {
    private:
        int coordinateX;
        int coordinateY;

        int width;
        int height;

        bool button;

        WINDOW* window;

    public:
        Delete(const int& windowSize, const int& windowHeight);

        void clear() const;
        bool print(const bool& isDirectory, const std::string& fileName,
                const std::string& path);
        
        void printButton(const int& positionX, const int& positionY, const bool& buttonType, const bool& isPointer);
};