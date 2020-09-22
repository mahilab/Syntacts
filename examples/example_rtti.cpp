#include <syntacts>
#include <functional>
#include <unordered_map>

using namespace tact;
using std::string;

// This example demonstrates how a Signal can be deconstructed with RTTI once
// it has been type erased. This isn't normally recommended, but under certain 
// circumstances it may be necessary to know what the underlying type(s)
// of a Signal are (e.g. see Nodes in the GUI source code)

// This example is quite advanced and only applies if you hope to load, edit, 
// and resave cues. 

void signalPrinter(const Signal& sig, int depth) {
    std::cout << std::string(depth*2,' ') << signalName(sig) << std::endl;
}

int main(int argc, char const *argv[]) {
    Signal x;
    if (!Library::loadSignal(x,"funky")) {
        std::cout << "Run example_music first!" << std::endl;
        return -1;
    }
    recurseSignal(x, signalPrinter);
    return 0;
}