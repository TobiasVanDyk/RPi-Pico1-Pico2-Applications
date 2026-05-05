Compiled with Pico SDK 2.21-develop, Arduino Pico 5.6.0 and included Adafruit_TinyUSB_Arduino 3.7.4, and TFTeSPI 2.5.43
Pico 1 RP2040 and 3.5inch Capacitive Touch Display Module included SDCard module https://www.dfrobot.com/product-2107.html
-------------------------------------------------------------------------------------------------------------------------------------------------
Using library Adafruit_TinyUSB_Arduino at version 3.7.4 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\SdFat 
Using library GT911_Lite at version 1.0.2 in folder: C:\Users\Tobias\Documents\Arduino\libraries\GT911_Lite 
Using library Wire at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\Wire 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad423.ino.elf"
Sketch uses 270632 bytes (25%) of program storage space. Maximum is 1044480 bytes.
Global variables use 58980 bytes (22%) of dynamic memory, leaving 203164 bytes for local variables. Maximum is 262144 bytes.
Resetting COM12
Converting to uf2, output size: 613888, start address: 0x2000
Scanning for RP2040 devices
Flashing E: (RPI-RP2)
Wrote 613888 bytes to E:/NEW.UF2
----------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.
If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

NB: Use 2MB Flash option with 1MB Sketch 1 MB FS 
    Use USB Stack Adafruit TinyUSB
    Use 200 or 125 MHz CPU when higher SPI read is used:  #define SPI_FREQUENCY      35000000  // 50MHz causes volume artifacts
                                                          #define SPI_READ_FREQUENCY 15000000  // 20 MHz also ok

New changes:
1. ILI9488 and GT911 LCD and Touch display - press the Pad buttons on the inside of their border






