---
layout: page
title: Hardware
permalink: /hardware/
---

![Syntacts Amps](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/syntacts_amps.png)

## Syntacts Amplifiers

The Syntacts amplifier is a purpose built, 8-channel amplifier designed for driving vibrotactors from audio sources. The amp uses eight fully differential, Class AB `TI-TPA6211A1-Q1` power amplifiers for low-noise operation. Two variants are available: one with an AES-59 DB25 input, great for connecting to high-end devices like the MOTU 24Ao; and one with four 3.5 mm phone inputs, perfect for use with generic audio output or surround sound cards. Both require a 5V power supply and output amplified signal through a universal 2x8 0.1" pitch header. The amp will drive loads greater than 3 ohms, and we have successfully tested it with a variety of LRAs and voice coil actuators.

![Syntacts Cases](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/cases2.png)

CAD/EDA files, bill of materials, and documentation for both variants can be found below. The files have been formatted so that you can upload or send them directly to your favorite board house. Need a recommendation? We have used [Sunstone Circuits](https://www.sunstone.com/) for basic PCB printing, and both [Sierra Circuits](https://www.protoexpress.com/) and [Screaming Circuits](https://www.screamingcircuits.com/) for turn-key printing and assembly with great results.

All designs are licensed under the [TAPR Open Hardware License](https://tapr.org/the-tapr-open-hardware-license/). Feel free to modify and/or redistribute the board designs within the confines of the license, *but understand that we are not liable for any damage or harm the amplifier may cause to you or your equipment*. **It is your responsibility to safely integrate the amplifier with your projects**.  

|**Version**|**v1.2**|**v3.1**|
|:--|:-:|:-:|
|**Image**|<img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/amps/v1.2.png" width="200">|<img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/amps/v3.1.png" width="200">|
|**Channels**|8|8|
|**Input**|AES-59 DB25|3.5 mm TRS (4)|
|**Output**|2x8 0.1" Pitch Header|2x8 0.1" Pitch Header|
|**Dimensions**|2.375" x 3.5" x 0.54"|2.375" x 3.5" x 0.43"|
|**CAD / BOM**| [syntacts_amp_v1.2.zip](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/amps/syntacts_amp_v1.2.zip) | [syntacts_amp_v3.1.zip](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/amps/syntacts_amp_v3.1.zip) | 
|**3D Model**| [syntacts_amp_v1.2.step](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/amps/syntacts_amp_v1.2.step) | [syntacts_amp_v3.1.step](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/amps/syntacts_amp_v3.1.step) | 
|**Manual**| [syntacts_amp_v1.2.pdf](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/amps/syntacts_amp_v1.2.pdf) | [syntacts_amp_v3.1.pdf](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/amps/syntacts_amp_v3.1.pdf) | 
|**Case**| *N/A* | [syntacts_case_v3.x.zip](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/amps/syntacts_case_v3.x.zip) | 
|**Kit**| *N/A* | [syntacts_kit_v3.x.pdf](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/amps/syntacts_kit_v3.x.pdf) |
|**License**|[TAPR Open Hardware License](https://tapr.org/the-tapr-open-hardware-license/)|[TAPR Open Hardware License](https://tapr.org/the-tapr-open-hardware-license/)|

## Tactile Displays

Below you fill find simple tactile displays that you can create or learn from as you get started with Syntacts. You may modify the designs however you like.

|Display|Syntacts Array|Syntacts Bracelet|
|---|:-:|:-:|
|**Image**|<img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/array.png" width="200">|<img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/bracelet.png" width="200">|
|**Description**|High density 2D tactile array|Tactile bracelet with integrated tensioner|
|**Actuators**|24x 10mm LRAs|8x 10mm LRAs|
|**CAD/BOM**|[syntacts_array_v1.0.zip](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/displays/syntacts_array_v1.0.zip)|*Coming Soon*|
|**License**|[TAPR Open Hardware License](https://tapr.org/the-tapr-open-hardware-license/)|[TAPR Open Hardware License](https://tapr.org/the-tapr-open-hardware-license/)|

## Actuators

The following table lists actuators that have been tested and are known to run well on the Syntacts amplifier. This list is by no means exhaustive. If you discover that the amplifier runs an actuator not listed here, please let us know by submitting an Issue on GitHub.

|Manufacturer|Part Number|Type|
|---|---|---|
|Mplus|[1040W](http://www.mpluskr.com/?page_id=200)|LRA|
|Jinlong Machinery & Electronics|[G1040001D](https://www.vibration-motor.com/coin-vibration-motors/coin-linear-resonant-actuators-lra/g1040003d)|LRA|
|Jinlong Machinery & Electronics|[LV061228B-L65-A](https://www.digikey.com/product-detail/en/jinlong-machinery-electronics-inc/LV061228B-L65-A/1670-1050-ND/7732325)|LRA|
|Engineering Acoustics, Inc.|[C2](https://www.eaiinfo.com/tactor-info)|Voice Coil|
|Engineering Acoustics, Inc.|[C3](https://www.eaiinfo.com/tactor-info)|Voice Coil|
|Nanoport|[TacHammer-Carlton](https://nanoport.io/haptics/)|Voice Coil|

## Audio Interfaces

We have tested the following audio interfaces with Syntacts. As above, this is not an exhaustive list and in general Syntacts will work with virtually any audio interface on the market. 

> On Windows, you may want to look for audio devices that implement the [ASIO](https://en.wikipedia.org/wiki/Audio_Stream_Input/Output) driver protocol. ASIO drivers typically provide lower latency performance than standard Windows driver protocols (i.e. MME, WASAPI, DirectSound, and WDM-KS). High-end or professional-grade audio devices usually do a better job at implementing the ASIO standard for ultra-low latency performance. 

|Manufacturer|Product Name|Interface|Max Channels|ASIO Driver|Approx. Cost|
|---|---|---|---|---|---|
|MOTU|[24Ao](https://motu.com/products/avb/24ai-24ao)|USB|24|Excellent|$1000|
|MOTU|[UltraLite Mk4](https://motu.com/products/proaudio/ultralite-mk4)|USB|12|Excellent|$600|
|ASUS|[Xonar U7 MKII](https://www.asus.com/us/Sound-Cards/Xonar-U7-MKII/)|USB|8|Good|$90|
|StarTech|[7.1 USB External Adapter](https://www.startech.com/Cards-Adapters/Sound/USB-Audio/USB-7-Channel-Audio-Adapter-with-SPDIF~ICUSBAUDIO7D)|USB|8|N/A|$50|
|ASUS|[Xonar AE](https://www.asus.com/Sound-Cards/Xonar-AE/)|PCIe|8|Good|$80|
|Soundblaster|[SB Audigy RX7.1](https://us.creative.com/p/sound-blaster/sound-blaster-audigy-rx)|PCIe|8|Ok|$50|
