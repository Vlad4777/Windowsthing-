# Windowsthing

<img width="64" height="64" alt="windowsthing" src="https://github.com/user-attachments/assets/2604d867-29fc-47fa-9cc5-76ba98819e0a" />

**Windowsthing** is an Arduino-based, open-source media controller for Windows.  
It displays the currently playing media and allows basic media control via a rotary encoder.

---

## Features
- Displays current track information
- Volume control
- Play / pause
- Next / previous track
- Compact OLED display
- USB serial communication with Windows

---

## How It Works
The desktop host application retrieves information about the currently playing media from the **Windows Global Media Player** and sends it over **serial** to an **Arduino Pro Micro**.

The Arduino then displays the media information on an **SSD1306 OLED display** and handles user input from the rotary encoder.

---
## **PLEASE NOTE!

This project is still in development and may have bugs!**

Known bugs:





Some apps like Firefox dont support timeline reporting, so the timeline has to be estimated.  May not be accurate. 



Title scrolling randomly slows down. 



If 2 or more applications play media, the display switches between the applications fast



I am happy if you report any bugs you found or improvement ideas to me.

You can **contact me directly** on:





**discord: @pablobee**



**telegram: @aosp_user**

If you want to improve the project by yourself or fix bugs, feel free to do so. 

---

## Controls

| Action | Function |
|------|---------|
| Press | Play / Pause |
| Rotate right | Volume up |
| Rotate left | Volume down |
| Press + rotate right | Next track |
| Press + rotate left | Previous track |

---

## Parts Needed

### Electronics
- Arduino Pro Micro  
- Rotary encoder with **6 mm knurled shaft** and **integrated push button**  
- SSD1306 **128×32 I²C OLED display**  
- Flexible wires (used: **28 AWG / 0.08 mm²**)  

### 3D Printed Parts
- Housing  
- Housing cover  
- Encoder knob  

> STL files are available on **Printables**.  
> Printed in **PLA**, but other materials can be used.

### Other Parts
- 4× Countersunk screws **M3×8**
- 4× **M3 nuts**

---

## Assembly
1. Solder all electronic components according to the [wiring diagram](https://github.com/Vlad4777/Windowsthing-/blob/main/wiring%20diagram.png)
2. Flash the Arduino firmware
3. Assemble the 3D printed housing
4. Install the Windows host application
5. Connect via USB

---

## License
Open source – see the LICENSE file for details.
