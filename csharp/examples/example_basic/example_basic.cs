using System;
using System.Threading;
using Syntacts;

// run with the following console command:
// dotnet run

class Example
{
    static void Main(string[] args)
    {
    // Syntacts usage begins with creating an audio context, or Session
    Session session = new Session();
   
    // Now let's open a device ...

    // Usually, you would use "session.open(i)"" to open a specific device with index i 

    // Alternatively, you can just open the system default device by passing no arguments 
    // (which this example does because we don't know what device numbers you might have!)   
    session.Open();
    

    //-------------------------------------------------------------------------
      
    // Now, let's create some vibrations ...

    // Vibrations are represented by Signals and combinations of Signals

    // Some Signals (e.g. oscillators) have an INFINITE duration
    Signal sig1 = new Sine(440); // a 440 Hz sinewave
    Console.WriteLine(sig1.length); // inf

    // Other Signals (e.g. envelopes) have FINITE duration
    Signal sig2 = new ASR(1,3,1); // a 5 second attack, sustain, release envelope
    Console.WriteLine(sig2.length); // 5

    // Signals can be combined using math operation
    Signal sig3 = sig1 * sig2; // a 5 second 440 Hz sinewave with an ASR envelope
    Console.WriteLine(sig3.length); // 5

    // Such operations can be done in a single line
    Signal sig4 = new Square(880) * new Sine(10) * new ADSR(1,1,1,1); // 880 Hz square, amplitude modulated with 10 Hz sine and 4 s ADSR envelope
    Console.WriteLine(sig4.length); // 4

    // For more advanced Signal synthesis, see "example_signals.cpp"

    //-------------------------------------------------------------------------

    // Now that we have some Signals, let's play them ...

    // Play sig1 on channel 0 of the open Device
    session.Play(0, sig1);
    // The Signal will immediately start playing in the Session's audio thread,
    // but we need to sleep this thread so that the program doesn't continue prematurely
    Sleep(3);
    // Now, we stop the Signal on channel 0 (sig1 will have played for 3 seconds)
    session.Stop(0);

    // Let's play another on channel 1...
    session.Play(1, sig3);
    Sleep(sig3.length);
    // We don't have to call session.stop(1) because sig3 is FINITE

    // You can also play a Signal on all channels
    session.PlayAll(sig4);
    Sleep(sig4.length);

    //-------------------------------------------------------------------------

    // It is important to dispose of the session at the end of your program!
    session.Dispose();

    //-------------------------------------------------------------------------

    // This was an extremely basic example of using Syntacts. See the other 
    // examples for more complex usage and other included features!
    }
    static void Sleep(double seconds) {
        Thread.Sleep((int)(seconds*1000));
    }
}

