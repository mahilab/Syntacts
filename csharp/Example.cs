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
        Sleep(x.length);
        s.Play(1, y);
        Sleep(y.length);

        Sequence seq = new Sequence();
        seq.Push(1).Push(x).Push(-2).Push(y).Insert(z, 4);

        Library.SaveSignal(seq, "csharp");

        Signal loaded;
        Library.LoadSignal(out loaded, "csharp");

        s.PlayAll(loaded);
        Sleep(loaded.length);

        Signal noise = new Noise();
        Spatializer sp = new Spatializer(s);
        sp.SetPosition(0, new Point(0,0));
        sp.SetPosition(1, new Point(1,0));
        sp.target = new Point(0,0);
        sp.radius = 0.5;
        sp.Play(noise);

        double t = 0;
        while (t < 10) {
            double xPos = 0.5 + 0.5 * Math.Sin(2*Math.PI*t);
            sp.target = new Point(xPos, 0);
            Sleep(0.01);
            t += 0.01;
        }

        s.Dispose(); // important!
    }

    static void Sleep(double seconds) {
        Thread.Sleep((int)(seconds*1000));
    }
}

