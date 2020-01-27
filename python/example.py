from syntacts import *
from time import sleep

session = Session()
res = session.open(3,8,48000)

s = Sine(440)

session.play(0,s)

sleep(10)