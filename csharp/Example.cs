using System;
using Syntacts;

class Example
{
    static void Main(string[] args)
    {
        Session s = new Session();
        s.Open(8, 24);
        Signal x = new Sine(175);
        s.Play(0, x);
        System.Threading.Thread.Sleep(10000);
    }
}

