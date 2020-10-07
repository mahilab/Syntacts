#ifndef DEBUG
#define MAHI_GUI_NO_CONSOLE
// #define MAHI_GUI_USE_DISCRETE_GPU
#endif
#include "Gui.hpp"

int main(int argc, char const *argv[])
{    
    mahi::gui::Application::Config config;

    config.title     = "Syntacts";
    config.width     = 960;
    config.height    = 540;
    config.resizable = false;
    config.msaa      = true;
    config.center    = true;
    // config.decorated = false;

    Gui gui(config);
    gui.run();
    return 0;
}
