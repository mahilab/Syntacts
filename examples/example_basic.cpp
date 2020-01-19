#include <syntacts> // include Syntacts in its entirety
#include <iomanip>

using namespace tact; // everything in Syntacts is under the namespace tact

int main(int argc, char const *argv[])
{  
    //-------------------------------------------------------------------------

    // Syntacts usage begins with creating an audio context, or Session
    Session session;

    // With a Session, we can enumerate the Device map to see what audio
    // interfaces are available for us to use
    for (auto& item : session.getAvailableDevices()) {
        int idx = item.first;            // the device index is the map key
        const Device& dev = item.second; // the device struct is the map value
        std::cout << idx << " " << dev.name << std::endl;
    }
   
    // Now let's open a device ...

    // Usually, you would use "session.open(i)"" to open a specific device with index i 

    // Alternatively, you can just open the system default device by passing no arguments 
    // (which this example does because we don't know what device numbers you might have!)   
    session.open();

    //-------------------------------------------------------------------------
      
    // Now, let's create some vibrations ...

    // Vibrations are represented by Signals and combinations of Signals

    // Some Signals (e.g. oscillators) have an INFINITE duration
    Signal sig1 = Sine(440); // a 440 Hz sinewave
    std::cout << sig1.length() << std::endl; // inf

    // Other Signals (e.g. envelopes) have FINITE duration
    Signal sig2 = ASR(1,3,1); // a 5 second attack, sustain, release envelope
    std::cout << sig2.length() << std::endl; // 5

    // Signals can be combined using math operation
    Signal sig3 = sig1 * sig2; // a 5 second 440 Hz sinewave with an ASR envelope
    std::cout << sig3.length() << std::endl; // 5

    // Such operations can be done in a single line
    Signal sig4 = Square(880) * Sine(10) * ADSR(1,1,1,1); // 880 Hz square, amplitude modulated with 10 Hz sine and 4 s ADSR envelope
    std::cout << sig4.length() << std::endl; // 4

    // For more advanced Signal synthesis, see "example_signals.cpp"

    //-------------------------------------------------------------------------

    // Now that we have some Signals, let's play them ...

    // Play sig1 on channel 0 of the open Device
    session.play(0, sig1);
    // The Signal will immediately start playing in the Session's audio thread,
    // but we need to sleep this thread so that the program doesn't continue prematurely
    sleep(3);
    // Now, we stop the Signal on channel 0 (sig1 will have played for 3 seconds)
    session.stop(0);

    // Let's play another on channel 1...
    session.play(1, sig3);
    sleep(sig3.length());
    // We don't have to call session.stop(1) because sig3 is FINITE

    // You can also play a Signal on all channels
    session.playAll(sig4);
    sleep(sig4.length());

    //-------------------------------------------------------------------------

    // Devices will automatically close when the Session goes out of scope,
    // but it is good practice to do this explicitly

    session.close();

    //-------------------------------------------------------------------------

    // This was an extremely basic example of using Syntacts. See the other 
    // examples for more complex usage and other included features!

}
