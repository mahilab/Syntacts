from syntacts import *

session = Session()

for dev in session.available_devices:
    print("")
    print("Index:       ", dev.index)
    print("Name:        ", dev.name)
    print("Default:     ", dev.is_default)
    print("API:         ", dev.api_name)
    print("API Default: ", dev.is_api_default)
    print("Max Channels:", dev.max_channels)
    print("Sample Rates:", dev.sample_rates)
 