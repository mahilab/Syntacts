using System;
using System.Threading;
using System.Collections.Generic;
using Syntacts;

// run with the following console command:
// dotnet run

class Example
{
    // This example is meant to show the full process of creating cues, sequencing 
    // them in time, and finally playing it. We're not saying that Syntacts is meant
    // to create music, but this is an illustration that can be played over speakers

    /// Returns musical note n semitones away from f0 (440 Hz = A4 by default)
    static Signal key(string name, int octave){
        string[] names = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
        int idx = Array.IndexOf(names, name);
        float i = (float)idx - 9.0f + ((float)octave - 4.0f) * 12.0f;
        float a = (float)Math.Pow(2.0f,1.0f/12.0f);
        float freq = 440.0f *  (float)Math.Pow(a,i);
        Console.WriteLine(freq);
        return new Square(freq) * new ASR(0.05, 0.10, 0.1);
    }
    // Cretes the notes required for "Funkytown" and returns the notes sequenced in time 
    static Sequence funkytown() {
        Signal Dsharp = key("D#",6);
        Signal Csharp = key("C#",6);
        Signal Asharp = key("A#",5);
        Signal Gsharp = key("G#",6);
        Signal G      = key("G", 6);  
        Sequence funkytown = new Sequence();
        return funkytown.Push(Dsharp).Push(Dsharp).Push(Csharp).Push(Dsharp).Push(0.2f).Push(Asharp).Push(0.2f).Push(Asharp).Push(Dsharp).Push(Gsharp).Push(G).Push(Dsharp);
    }

    // Cretes the notes required for "Dixie" and returns the notes sequenced in time 
    static Sequence dixie() {
        Sequence dixie = new Sequence();
        return dixie.Push(key("B", 5)).Push(key("G#",5)).Push(key("E", 5)).Push(key("E", 5)).Push(key("E", 5)).Push(key("F#",5)).Push(key("G#",5)).Push(key("A", 5)).Push(key("B", 5)).Push(key("B", 5)).Push(key("B", 5)).Push(key("G#",5));
    }

    static void Main(string[] args)
    {
        Session session = new Session();

        session.Open();  
        Sequence f = funkytown(); 
        Library.SaveSignal(f, "funky");
        session.PlayAll(f);
        Sleep(f.length + 0.25f);
        Sequence d = dixie();
        session.PlayAll(d);
        Sleep(d.length + 0.25f);
        session.Dispose(); // important!
    }

    static void Sleep(double seconds) {
        Thread.Sleep((int)(seconds*1000));
    }
}

