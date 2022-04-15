#include <syntacts>

using namespace tact; // everything in Syntacts is under the namespace tact

int main(int argc, char const *argv[])
{
    Session session;
    session.open();

    int freq = 440; //Hz, Choose signal frequency, 440 Hz is audible and ok for speakers. Many tactors are closer to 150-250 Hz

    // We will begin by creating some basic oscillators, these are default amplitude 1.0 and infinite length of time
    Signal sin = Sine(freq); // Sine wave
    Signal squ = Square(freq); // Square wave
    Signal saw = Saw(freq); // Saw wave
    Signal tri = Triangle(freq); // Triangle wave
    // We can use pulse width modulation (PWM) to quickly create a repeating cue train with frequency (1Hz) and duty cycle (0.3)
    Signal pwm = Pwm(1, 0.3);

    // Now we can pair those oscillators with an envelope to give them shape

    // This is a basic envelope that specifies duration (0.9), and amplitude (0.5 sec)
    Signal bas = Envelope(0.9, 0.5);
    // This is an attack (1 sec), sustain (3 sec), release (1 sec) envelope. The sustain amplitude is 1.0. 
    // Envelopes can interpolate between amplitudes with different curves, this example uses a smooth s-curve and linear.
    Signal asr = ASR(1, 2, 1, 1.0, Curves::Smootheststep(), Curves::Linear()); 
    // This adds one more part to the above envelope. Attack (1 sec, to amplitude 1.0), decay (2 sec), 
    // sustain (3 sec, amplitude 0.8), release (1 sec). Curves can be added here as well
    Signal adsr = ADSR(1, 2, 3, 1, 1.0, 0.8);
    // You can also use another oscillator as an envelope. This is a 10 Hz sine wave, duration 2.1 sec, amplitude 0.9
    Signal sigEnv = SignalEnvelope(Sine(10), 2.1, 0.9);
                

    // Pairing these oscillators and envelopes can give us complex cues

    Signal sig1 = sin * bas;

    Signal sig2 = sin * pwm * adsr;

    // You can also do addition/subtraction operations to create cues you may wany

    Signal sig3 = squ - pwm;
    
    // More information in sequencing these in time can be found in examples_sequences
    session.play(0,sig2);
    sleep(sig2.length());
    session.stop(0);
    
    session.close();
}
