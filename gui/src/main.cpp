#ifndef DEBUG
#define MAHI_GUI_NO_CONSOLE
#endif
#include "Gui.hpp"

int main(int argc, char const *argv[])
{    
    Gui gui;
    gui.run();
    return 0;
}
