#include <syntacts>

using namespace tact;

int main(int argc, char const *argv[])
{
    Session session;

    session.openControlPanel(12);

    std::cout << session.open(12) << std::endl;

    while(true) {

    }
    return 0;
}
