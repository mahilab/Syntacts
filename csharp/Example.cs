using System;
using Syntacts;

class Example
{
    static void Main(string[] args)
    {
        Session s = new Session();
        var result = s.Open();
        Console.WriteLine(result);
        Console.WriteLine(s.ChannelCount);
        Console.WriteLine(s.SampleRate);
        Console.WriteLine(s.CpuLoad);

        Signal x = new Sine(440) * new Sine(20) * new ASR(1,1,1);

        s.PlayAll(x);

        System.Threading.Thread.Sleep(5000);
    }
}

