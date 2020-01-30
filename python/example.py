from syntacts import *
from time import sleep


s = Session()
print( s.open(22, 24, 44100) )
print( s.getChannelCount() )
print( s.getSampleRate() )
print( s.getCpuLoad() )
print (s.count() )

seq = Sequence()
seq.head = 5
print (seq.head)

sn = Sine(440, Sine(10), 100) * ADSR(1,1,1,1)

print (sn.length())

# sleep(10)