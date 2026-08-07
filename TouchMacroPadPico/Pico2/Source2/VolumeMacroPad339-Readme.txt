Compiled with Pico SDK 2.3.0, Arduino Pico 6.0.0 and included Adafruit_TinyUSB_Arduino 3.7.7, and TFTeSPI 2.5.44
Pico 2 RP2350 and 3.5inch Touch Display Module for 150MHz Raspberry Pi Pico 2 included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
RP2350-E9: Adding absolute block to UF2 targeting 0x10ffff00
Multiple libraries were found for "SD.h"
 Used: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\SD
 Not used: C:\Program Files (x86)\Arduino\libraries\SD
 Not used: C:\Users\Tobias\Documents\Arduino\libraries\SD
Using library Adafruit_TinyUSB_Arduino at version 3.7.7 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\SPI 
Using library TFT_eSPI at version 2.5.44 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\LittleFS 
Using library SD at version 2.0.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\SD 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\SdFat 
Using library Time at version 1.6.1 in folder: C:\Users\Tobias\Documents\Arduino\libraries\Time 
Using library Wire at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\6.0.0\libraries\Wire 
Using library SparkFun_Qwiic_Twist at version 1.0.4 in folder: C:\Users\Tobias\Documents\Arduino\libraries\SparkFun_Qwiic_Twist 
Using library Adafruit_MCP23017_Arduino_Library at version 2.3.2 in folder: C:\Users\Tobias\Documents\Arduino\libraries\Adafruit_MCP23017_Arduino_Library 
Using library Adafruit_BusIO at version 1.17.4 in folder: C:\Users\Tobias\Documents\Arduino\libraries\Adafruit_BusIO 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\5.0.0-9576866/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad339.ino.elf"
Sketch uses 270468 bytes (12%) of program storage space. Maximum is 2088960 bytes.
Global variables use 71756 bytes (13%) of dynamic memory, leaving 452532 bytes for local variables. Maximum is 524288 bytes.
Resetting COM19
Converting to uf2, output size: 624640, start address: 0x2000
Scanning for RP2040 devices
Flashing E: (RP2350)
Wrote 624640 bytes to E:/NEW.UF2
----------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.

If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

NB: Use 4MB Flash option with 2MB Sketch 2MB FS 
    Use USB Stack Adafruit TinyUSB

    Waveshare settings in https://files.waveshare.com/upload/f/fc/Pico-ResTouch-LCD-X_X_Code.zip are different:
    (1) #define ILI9488_DRIVER - Bodmer WARNING: Do not connect ILI9488 display SDO to MISO if other devices share the SPI bus 
        because TFT SDO does NOT tristate when CS is high https://github.com/Bodmer/TFT_eSPI/discussions/898
        Waveshare schematic https://files.waveshare.com/upload/8/85/Pico-ResTouch-LCD-3.5_Sch.pdf has LCD NOT connected to MISO 
        TFT connections is via ||->serial 74HC interface no MISO used there - Therefore Bodmer can be ignored
    (2) #define TFT_INVERSION_ON        // Waveshare has this commented out in their Setup60_RP2040_ILI9341.h
    (3) Wire1 i2c1 External Devices SDA/SCL GP26/GP27 and Wire i2c0 External Devices on GP4/GP5.
        Sparkfun Twist RGB Rotary Encoder and MPC23xxx GPIO Expanders on Wire1 i2c1

New changes:
1. Google Gemini fixes for functions DoLinkStr() DoNKeys() DoKeyMST() DoKey16()
2. Arduino Pico 6.0.0 and Pico SDK 2.3.0 and nKeysL134 = false CheckSerial = true KeyHeldEnable = false as defaults
3. *ic* i2c bus scanner *ic*0,1aabb aa bb hex value external i2c0 devices use 0 SDA SCL aa,bb = 00-7F
4. Added *ic* i2c bus scanner
5. Added Twists connected to PC App
6. Fixed 2nd and 3rd Twist not changing colour when turned
7. Fixed missed Twist device 0 and limited scanning for Twists devices to actual connected devices.
8. Changed option x to repeat the last key pressed when Twist is turned - it is a very useful option. Whether the [S1] is pressed that types a text string, 
   or the [Del]ete key, or the [*Cm] key that runs through all the star options - all three repeat when turning the Twist knob. 
   Option capital X can now be assigned two characters - enter *tc*abcd and and b will replace the / and * typed when the Twist is turned.
9. Added up to 7 Sparkfun Twist Encoder i2c devices - default is 2 but change #define twX 2 to the required number 0-7 of twistDevices.
   If more than one Twist device choose which Twist device to configure and control with the star commands through *tc**n with n = 0-7 
   where 0 is when one Twist device connected. For example four Twist devices connected but control the second device through starcodes 
   and the PC App, then use *tc**1.
10. Added MCP23008,MCP23017,MCP23018 0-8 devices on i2c bus. Can read inputs then run either linked seequence of macros or single macro, and set outputs
    using star codes. Can toggle inputs and outputs on the PC App. See manual section (Ab) for details.
11. CircuitPython device control and used as input and/or output for Pico macropad and/or PC App. 
    *cp* CircuitPython filelist *cp*cnn c=command a,d,r,c nn=filelist index=00-99 *cp*cnnfilename a Activate, d Delete, r Rename, c Copy
    *cp*cnn a-z, nn=00-99 Commands to control CircuitPy device a-z excludes a,c,d,r. Commands sent to CPy device as <Ccnn> or <CcnnFileName>
    Pico macropad receives CircuitPy device filelist as <CX:File1.py,File2.py;File3.py> with File2.py the active function and X: the driveletter.
12. Rotary Encoder long-press for "Twist Options d-Z Ready" shows. Then turn encoder for the options vuzsxdwbVUZSXDWB. Long-press Twist again
    to exit the encoder options mode. If the star * option is chosen the Twist mode will change from the coded options Volume, Scroll, Zoom 
    etc. to Twist File macros. To cahnge back from file macros choose any of coded macros such as S V Z etc.







 








