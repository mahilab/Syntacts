from syntacts import *
from time import sleep

session = Session()
session.open(10,2,48000)

a = Sine(100) * ASR(.1,.1,.1)
b = Square(100) * ASR(.1,.1,.1)
c = Sine(100) * ASR(.1,.1,.1)


x = 1 << a << b << 1 << c

# session.play(0, x)
# sleep(x.length())

Library.save_signal(x, "pythonX")

session.close()

