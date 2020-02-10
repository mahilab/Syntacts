from syntacts import *
from time import sleep
from math import sin
from math import pi

#-----------------------------------------------------------

s = Session()
s.open()

# Make a Signal to save/export, and a blank Signal to import in to
py = Sine(440) * ASR(1,1,1)


# Syntacts Binary Format (Default Location, i.e. APPDATA/Syntacts/Library)

Library.save_signal(py, 'py')
loaded = Library.load_signal('py')
s.play(0,loaded)
sleep(loaded.length)

# # Syntacts Binary Format (Custom Location)

Library.save_signal(py, 'py.sig')
loaded = Library.load_signal('py.sig')


Library.export_signal(py, '.\\relative\\folder\\ py.sig')
loaded_sig = Library.import_signal('.\\relative\\folder\\ py.sig')
print(loaded_sig.length)

# Library.save_signal(py, '/absolute/folder/py.sig')
# loaded = Library.load_signal('/absolute/folder/py.sig')

# # Syntacts JSON Format

Library.save_signal(py, 'py.json')
loaded = Library.load_signal('py.json')

# Library.save_signal(py, 'relative/folder/py.json')
# loaded = Library.load_signal('relative/folder/py.json')

# Library.save_signal(py, '/absolute/folder/py.json')
# loaded = Library.load_signal('/absolute/folder/py.json')

# # WAV Format

Library.save_signal(py, 'py.wav')
loaded = Library.load_signal('py.wav')

# Library.save_signal(py, 'relative/folder/py.wav')
# loaded = Library.load_signal('relative/folder/py.wav')

# Library.save_signal(py, '/absolute/folder/py.wav')
# loaded = Library.load_signal('/absolute/folder/py.wav')

# # AIFF Format

Library.save_signal(py, 'py.aiff')
loaded = Library.load_signal('py.aiff')

# Library.save_signal(py, 'relative/folder/py.aiff')
# loaded = Library.load_signal('relative/folder/py.aiff')

# Library.export_signal(py, '/absolute/folder/py.aiff')
# loaded_aiff = Library.import_signal('/absolute/folder/py.aiff')

# CSV/TXT Format (import not yet supported)

Library.save_signal(py, 'py.txt')
loaded = Library.load_signal('py.txt')  
