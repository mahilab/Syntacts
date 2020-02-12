using System;
using System.Threading;
using Syntacts;

// run with the following console command:
// dotnet run

class Example
{
    static void Main(string[] args)
    {
        Session ss = new Session();
        ss.Open();

        Signal sig = new Noise();

        // Create spatializer
        Spatializer sp = new Spatializer(ss);

        // set up position of channels, evenly distributed 
        int chs = ss.channelCount;
        double spc = 1.0 / (chs - 1);
        for (int i = 0; i < ss.channelCount; ++i) 
            sp.SetPosition(i, new Point(i * spc,0));

        // set up target where vibration will be played
        sp.target = new Point(0,0);
        sp.radius = 0.1;
        
        // play
        sp.Play(sig);

        // make moving target on pre-described path
        double t = 0;
        while (t < 10) {
            double xPos = 0.5 + 0.5 * Math.Sin(2*Math.PI*t);
            sp.target = new Point(xPos, 0);
            Sleep(0.01);
            t += 0.01;
        }

        Console.WriteLine("Finished");
        ss.Dispose();
    }
    static void Sleep(double seconds) {
        Thread.Sleep((int)(seconds*1000));
    }
}

