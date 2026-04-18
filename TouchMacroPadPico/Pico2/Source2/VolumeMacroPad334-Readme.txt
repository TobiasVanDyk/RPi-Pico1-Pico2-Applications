Compiled with Pico SDK 2.21-develop, Arduino Pico 5.5.1 and included Adafruit_TinyUSB_Arduino 3.7.2, and TFTeSPI 2.5.43
Pico 2 RP2350 and 3.5inch Touch Display Module for 150MHz Raspberry Pi Pico 2 included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
RP2350-E9: Adding absolute block to UF2 targeting 0x10ffff00
Multiple libraries were found for "SD.h"
 Used: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\SD
 Not used: C:\Program Files (x86)\Arduino\libraries\SD
 Not used: C:\Users\Tobias\Documents\Arduino\libraries\SD
Using library Adafruit_TinyUSB_Arduino at version 3.7.2 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\LittleFS 
Using library SD at version 2.0.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\SD 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\SdFat 
Using library Time at version 1.6.1 in folder: C:\Users\Tobias\Documents\Arduino\libraries\Time 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad334.ino.elf"
Sketch uses 250980 bytes (12%) of program storage space. Maximum is 2088960 bytes.
Global variables use 64272 bytes (12%) of dynamic memory, leaving 460016 bytes for local variables. Maximum is 524288 bytes.
C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed-1/python3 -I C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1/tools/uf2conv.py --serial COM3 --family RP2040 --deploy I:\Data\Win10\Arduino/VolumeMacroPad334.ino.uf2 
Resetting COM3
Converting to uf2, output size: 581632, start address: 0x2000
Scanning for RP2040 devices
Flashing D: (RP2350)
Wrote 581632 bytes to D:/NEW.UF2
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

    Used here:  #define SPI_FREQUENCY       30000000   // Waveshare use 7MHz - 35MHz still seems ok
                #define SPI_READ_FREQUENCY  16000000   // 20MHz also ok
                #define SPI_TOUCH_FREQUENCY  500000    // 500kHz used - 250kHz or 500kHz needed for Pico 2


New changes:
1. Added Volume and Play Media Star Codes: Use *v+* *v-* *vm* Volume + - mute or *v+,-*nn = 00-99 and *p+* *p-* *pp* *ps* Play Media Next Previous Play/Pause Stop. 
   For *v+*nn and *v-*nn the nn is not and absolute number but an increase or decrease of the existing value  divided by two - i.e. if the existing value is 40 and
   you send *v+*20 then the new volume will be 50 (not 60 or 20). To set the volume to a specific value first send <*v-*50> which will set it to 0 and mute, then send 
   <*v+*value/2> i.e. send <*v+*25> if the volume is to be set at 50 percent.
2. Expanded iList options to 20 and added colour copy defaults to iList
3. Added pre-coded M1-M24 text string as stringm24.h - use new iList number 14 to send without loading or saving the text strings - similar to Do2=1 option
4. Fixed regression errors for Timers and Clocks + 1 minute lock fix
5. Added 0xF4 and 0xF0 options to ExecuteCode()
6. Macro Instruction List execution data sent to PC App iList tab where it can be edited and sent back to Pico.
7. Labels L1, L3, L4 char L replaced by nKeys char - switch on/off with *0n*. Config1 only saved if App switching is not active (AppState==0)
8. Fix when large textfile is printed then the second action is also executed additionally when using L4. This can be corrected by choosing an alternative instruction list that excludes 
   that specific second action, but in this case DoNKeys() was modified: 
   if (StrLen==0) { if (LayerAxD) status("nKeys File not found on SDCard"); else status("nKeys File not found on Flash"); } else LinkOk = true;
9. Various fixes to <1 - <6 handling, *up*0,1 off/on Macro Upper/Lower case added, SDCardNum Pad [n] + Pad[o] used, SDCardArr[2] sent to PC App, changes to [Cfg][Sav] such as A-D only 
   saved from [A-D] not L1,L3,L4 changes.
10. MacroUL part of Config1
11, Added Custom Labels M,S,T to config  data sent to PC App.
12. Added Custom Labels Off/On *lm,s,t*0,1 switch custem labels off/on - *lm,s,t*x <> 0,1 toggle state on/off


 








