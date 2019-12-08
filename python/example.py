from syntacts import *
from time import sleep

session = Session()
session.open(10,2,48000)

s = Square(440)
m = Sine(10)
e = ASR(3,3,1)

p = s * m * e

Library.save_signal(p, "python2")

l = Library.load_signal("meow")

session.play(0,l)

sleep(l.length()+1)

session.close()