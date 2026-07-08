# TheraPatch
TheraPatch is a handheld therapeutic device designed to help mitigate the effects of injuries such as bruises, sprains, minor cuts, and more. By using a reverse-polarity system via an H-bridge, TheraPatch can heat the affected area (dilating blood vessels) or cool it (constricting blood vessels), both of which have been shown to help with injury healing at certain temperatures (which TheraPatch can achieve). 

# Use of TheraPatch
To use the TheraPatch, first ensure your battery/wall adapter is plugged in, then locate the 4 buttons on the top of the device; these buttons will allow you to set your desired parameters by moving through the mode (hot/cold), set temp, and set time menus. After completing, click the start button to initiate your therapy session, and place the side with the Peltier facing out against the affected area

# Motivation
I created TheraPatch with the primary motivation of helping people with common sports and activity-related injuries while maintaining an efficient device to do so. Most of the time, when you have a bruise or scarpe you'd rely on disposable ice packs and single-use heating pads, which are wasteful and inefficient for long-term use. Therpatch offers an effecit and infitely resuable sultion to this, which overall will create a benfit among athelets, and the induvaual.

The TheraPatch looks like a simple black-and-white hot-cold block, but its guts are far more advanced. In electronics, generating heat is relatively simple. Electrons bump into each other rapidly in the wire, vibrating and thus creating the heat we feel. This is the easy part; cooling is the real engineering challenge. To cool it, I used a Peltier module, which transfers heat from one side to the other. Now, this needs to be driven by a heatsink and a fan to effectively cool. To achieve heating and cooling on the same side, I used an H bridge. Now the H-bridge module reverses the polarity in a Peltier module, meaning that when one side is + and the other is -, it cools; vice versa, the side that was just cold becomes hot. All of this is controlled by a custom 4-layer PCB I designed in KiCad, with an ATmega328P as my brain. The firmware reads a thermistor pressed against the Peltier plate and uses a bang-bang controller to turn the Peltier on/off to reach the desired temperature. This is all run by a rechargeable battery (charges via USB-C) or a 12V wall adapter. These components are encased in a CAD-made box with a hole for all the input plugs and vent fans for air circulation.

Here are images of my project






# CAD with the PCB:

<img width="491" height="524" alt="Screenshot 2026-07-04 at 1 53 12 PM" src="https://github.com/user-attachments/assets/c9aa02cf-47bf-4d0b-9159-3f62e53c6c75" />

<img width="860" height="535" alt="Screenshot 2026-07-04 at 1 53 32 PM" src="https://github.com/user-attachments/assets/61e79f9d-1325-4d9e-8768-30e69478cb92" />


# CAD input ports, fins, and Peliter hole

<img width="576" height="519" alt="Screenshot 2026-07-04 at 1 54 11 PM" src="https://github.com/user-attachments/assets/8a456222-b25e-43ab-94a3-2ddde18c2c22" />

<img width="739" height="498" alt="Screenshot 2026-07-04 at 1 55 22 PM" src="https://github.com/user-attachments/assets/37d6ade7-3b82-45b5-abc2-69d01d795669" />

<img width="366" height="356" alt="Screenshot 2026-07-04 at 1 55 41 PM" src="https://github.com/user-attachments/assets/d9225b02-e006-4cde-81e3-795e07dad236" />

# Link to Onshape case:

https://cad.onshape.com/documents/d627d659f05dae60588ec801/w/ad74ef98cd2d7a13cb59f9ec/e/f50ed022a94792dc2cfe6130?renderMode=0&uiState=6a4947abb17d022f5d41873a



# PCB (Signal Layer and ground):

F.cu (signal traces and ground):

<img width="654" height="654" alt="Screenshot 2026-06-03 at 1 51 27 AM" src="https://github.com/user-attachments/assets/fbb4aada-035c-4d4d-a86d-8448950266f5" />


In1.cu(ground):

<img width="746" height="748" alt="Screenshot 2026-06-03 at 1 52 55 AM" src="https://github.com/user-attachments/assets/b6a859eb-523d-4d4b-ae47-f9068150ecfb" />

In2.cu (5V & 12V power)

<img width="784" height="783" alt="Screenshot 2026-06-03 at 1 53 45 AM" src="https://github.com/user-attachments/assets/139ca568-bd86-47c3-9217-3a166924e9ce" />

B.cu (signal traces and ground):

<img width="783" height="778" alt="Screenshot 2026-06-03 at 1 54 23 AM" src="https://github.com/user-attachments/assets/faacdbda-8a3d-41f6-8973-919288aac1d3" />

3D:

<img width="879" height="861" alt="Screenshot 2026-06-03 at 1 55 00 AM" src="https://github.com/user-attachments/assets/a0fd8c00-925b-4637-ac82-2886d6d96704" />

Schematic (wiring diagram):

Pic_1:

<img width="975" height="641" alt="Screenshot 2026-06-03 at 1 55 57 AM" src="https://github.com/user-attachments/assets/ce9e7d3d-e9ee-4357-b866-ed55678c2371" />

Pwr:

<img width="1009" height="650" alt="Screenshot 2026-06-03 at 1 56 46 AM" src="https://github.com/user-attachments/assets/f2777dec-bb50-4640-930f-3b2bf1c6f293" />


