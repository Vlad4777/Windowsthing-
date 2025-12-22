# Windowsthing-
<img width="64" height="64" alt="windowsthing" src="https://github.com/user-attachments/assets/2604d867-29fc-47fa-9cc5-76ba98819e0a" />

A Arduino based open source media controller for Windows that shows currently playing media and controls it 

Parts needed for assembly:

Electronics:
  -Arduino Pro Micro
  -Rotary encoder with 6mm knurl shaft and integrated push button
  -ssd1306 128x32 i2c oled display
  -Flexible wires (I used 28AWG/ 0.08 mm²)
  
3d printed parts:
  -Housing 
  -Housing cover 
  -Encoder knob
  Aviable on printables. Printed in pla, other materials can be used. 

Other parts:
  -4x Countersunk screw M3x8
  -4x M3 Nuts

Controls:
 Press - play/pause
 Rotate right - volume up
 Rotate left - volume down
 Press & rotate right - next track
 Press & rotate left - previous track

The desktop host application fetches data about currently playing media from the global Windows media player and sends it to the Arduino Pro Micro over serial.

The Arduino Pro Micro displays data about currently playing media on the oled. 
