#include <syntacts>
#include <functional>
#include <unordered_map>

using namespace tact;
using std::string;

// This example demonstrates how a Signal can be deconstructed with RTTI once
// it has been type erased. This isn't normally recommended, but under certain 
// circumstances it may be necessary to know what the underlying type(s)
// of a Signal are (e.g. see Nodes in the GUI source code)

void printSignal(Signal sig, int pos = 0);

void printTime(Signal sig, int pos) {
    auto cast = static_cast<Time*>(sig.get());
    std::cout << string(pos*2,' ') << "Time: " << sig.gain << std::endl;
}

void printScalar(Signal sig, int pos = 0) {
    auto cast = static_cast<Scalar*>(sig.get());
    std::cout << string(pos*2,' ') <<  "Scalar: " << cast->value << std::endl;
}

void printSine(Signal sig, int pos = 0) {
    auto cast = static_cast<Sine*>(sig.get());
    std::cout << string(pos*2,' ') << "Sine:" << std::endl;
    printSignal(cast->x, pos+1);
}

void printEnvelope(Signal sig, int pos = 0) {
    auto cast = static_cast<Envelope*>(sig.get());
    std::cout << string(pos*2,' ') << "Envelope: " << cast->duration << ',' << cast->amplitude << std::endl;
}

void printProduct(Signal sig, int pos = 0) {
    auto cast = static_cast<Product*>(sig.get());
    std::cout << string(pos*2,' ') << "Product:" << std::endl;
    printSignal(cast->lhs, pos+1);
    printSignal(cast->rhs, pos+1);
}

std::unordered_map<std::type_index,std::function<void(Signal,int)>> print_map = {
    {typeid(Scalar),   printScalar},
    {typeid(Time),     printTime},
    {typeid(Sine),     printSine},
    {typeid(Envelope), printEnvelope},
    {typeid(Product),  printProduct}
};

void printSignal(Signal sig, int pos) {
    auto id = sig.typeId();
    if (print_map.count(id))
        print_map[id](sig, pos);
    else
        std::cout << "ERROR" << std::endl;
}

int main(int argc, char const *argv[])
{
    Signal x = Sine(440) * Envelope(5, 0.75f);
    printSignal(x);
    return 0;
}

