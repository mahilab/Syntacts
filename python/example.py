from syntacts import *
from time import sleep

session = Session()
session.open(10,2,48000)

a = Sine(100) * ASR(.1,.1,.1)
b = Square(100) * ASR(.1,.1,.1)
c = Sine(100) * ASR(.1,.1,.1)

x = 1 << a << b << 1 << c

y = 0.5 * (a - 0.5)

# session.play(0, x)
# sleep(x.length())

Library.save_signal(y, "pythonY")

session.close()

