#define CARNOT_NO_CONSOLE
#define CARNOT_USE_DISCRETE_GPU

#include "Gui.hpp"

using namespace carnot;

int main(int argc, char const *argv[])
{
    Engine::makeRoot<Gui>();
    Engine::init(960, 540, WindowStyle::Close);
    Engine::window->setTitle("Syntacts");
    Engine::setBackgroundColor(Grays::Gray5);
    Engine::run(); 
    return 0;
}
