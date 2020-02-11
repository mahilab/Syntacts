using System;
using System.Threading;
using Syntacts;

class example
{
    static void Main(string[] args)
    {
    Session s = new Session();
    s.Open();

    // Sequences allow to you order multiple Signals in time.

    // Most simply, you can concatenate two or more signals:
    Signal sigA = new Sine(440) * new ASR(1,1,1);
    Signal sigB = new Square(440) * new ADSR(1,1,1,1);

    Sequence seq1 = new Sequence();
    seq1.Push(sigA).Push(sigB);

    Console.WriteLine(sigA.length); // 3 s
    Console.WriteLine(sigB.length); // 4 s
    Console.WriteLine(seq1.length); // 7 s

    s.PlayAll(seq1);
    Sleep(seq1.length);

    // You can also add delay, pause, and fade with scalar values:
    Sequence seq2 = new Sequence();
    seq2.Push(1).Push(sigA).Push(2).Push(sigB).Push(-1).Push(sigA); // 1 s delay, 2 s pause, 1 s fade

    Console.WriteLine(seq2.length); //12 s

    s.PlayAll(seq2);
    Sleep(seq2.length);

    // Of course, Sequences of Sequences is possible:
    Sequence seq3 = new Sequence();
    seq3.Push(seq1).Push(seq2); // note this will also modify seq1
    Console.WriteLine(seq3.length); // 19, we won't play this one :)

    // The << operator inserts a Signal at the Sequence head position, 
    // and then moves it forward or backward. You can get/set the head position
    // manually for the next << operation:

    Console.WriteLine(seq2.head); // 12
    seq2.head = 2; // set back to 2
    seq2.Push(new Noise() * new Envelope(1)); // 1 s of noise starts at the 2 second mark

    // You can accomplish the same thing with the insert function
    seq2.Insert(new Noise() * new Envelope(1), 4); // 1 s of noise starts at the 4 second mark

    s.PlayAll(seq2);
    Sleep(seq2.length); // still 12 s long because the inserted noise didn't extend its length
    
    s.Dispose();
    }
    static void Sleep(double seconds) {
        Thread.Sleep((int)(seconds*1000));
    }
}

