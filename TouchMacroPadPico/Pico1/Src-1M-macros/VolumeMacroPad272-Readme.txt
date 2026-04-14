Compiled with Pico SDK 2.21-develop, Arduino Pico 5.5.1 and included Adafruit_TinyUSB_Arduino 3.7.2, and TFTeSPI 2.5.43
Pico 1 RP2040 and 3.5inch Touch Display Module for 200MHz Raspberry Pi Pico included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
Using library Adafruit_TinyUSB_Arduino at version 3.7.2 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\SdFat 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad272.ino.elf"
Sketch uses 260652 bytes (24%) of program storage space. Maximum is 1044480 bytes.
Global variables use 62652 bytes (23%) of dynamic memory, leaving 199492 bytes for local variables. Maximum is 262144 bytes.
Resetting COM9
Converting to uf2, output size: 595456, start address: 0x2000
Scanning for RP2040 devices
Flashing E: (RPI-RP2)
Wrote 595456 bytes to E:/NEW.UF2
-------------------------------------------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.
If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

NB: Use 2MB Flash option with 1MB Sketch 1 MB FS 
    Use USB Stack Adafruit TinyUSB
    Use 200MHz CPU 

    Waveshare settings in https://files.waveshare.com/upload/f/fc/Pico-ResTouch-LCD-X_X_Code.zip are different:
    (1) #define ILI9488_DRIVER - Bodmer WARNING: Do not connect ILI9488 display SDO to MISO if other devices share the SPI bus 
        because TFT SDO does NOT tristate when CS is high https://github.com/Bodmer/TFT_eSPI/discussions/898
        Waveshare schematic https://files.waveshare.com/upload/8/85/Pico-ResTouch-LCD-3.5_Sch.pdf has LCD NOT connected to MISO 
        TFT connections is via ||->serial 74HC interface no MISO used there - Therefore Bodmer can be ignored
    (2) #define TFT_INVERSION_ON        // Waveshare has this commented out in their Setup60_RP2040_ILI9341.h

    Used here:  #define SPI_FREQUENCY       30000000   // Waveshare use 7MHz - 35MHz still seems ok
                #define SPI_READ_FREQUENCY  16000000   // 20MHz also ok
                #define SPI_TOUCH_FREQUENCY  2500000   // 1MHz max 250kHz or 600kHz needed for Pico 2

New changes:
1. Added pre-coded M1-M24 text string as stringm24.h - use new iList number 14 to send without loading or saving the text strings - similar to Do2=1 option
2. Expanded iList options to 19.
3. Fixed regression errors for Timers and Clocks + 1 minute lock fix
4. Added 0xF4 and 0xF0 options to ExecuteCode()
5. Macro Instruction List execution data sent to PC App iList tab where it can be edited and sent back to Pico.
6. Labels L1, L3, L4 char L replaced by nKeys char - switch on/off with *0n*. Config1 only saved if App switching is not active (AppState==0)
7. Fix when large textfile is printed then the second action is also executed additionally when using L4. This can be corrected by choosing an alternative instruction list that excludes 
   that specific second action, but in this case DoNKeys() was modified: 
   if (StrLen==0) { if (LayerAxD) status("nKeys File not found on SDCard"); else status("nKeys File not found on Flash"); } else LinkOk = true;
8. Various fixes to <1 - <6 handling, *up*0,1 off/on Macro Upper/Lower case added, SDCardNum Pad [n] + Pad[o] used, SDCardArr[2] sent to PC App, changes to [Cfg][Sav] such as A-D only 
   saved from [A-D] not L1,L3,L4 changes.
9. MacroUL part of Config1
10, Added Custom Labels M,S,T to config  data sent to PC App.
11. Added Custom Labels Off/On *lm,s,t*0,1 switch custem labels off/on - *lm,s,t*x <> 0,1 toggle state on/off
12. Fixed Labels Folder switching in Star Codes function and added nDir = AppDir switch. Reset nDir to / after App Switch closed.


