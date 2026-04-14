Compiled with Pico SDK 2.21-develop, Arduino Pico 5.5.1 and included Adafruit_TinyUSB_Arduino 3.7.2, and TFTeSPI 2.5.43
Pico 1 RP2040 and 3.5inch Touch Display Module for 200MHz Raspberry Pi Pico included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
Using library Adafruit_TinyUSB_Arduino at version 3.7.2 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.1\libraries\SdFat 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad424.ino.elf"
Sketch uses 260324 bytes (24%) of program storage space. Maximum is 1044480 bytes.
Global variables use 62644 bytes (23%) of dynamic memory, leaving 199500 bytes for local variables. Maximum is 262144 bytes.
Resetting COM9
Converting to uf2, output size: 594432, start address: 0x2000
Scanning for RP2040 devices
Flashing E: (RPI-RP2)
Wrote 594432 bytes to E:/NEW.UF2
----------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.
If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

NB: Use 2MB Flash option with 1MB Sketch 1 MB FS 
    Use USB Stack Adafruit TinyUSB
    Use 200 or 125 MHz CPU when higher SPI read is used:  #define SPI_FREQUENCY      35000000  // 50MHz causes volume artifacts
                                                          #define SPI_READ_FREQUENCY 15000000  // 20 MHz also ok

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












