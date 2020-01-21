#include <syntacts>
#include <iomanip>

using namespace tact;

// Show pass/fail for testing purposes (you don't need this)
inline void showResult(bool r) {
    std::cout << (r ? "Pass" : "Fail") << std::endl;
}

int main(int argc, char const *argv[])
{

    // Make a Signal to save/export, and a blank Signal to import in to
    Signal out = Sine(440) * ASR(1,1,1);
    Signal in;

    // Syntacts Binary Format (Default Location, i.e. APPDATA/Syntacts/Library)
    
    showResult( Library::saveSignal(out, "out"));
    showResult( Library::loadSignal(in, "out"));

    // Syntacts Binary Format (Custom Location)

    showResult( Library::exportSignal(out, "out.sig"));
    showResult( Library::importSignal(in, "out.sig"));

    showResult( Library::exportSignal(out, "relative/folder/out.sig"));
    showResult( Library::importSignal(in, "relative/folder/out.sig"));

    showResult( Library::exportSignal(out, "/absolute/folder/out.sig"));
    showResult( Library::importSignal(in, "/absolute/folder/out.sig"));

    // Syntacts JSON Format

    showResult( Library::exportSignal(out, "out.json"));
    showResult( Library::importSignal(in, "out.json"));

    showResult( Library::exportSignal(out, "relative/folder/out.json"));
    showResult( Library::importSignal(in, "relative/folder/out.json"));

    showResult( Library::exportSignal(out, "/absolute/folder/out.json"));
    showResult( Library::importSignal(in, "/absolute/folder/out.json"));

    // WAV Format

    showResult( Library::exportSignal(out, "out.wav"));
    showResult( Library::importSignal(in, "out.wav"));

    showResult( Library::exportSignal(out, "relative/folder/out.WAV"));
    showResult( Library::importSignal(in, "relative/folder/out.WAV"));

    showResult( Library::exportSignal(out, "/absolute/folder/out.wave"));
    showResult( Library::importSignal(in, "/absolute/folder/out.wave"));

    // AIFF Format

    showResult( Library::exportSignal(out, "out.aiff"));
    showResult( Library::importSignal(in, "out.aiff"));

    showResult( Library::exportSignal(out, "relative/folder/out.AIFF"));
    showResult( Library::importSignal(in, "relative/folder/out.AIFF"));

    showResult( Library::exportSignal(out, "/absolute/folder/out.aifc"));
    showResult( Library::importSignal(in, "/absolute/folder/out.aifc"));

    // CSV/TXT Format (import not yet supported)

    showResult( Library::exportSignal(out, "out.csv"));
    showResult( Library::exportSignal(out, "relative/folder/out.txt"));
    showResult( Library::exportSignal(out, "/absolute/folder/out.txt"));

    return 0;
}
