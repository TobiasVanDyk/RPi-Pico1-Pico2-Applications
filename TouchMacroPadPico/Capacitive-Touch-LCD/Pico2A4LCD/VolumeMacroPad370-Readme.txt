Compiled with Pico SDK 2.3.0, Arduino Pico 6.0.0 and included Adafruit_TinyUSB_Arduino 3.7.7, and TFTeSPI 2.5.43
Waveshare RP2350B-A4 FT6336 Capacitive Touch ST7796 LCD 3.5 480x320 with Audio ES8311, RTC, and SDCard modules:
https://www.waveshare.com/RP2350-Touch-LCD-3.5.htm
https://docs.waveshare.com/RP2350-Touch-LCD-3.5?variant=RP2350-Touch-LCD-3.5
For the Twist and MCP23xxx i2c devices use GPIO 4 and 5 (SDA and SCL Wire0) with the 3v3 and Gnd connection on the LCD 32 pin GPIO connector
-------------------------------------------------------------------------------------------------------------------------------------------------
RP2350-E9: Adding absolute block to UF2 targeting 0x10ffff00
Multiple libraries were found for "SD.h"
 Used: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\SD
 Not used: C:\Program Files (x86)\Arduino\libraries\SD
 Not used: C:\Users\Tobias\Documents\Arduino\libraries\SD
Using library Adafruit_TinyUSB_Arduino at version 3.7.7 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\LittleFS 
Using library SD at version 2.0.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\SD 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\SdFat 
Using library I2S at version 2.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\I2S 
Using library AudioBufferManager at version 1.0.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\AudioBufferManager 
Using library Time at version 1.6.1 in folder: C:\Users\Tobias\Documents\Arduino\libraries\Time 
Using library Wire at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\Wire 
Using library SparkFun_Qwiic_Twist at version 1.0.4 in folder: C:\Users\Tobias\Documents\Arduino\libraries\SparkFun_Qwiic_Twist 
Using library Adafruit-MCP23017 at version 2.3.2 in folder: C:\Users\Tobias\Documents\Arduino\libraries\Adafruit-MCP23017 
Using library Adafruit_BusIO at version 1.17.4 in folder: C:\Users\Tobias\Documents\Arduino\libraries\Adafruit_BusIO 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\5.0.0-9576866/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad370.ino.elf"
Sketch uses 290128 bytes (3%) of program storage space. Maximum is 8380416 bytes.
Global variables use 74388 bytes (14%) of dynamic memory, leaving 449900 bytes for local variables. Maximum is 524288 bytes.
C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed-1/python3 -I C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0/tools/uf2conv.py --serial COM20 --family RP2040 --deploy I:\Data\Win10\Arduino/VolumeMacroPad370.ino.uf2 
Resetting COM20
Converting to uf2, output size: 664576, start address: 0x2000
Scanning for RP2040 devices
Flashing D: (RP2350)
Wrote 664576 bytes to D:/NEW.UF2
----------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.

If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

NB: Board Setting: Also see included BoardSettings.jpg
    Generic RP2350
    Variant RP2350B (Important)
    16MB Flash option with 8MB Sketch 8MB FS (change according to own use)
    USB Stack Adafruit TinyUSB

Wire  i2c0 External Devices on GP32/GP33
Wire1 i2c1 Internal Devices on GP34/35


New changes: (All changes from 8 below were in the last GT911 capacive touch version)
1. Updated to Arduino-Pico 6.0.0 and Pico SDK 2.3.0 - fix for warning in TFT_eSPI.h see https://github.com/TobiasVanDyk/RPi-Pico1-Pico2-Applications/wiki
2. *ic* i2c bus scanner *ic*0,1aabb aa bb hex value external i2c0 devices use 0 SDA SCL aa,bb = 00-7F
3. Fixed ES8311 volume and tone *ac*t,Tnnn and *acVnn - t T short long duration
4. Fixed ES8311 audio codec - use with *ac*options such as *ac*s + filename = name.wav or /folder/filename.wav. Use 24kHz 16bit mono no metadats wav files
   Arduino-Pico i2s library used. Waveshare (modified) libraries also functional as alternative - refer to wiki.
5. Added *ic* i2c bus scanner 
6. Added Twists connected to PC App
7. Slight tweaking of FT6336 init routines
8. Fixed 2nd and 3rd Twist not changing colour when turned
9. Fixed Key Repeat and KeyHeld Vol Mute change - reverted to previous code.
10. Fixed missed Twist device 0 and limited scanning for Twists devices to actaul connected devices
11. Changed option x to repeat the last key pressed when Twist is turned - it is a very useful option. Whether the [S1] is pressed that types a text string, 
    or the [Del]ete key, or the nKeys page-up [+] key, or the [*Cm] key that runs through all the star options - all four repeat when turning the Twist knob. 
    Option capital X can now be assigned two characters - enter *tc*abcd and and b will replace the / and * typed when the Twist is turned.
12. Added up to 7 Sparkfun Twist Encoder i2c devices - default is 2 but change #define twX 2 to the required number 0-7 of twistDevices.
   If more than one Twist device choose which Twist device to configure and control with the star commands through *tc**n with n = 0-7 
   where 0 is when one Twist device connected. For example four Twist devices connected but control the second device through starcodes 
   and the PC App, then use *tc**1.









 








