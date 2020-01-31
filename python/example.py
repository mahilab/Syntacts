from syntacts import *
from time import sleep

s = Session()
s.open()

x = Sine(440) * Triangle(20) * ASR(1,2,3)
y = Square(440, 1000) * ADSR(1,1,1,1)
z = Pwm(500,0.5) * Envelope(1)

s.play(0, x)
sleep(x.length())
s.play(1, y)
sleep(y.length())

seq = Sequence()

seq << 1 << x << -2 << y
seq.insert(z, 4)

Library.saveSignal(seq,'python')

loaded = Library.loadSignal('python')
if (loaded):
    s.playAll(loaded)
    sleep(loaded.length())

