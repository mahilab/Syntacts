from syntacts import *
from time import sleep
from math import sin
from math import pi

#  Returns musical note corresponding to name and octave
def key(name, octave):
    names = ['C','C#','D','D#','E','F','F#','G','G#','A','A#','B']
    idx = names.index(name)
    i = idx - 9 + (octave - 4) * 12
    a = 2**(1/12)
    freq = 440 *  a**i
    return Square(freq) * ASR(0.05, 0.10, 0.1)


funkytown = Sequence() 
Dsharp = key("D#",6)
Csharp = key("C#",6)
Asharp = key("A#",5)
Gsharp = key("G#",6)
G      = key("G", 6)  
funkytown << Dsharp << Dsharp << Csharp << Dsharp  << 0.2 << Asharp  << 0.2 << Asharp << Dsharp << Gsharp << G << Dsharp

dixie = Sequence() 
dixie << key("B", 5) << key("G#",5) << key("E", 5) << key("E", 5) << key("E", 5) << key("F#",5) << key("G#",5) << key("A", 5) << key("B", 5) << key("B", 5) << key("B", 5) << key("G#",5)

session = Session()
session.open()
Library.save_signal(funkytown, "funky")
session.play_all(funkytown)
sleep(funkytown.length + 0.25)
session.play_all(dixie)
sleep(dixie.length + 0.25)
