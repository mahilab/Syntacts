from syntacts import *
from time import sleep
from math import sin
from math import pi

#-----------------------------------------------------------
# Function to make sure export/import works
def check(signal):
    if signal is not None:
        print('Pass')
    else:
        print('Fail') 

#-----------------------------------------------------------
s = Session()
s.open()

# Make a Signal to save/export
py = Sine(440) * ASR(1,1,1)

# Syntacts Binary Format (Default Location, i.e. APPDATA/Syntacts/Library)

Library.save_signal(py, 'py')
loaded = Library.load_signal('py')
s.play(0,loaded)
sleep(loaded.length)
check(loaded)

# # Syntacts Binary Format (Custom Location)

Library.export_signal(py, 'relative/folder/py.sig')
loaded = Library.import_signal('relative/folder/py.sig')
check(loaded)
del loaded

Library.export_signal(py, '/absolute/folder/py.sig')
loaded = Library.import_signal('/absolute/folder/py.sig')
check(loaded)
del loaded

# # Syntacts JSON Format

Library.export_signal(py, 'relative/folder/py.json')
loaded = Library.import_signal('relative/folder/py.json')
check(loaded)
del loaded

Library.export_signal(py, '/absolute/folder/py.json')
loaded = Library.import_signal('/absolute/folder/py.json')
check(loaded)
del loaded

# # WAV Format

Library.export_signal(py, 'relative/folder/py.wav')
loaded = Library.import_signal('relative/folder/py.wav')
check(loaded)
del loaded

Library.export_signal(py, '/absolute/folder/py.wav')
loaded = Library.import_signal('/absolute/folder/py.wav')
check(loaded)
del loaded

# # AIFF Format

Library.export_signal(py, 'relative/folder/py.aiff')
loaded = Library.import_signal('relative/folder/py.aiff')
check(loaded)
del loaded

Library.export_signal(py, '/absolute/folder/py.aiff')
loaded = Library.import_signal('/absolute/folder/py.aiff')
check(loaded)
del loaded

# CSV/TXT Format (import not yet supported)

Library.export_signal(py, 'py.csv')
Library.export_signal(py, 'relative/folder/py.txt')
Library.export_signal(py, '/absolute/folder/py.txt')
