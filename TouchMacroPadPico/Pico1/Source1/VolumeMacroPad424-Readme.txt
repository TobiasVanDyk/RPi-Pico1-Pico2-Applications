Compiled with Pico SDK 2.21-develop, Arduino Pico 5.6.0 and included Adafruit_TinyUSB_Arduino 3.7.4, and TFTeSPI 2.5.43
Pico 1 RP2040 and 3.5inch Touch Display Module for 200MHz Raspberry Pi Pico included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
Using library Adafruit_TinyUSB_Arduino at version 3.7.4 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\SdFat 
Using library Wire at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.6.0\libraries\Wire 
Using library SparkFun_Qwiic_Twist at version 1.0.4 in folder: C:\Users\Tobias\Documents\Arduino\libraries\SparkFun_Qwiic_Twist 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad424.ino.elf"
Sketch uses 269728 bytes (25%) of program storage space. Maximum is 1044480 bytes.
Global variables use 63872 bytes (24%) of dynamic memory, leaving 198272 bytes for local variables. Maximum is 262144 bytes.
Resetting COM9
Converting to uf2, output size: 614400, start address: 0x2000
Scanning for RP2040 devices
Flashing E: (RPI-RP2)
Wrote 614400 bytes to E:/NEW.UF2
----------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.
If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

NB: Use 2MB Flash option with 1MB Sketch 1 MB FS 
    Use USB Stack Adafruit TinyUSB
    Use 200 or 125 MHz CPU when higher SPI read is used:  #define SPI_FREQUENCY      35000000  // 50MHz causes volume artifacts
                                                          #define SPI_READ_FREQUENCY 15000000  // 20 MHz also ok

New changes:
1. Added Rotary Encoder plugin with support for coded functions and symbolic link to macro actions in App folders and example Files4Twist.zip
   *tc* set Twist colours and connect *tc*RRGGBBCrCgCb RGB on rgb dimmed Cx Connect -128 to +127
   *tc* = version *tc*d,D = dimmed value *tc*X = r,R g,G b,B y,Y w,W p,P 0-9 various colour and connect options
   *tc* = version *tc*l,L = limit value l=0 no limits (version 1.0) L=24 steps limit such as -12 0 +12
   *tf* = delete file twist *tf*nameR=nameL=nameP default is twist1=twist2=twist3
   *tf*s,r = Save or Read twist config file twistCfg 
   *tm*char = vuzsxdwbVUZSXDWB code definitions or use *tm* twistMacro=0x00 Use symbolic file twist not coded macros.
   Default volume vV - *tm*char = vV uU zZ sS xX dD wW etc in vuzsxdbVUZSXDB coded macro options volume =/-/mute
   undo/redo zoom +/-/reset scroll +/- X /* x -= backspace/delete Wallaper-Next Photoshop Brush Size = b Hardness = B
   *tm*0 same as *tm* switch to file-based macros for Twist not the coded macros as in *tm*char = vuzsxdwbVUZSXDWB 
   Use Sparkfun Qwiic Twist https://www.sparkfun.com/sparkfun-qwiic-twist-rgb-rotary-encoder-breakout.html and connect to 3v3 Gnd and GPIO 26 GPIO 27 for SDA SCL
2. Support Mouse keys page on PC App
3. Added Volume and Play Media Star Codes: Use *v+* *v-* *vm* Volume + - mute or *v+,-*nn = 00-99 and *p+* *p-* *pp* *ps* Play Media Next Previous Play/Pause Stop. 
   For *v+*nn and *v-*nn the nn is not and absolute number but an increase or decrease of the existing value  divided by two - i.e. if the existing value is 40 and
   you send *v+*20 then the new volume will be 50 (not 60 or 20). To set the volume to a specific value first send <*v-*50> which will set it to 0 and mute, then send 
   <*v+*value/2> i.e. send <*v+*25> if the volume is to be set at 50 percent.
4. Expanded iList options to 20 and added colour copy defaults to iList
5. Added pre-coded M1-M24 text string as stringm24.h - use new iList number 14 to send without loading or saving the text strings - similar to Do2=1 option
6. Fixed regression errors for Timers and Clocks + 1 minute lock fix
7. Added 0xF4 and 0xF0 options to ExecuteCode()
8. Macro Instruction List execution data sent to PC App iList tab where it can be edited and sent back to Pico.
9. Labels L1, L3, L4 char L replaced by nKeys char - switch on/off with *0n*. Config1 only saved if App switching is not active (AppState==0)
10. Fix when large textfile is printed then the second action is also executed additionally when using L4. This can be corrected by choosing an alternative instruction list that excludes 
    that specific second action, but in this case DoNKeys() was modified: 
    if (StrLen==0) { if (LayerAxD) status("nKeys File not found on SDCard"); else status("nKeys File not found on Flash"); } else LinkOk = true;
11. Various fixes to <1 - <6 handling, *up*0,1 off/on Macro Upper/Lower case added, SDCardNum Pad [n] + Pad[o] used, SDCardArr[2] sent to PC App, changes to [Cfg][Sav] such as A-D only 
    saved from [A-D] not L1,L3,L4 changes.
12. MacroUL part of Config1












