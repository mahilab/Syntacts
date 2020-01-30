using System;
using System.Threading;
using Syntacts;

// run with the following console command:
// dotnet run

class Example
{
    static void Main(string[] args)
    {
        Session s = new Session();
        s.Open();

        Signal x = new Sine(440) * new Triangle(20) * new ASR(1,2,3);
        Signal y = new Square(440, 1000) * new ADSR(1,1,1,1);
        Signal z = new Pwm(500,0.5) * new Envelope(1);

        s.Play(0, x);
        Thread.Sleep((int)x.length * 1000);
        s.Play(1, y);
        Thread.Sleep((int)x.length * 1000);

        Sequence seq = new Sequence();
        seq.Push(1).Push(x).Push(-2).Push(y).Insert(z, 4);

        Library.SaveSignal(seq, "csharp");

        Signal loaded;
        Library.LoadSignal(out loaded, "csharp");

        s.PlayAll(loaded);
        Thread.Sleep((int)loaded.length * 1000);

        s.Dispose(); // important!
    }
}

