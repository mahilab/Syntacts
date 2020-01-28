using System;
using Syntacts;

class Example
{
    static void Main(string[] args)
    {
        Session s = new Session();
        Console.WriteLine(s.Valid);
        var result = s.Open();
        Console.WriteLine(result);
        Console.WriteLine(s.ChannelCount);
        Console.WriteLine(s.SampleRate);
        Console.WriteLine(s.CpuLoad);

        Signal x = new Sine(440) * new Sine(20) * new ASR(1,2,3);
        Signal y = new Square(440, 1000) * new ADSR(1,1,1,1);
        Signal z = new Pwm(500,0.5) * new Envelope(1);
        Signal q = new Expression("sin(2*pi*2000*t)") * new Ramp(0,1,1);

        Sequence seq = new Sequence();
        seq.Push(1).Push(x).Push(-2).Push(y);

        Library.SaveSignal(q, "q");

        // System.Threading.Thread.Sleep(1000);

        s.Dispose(); // important!

        Console.WriteLine("The End");
    }
}

