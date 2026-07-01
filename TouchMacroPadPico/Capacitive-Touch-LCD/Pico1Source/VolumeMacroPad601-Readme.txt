Compiled with Pico SDK 2.21-develop, Arduino Pico 5.6.1 and included Adafruit_TinyUSB_Arduino 3.7.7, and TFTeSPI 2.5.44
Pico 1 RP2040 and DFRobot DFR0669 3.5inch Capacitve Touch Display Module with ILI9488 and GT911
-------------------------------------------------------------------------------------------------------------------------------------------------
Multiple libraries were found for "SD.h"
 Used: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.1\libraries\SD
 Not used: C:\Program Files (x86)\Arduino\libraries\SD
 Not used: C:\Users\Tobias\Documents\Arduino\libraries\SD
Using library Adafruit_TinyUSB_Arduino at version 3.7.7 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.1\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.1\libraries\SPI 
Using library TFT_eSPI at version 2.5.44 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.1\libraries\LittleFS 
Using library SD at version 2.0.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.1\libraries\SD 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.1\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.1\libraries\SdFat 
Using library Wire at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.1\libraries\Wire 
Using library GT911_Lite at version 1.0.2 in folder: C:\Users\Tobias\Documents\Arduino\libraries\GT911_Lite 
Using library SparkFun_Qwiic_Twist at version 1.0.4 in folder: C:\Users\Tobias\Documents\Arduino\libraries\SparkFun_Qwiic_Twist 
Using library Adafruit_MCP23017_Arduino_Library at version 2.3.2 in folder: C:\Users\Tobias\Documents\Arduino\libraries\Adafruit_MCP23017_Arduino_Library 
Using library Adafruit_BusIO at version 1.17.4 in folder: C:\Users\Tobias\Documents\Arduino\libraries\Adafruit_BusIO 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad601.ino.elf"
Sketch uses 303248 bytes (29%) of program storage space. Maximum is 1044480 bytes.
Global variables use 71296 bytes (27%) of dynamic memory, leaving 190848 bytes for local variables. Maximum is 262144 bytes.
Resetting COM14
Converting to uf2, output size: 686592, start address: 0x2000
Scanning for RP2040 devices
Flashing E: (RPI-RP2)
Wrote 686592 bytes to E:/NEW.UF2
----------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.
If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

New changes:
1. Changed option x to repeat the last key pressed when Twist is turned - it is a very useful option. Whether the [S1] is pressed that types a text string, 
   or the [Del]ete key, or the nKeys page-up [+] key, or the [*Cm] key that runs through all the star options - all four repeat when turning the Twist knob. 
   Option capital X can now be assigned two characters - enter *tc*abcd and and b will replace the / and * typed when the Twist is turned.
2. Added up to 10 Sparkfun Twist Encoder i2c devices - default is 2 but change #define twX 2 to the required number 0-9 of twistDevices.
   If more than one Twist device choose which Twist device to configure and control with the star commands through *tc**n with n = 0-9 
   where 0 is when one Twist device connected. For example four Twist devices connected but control the second device through starcodes 
   and the PC App, then use *tc**1.
3. Added MCP23008,MCP23017,MCP23018 0-8 devices on i2c bus. Can read inputs then run either linked seequence of macros or single macro, and set 
   outputs or simulate inputs using star codes. Can toggle inputs and outputs on the PC App. See manual section (Ab) for details.
4. CircuitPython device control and used as input and/or output for Pico macropad and/or PC App. 
   *cp* CircuitPython filelist *cp*cnn c=command a,d,r,c nn=filelist index=00-99 *cp*cnnfilename a Activate, d Delete, r Rename, c Copy
   *cp*cnn a-z, nn=00-99 Commands to control CircuitPy device a-z excludes a,c,d,r. Commands sent to CPy device as <Ccnn> or <CcnnFileName>
   Pico macropad receives CircuitPy device filelist as <CX:File1.py,File2.py;File3.py> with File2.py the active function and X: the driveletter.
5. Rotary Encoder long-press for "Twist Options d-Z Ready" shows. Then turn encoder for the options vuzsxdwbVUZSXDWB. Long-press Twist again
   to exit the encoder options mode. If the star * option is chosen the Twist mode will change from the coded options Volume, Scroll, Zoom 
   etc. to Twist File macros. To cahnge back from file macros choose any of coded macros such as S V Z etc.
6. ILI9488 and GT911 LCD and Touch display 

If calibration does not run on first start force by *ro*  
Connections: GT911: Use Pico gpio 26 and 27 for i2c (Wire1). Use the connections in User_Setup.h for the rest (same as Waveshare LCDs)
             Twist and MPC23xxx: Use Pico gpio 4 and 5 for i2c (Wire1)













