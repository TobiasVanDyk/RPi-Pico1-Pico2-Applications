Compiled with Pico SDK 2.21-develop, Arduino Pico 5.6.0 and included Adafruit_TinyUSB_Arduino 3.7.4, and TFTeSPI 2.5.44
Pico 1 RP2040 and DFRobot DFR0669 3.5inch Capacitve Touch Display Module with ILI9488 and GT911
-------------------------------------------------------------------------------------------------------------------------------------------------
Multiple libraries were found for "SD.h"
 Used: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\SD
 Not used: C:\Program Files (x86)\Arduino\libraries\SD
 Not used: C:\Users\Tobias\Documents\Arduino\libraries\SD
Using library Adafruit_TinyUSB_Arduino at version 3.7.4 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\SPI 
Using library TFT_eSPI at version 2.5.44 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\LittleFS 
Using library SD at version 2.0.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\SD 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\SdFat 
Using library Wire at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\Wire 
Using library SparkFun_Qwiic_Twist at version 1.0.4 in folder: C:\Users\Tobias\Documents\Arduino\libraries\SparkFun_Qwiic_Twist 
Using library GT911_Lite at version 1.0.2 in folder: C:\Users\Tobias\Documents\Arduino\libraries\GT911_Lite 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad600.ino.elf"
Sketch uses 282848 bytes (27%) of program storage space. Maximum is 1044480 bytes.
Global variables use 64032 bytes (24%) of dynamic memory, leaving 198112 bytes for local variables. Maximum is 262144 bytes.
Resetting COM14
Converting to uf2, output size: 640512, start address: 0x2000
Scanning for RP2040 devices
Flashing E: (RPI-RP2)
Wrote 640512 bytes to E:/NEW.UF2
----------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.
If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

New changes:
1. Rotary Encoder long-press for "Twist Options d-Z Ready" shows. Then turn encoder for the options vuzsxdwbVUZSXDWB.
   Single-press Twist to exit the encoder options mode. Note: Do not release immediately when "Ready" shows, keep 
   pressing for another tenth of a second else the "Chenged" message will show before any changes are made
2. ILI9488 and GT911 LCD and Touch display 
If calibration does not run on first start force by *ro*  
Connections: GT911: Use Pico gpio 4 and 5 for i2c (Wire). Use the connections in User_Setup.h for the rest (same as Waveshare LCDs)
             Twist: Use Pico gpio 26 and 27 for i2c (Wire1)













