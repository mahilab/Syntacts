using System;
using System.Threading;
using Syntacts;

// run with the following console command:
// dotnet run

class Example
{
    static void Main(string[] args)
    {
    Session session = new Session();
    session.Open();

    int freq = 440; //Hz, Choose signal frequency, 440 Hz is audible and ok for speakers. Many tactors are closer to 150-250 Hz

    // We will begin by creating some basic oscillators, these are default amplitude 1.0 and infinite length of time
    Signal sin = new Sine(freq); // Sine wave
    Signal squ = new Square(freq); // Square wave
    Signal saw = new Saw(freq); // Saw wave
    Signal tri = new Triangle(freq); // Triangle wave
    // We can use pulse width modulation (PWM) to quickly create a repeating cue train with frequency (1Hz) and duty cycle (0.3)
    Signal pwm = new Pwm(1, 0.3);

    // Now we can pair those oscillators with an envelope to give them shape

    // This is a basic envelope that specifies amplitude (0.9), and duration (0.5 sec)
    Signal bas = new Envelope(0.9, 0.5);
    // This is an attack (1 sec), sustain (3 sec), release (1 sec) envelope. The sustain amplitude is 1.0. 
    Signal asr = new ASR(1, 2, 1, 1.0); 
    // This adds one more part to the above envelope. Attack (1 sec, to amplitude 1.0), decay (2 sec), 
    // sustain (3 sec, amplitude 0.8), release (1 sec). Curves can be added here as well
    Signal adsr = new ADSR(1, 2, 3, 1, 1.0, 0.8);
                

    // Pairing these oscillators and envelopes can give us complex cues

    Signal sig1 = sin * bas;

    Signal sig2 = sin * pwm * adsr;
    
    // More information in sequencing these in time can be found in examples_sequences
    session.Play(0,sig2);
    Sleep(sig2.length);
    session.Stop(0);
    
    session.Dispose();
    }
    static void Sleep(double seconds) {
        Thread.Sleep((int)(seconds*1000));
    }
}

