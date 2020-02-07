from syntacts import *
from time import sleep
from math import sin
from math import pi

  
# ------------------------------------------------------------------------

# Syntacts usage begins with creating an audio context, or Session
session = Session()

# Now let's open a device ...

# Usually, you would use "session.open(i)"" to open a specific device with index i 

# Alternatively, you can just open the system default device by passing no arguments 
# (which this example does because we don't know what device numbers you might have!)   
session.open()


# -------------------------------------------------------------------------
    
# Now, let's create some vibrations ...

# Vibrations are represented by Signals and combinations of Signals

# Some Signals (e.g. oscillators) have an INFINITE duration
sig1 = Sine(440) # a 440 Hz sinewave
print(sig1.length) # inf

# Other Signals (e.g. envelopes) have FINITE duration
sig2 = ASR(1,3,1) # a 5 second attack, sustain, release envelope
print(sig2.length) # 5

# Signals can be combined using math operation
sig3 = sig1 * sig2 # a 5 second 440 Hz sinewave with an ASR envelope
print(sig3.length) # 5

# Such operations can be done in a single line
sig4 = Square(880) * Sine(10) * ADSR(1,1,1,1) # 880 Hz square, amplitude modulated with 10 Hz sine and 4 s ADSR envelope
print(sig4.length) # 4

# For more advanced Signal synthesis, see "example_signals.cpp"

#-------------------------------------------------------------------------

# Now that we have some Signals, let's play them ...

# Play sig1 on channel 0 of the open Device
session.play(0, sig1)
# The Signal will immediately start playing in the Session's audio thread,
# but we need to sleep this thread so that the program doesn't continue prematurely
sleep(3)
# Now, we stop the Signal on channel 0 (sig1 will have played for 3 seconds)
session.stop(0)

# Let's play another on channel 1...
session.play(1, sig3)
sleep(sig3.length)
# We don't have to call session.stop(1) because sig3 is FINITE

# You can also play a Signal on all channels
session.play_all(sig4)
sleep(sig4.length)

#-------------------------------------------------------------------------

# Devices will automatically close when the Session goes out of scope,
# but it is good practice to do this explicitly

session.close()

#-------------------------------------------------------------------------

# This was an extremely basic example of using Syntacts. See the other 
# examples for more complex usage and other included features!

