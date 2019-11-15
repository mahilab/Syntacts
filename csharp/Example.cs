using System;
using Syntacts;

class Example
{
    static void Main(string[] args)
    {
        Session session = new Session();
        session.Open(8,1);

        Cue cue = new Cue();
        var asr = new ASR(1,3,1);
        var sin = new Sine(440);

        cue.SetEnvelope(asr);
        cue.Chain(sin);
        cue.Chain(new Sine(20));

        Syntacts.Dll.Library_saveCue(cue.handle, "willThisWork");

        session.Play(0, cue);

        session.Close();
        session.Dispose();

        Console.WriteLine(Signal.Count());

        while (true) {
            Console.WriteLine(session.IsOpen());
        }
    }
}
