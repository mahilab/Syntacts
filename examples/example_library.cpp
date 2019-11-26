#include <syntacts>
#include <thread>

using namespace tact;

int main(int argc, char const *argv[])
{
    auto env = ASR(0.1f,0.3f,0.1f);
    Signal sig1 = Sine(2 * PI * 175 * Time() + 2 * Sine(10)) * env;
    Signal sig2 = SineFM(175,10,2) * env;
    Signal sig3 = Expression("sin(2*pi*175*t+2*sin(2*pi*10*t))") * env;

    Library::exportSignal(sig1, "sig1", FileFormat::CSV);
    Library::exportSignal(sig2, "sig2", FileFormat::CSV);
    Library::exportSignal(sig3, "sig3", FileFormat::CSV);

    return 0;
}
