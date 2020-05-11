---
layout: page
title: Hardware
permalink: /hardware/
---

![Syntacts Amps](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/syntacts_amps.png)

# Amplifier

The Syntacts amplifier is a purpose built, 8-channel amplifier designed for driving vibrotactors from audio sources. The amp uses eight fully differential, Class AB `TI-TPA6211A1-Q1` power amplifiers for a completely noise free operation. Two variants are available: one with an AES-59 DB25 input, great for connecting to high-end devices like the MOTU 24Ao; and one with four 3.5 mm phone inputs, perfect for use with generic audio output or surround sound cards. Both require a 5V power supply and output amplified signal through a universal 2x8 0.1" ptich header. The amp will drive loads greater than 3 ohms, and we have successfully tested it with a vairety of LRAs from [Digikey](https://www.digikey.com/products/en/motors-solenoids-driver-boards-modules/motors-ac-dc/178?k=&pkeyword=&sv=0&pv110=419682&sf=0&FV=-8%7C178&quantity=&ColumnSort=0&page=1&pageSize=25), EAI's [C2 and C3](https://www.eaiinfo.com/tactor-info) voice coil actuators, and Nanoport's [TacHammer](https://nanoport.io/haptics/) actuators. 

CAD/EDA files, bill of materials, and documentation for both variants can be found below. The files have been formated so that you can upload or send them directly to your favorite board house. Need a recommendation? We have used [Sunstone Circuits](https://www.sunstone.com/) for basic PCB printing, and both [Sierra Circuits](https://www.protoexpress.com/) and [Screaming Circuits](https://www.screamingcircuits.com/) for turn-key printing and assembly with great results. Both designs are licensed under the [TAPR Open Hardware License](https://tapr.org/the-tapr-open-hardware-license/). Feel free to modify and/or redistribute the board designs within the confines of the license, but understand that we are not liable for any damage or harm the amplifier may cause to you or your equipment. *It is your responsibility safely integrate the amplifier with your projects*.  

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
|**License**|[TAPR Open Hardware License](https://tapr.org/the-tapr-open-hardware-license/)|[TAPR Open Hardware License](https://tapr.org/the-tapr-open-hardware-license/)|

## Tested Actuators

The following table lists actuators that have been tested and are known to run well on the Syntacts amplifier. This list is by no means exhaustive. If you discover that the amplifier runs an actuator not listed here, please let us know by submitted an Issue on GitHub.

|Manufacturer|Part Number|Type|Link|Comments|
|---|---|---|---|---|
|Mplus|1040W|LRA|[Link](http://www.mpluskr.com/?page_id=200)|Same as JME G1040001D|
|Jinlong Machinery & Electronics|G1040001D|LRA|[Link](https://www.vibration-motor.com/coin-vibration-motors/coin-linear-resonant-actuators-lra/g1040003d)|Same as Mplus 1040W|
|Jinlong Machinery & Electronics|	LV061228B-L65-A|LRA|[Link](https://www.digikey.com/product-detail/en/jinlong-machinery-electronics-inc/LV061228B-L65-A/1670-1050-ND/7732325)| Z-axis motor|
|Engineering Acoustics, Inc.|C-2|Voice Coil|[Link](https://www.eaiinfo.com/tactor-info)|Larger version of C-3|
|Engineering Acoustics, Inc.|C-3|Voice Coil|[Link](https://www.eaiinfo.com/tactor-info)|Larger version of C-2|
|Nanoport|TacHammer-Carlton|Voice Coil|[Link](https://nanoport.io/haptics/)||

# Tactile Displays


