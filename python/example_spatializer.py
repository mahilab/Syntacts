from syntacts import *
from time import sleep
from math import sin
from math import pi

#----------------------------------------------------------
s = Session()
s.open()

sig = Noise()

sp = Spatializer(s)

# set up channels
chs = s.channel_count # choose the number of channels you want to use here
spc = 1.0 / (chs - 1)
for i in range(chs):
    sp.set_position(i, (i * spc, 0))

# set up target
sp.target = (0,0) # arbitrary position 
sp.radius = 0.1 # arbitrtary radius

# play
sp.play(sig)

# do something cool
t = 0
while (t < 10):
    x = 0.5 + 0.5 * sin(2*pi*t)
    sp.target = (x, 0)
    sleep(0.01)
    t = t + 0.01

del sp # ensure Spatializer deleted before Session

print("Finished!")