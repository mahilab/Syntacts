#ifndef DEBUG
#define MAHI_GUI_NO_CONSOLE
// #define MAHI_GUI_USE_DISCRETE_GPU
#endif
#include "Gui.hpp"

int main(int argc, char const *argv[])
{    
    Gui gui;
    gui.run();
    return 0;
}
