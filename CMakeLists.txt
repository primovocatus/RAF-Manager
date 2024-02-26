find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_executable(main src/main.cpp src/core/directory.cpp src/core/screen.cpp src/core/window.cpp)
target_link_libraries(main ${CURSES_LIBRARIES})
