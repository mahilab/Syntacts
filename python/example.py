from syntacts import *
from time import sleep


s = Session()
print( s.open(3, 8, 44100) )
print( s.getChannelCount() )
print( s.getSampleRate() )
print( s.getCpuLoad() )
print (s.count() )

# sleep(10)