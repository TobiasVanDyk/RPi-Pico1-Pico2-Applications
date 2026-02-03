Compiled with Pico SDK 2.21-develop, Arduino Pico 5.5.5 and included Adafruit_TinyUSB_Arduino 3.7.2, and TFTeSPI 2.5.43
Pico 1 RP2040 and 3.5inch Touch Display Module for 200MHz Raspberry Pi Pico included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
Using library Adafruit_TinyUSB_Arduino at version 3.7.2 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SdFat 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad409.ino.elf"
Sketch uses 252292 bytes (24%) of program storage space. Maximum is 1044480 bytes.
Global variables use 46596 bytes (17%) of dynamic memory, leaving 215548 bytes for local variables. Maximum is 262144 bytes.
----------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.
If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

NB: Use 2MB Flash option with 1MB Sketch 1 MB FS 
    Use USB Stack Adafruit TinyUSB
    Use 200 or 125 MHz CPU when higher SPI read is used:  #define SPI_FREQUENCY      35000000  // 50MHz causes volume artifacts
                                                          #define SPI_READ_FREQUENCY 15000000  // 20 MHz also ok

New changes:
1. Glyph code sent to macropad with selected delay = d in <gHHHds> HHH = unicode hexcode for glyph s = 0 LCD send 1 PC app send
2. Added <*lx*s> <*lx*m> <*lx*n> <*lx*0> <*lx*k> switch directly to 5 Pads in Layout 2
2. Execute nKey when pressed on PC app executes the command on the LCD sends <npppkkkd> ppp = Page number 001-833 kkk=key number 001-996 d = delay same as for the M,S,T keys
4. Additional nKeys *commands: 
   *nd*nnd send raw keys 1-17 -> 0-16 to LCD d = delay*1000 mS (optional)
   *nf*xmmm x = nChar mmm = nKeyNumber  Send content of nkeyfile to PC App - can also use to get content of any other textfile as m = m-mmm and a-Z,0-9 and x = any character
   *np*nnn switch LCD to nKeys page on command from App - switch off with a second *np*nnn
5. Fixed furher Media *en* logic and expanded config to PC
6. Added press keys M,S,T1-M,S,T24 on TouchLCD from PC with <kabcd> a=key1--6 b=LayerAD 0-3 c=Layout 1-4 d=delay (0 to 5 seconds for focus change if text string sent). Note that if LCD is dimmed it
   requires one keypress to wake, i.e. two keypresses before it acts. Delay is 1=100mS and 9=5 seconds. Use the checkbox + combobox in Layout 2 to configure. 


Previous changes:
1. Added config data to *pc*t
2. Fixed *os* and *se* and *sd*n not updating indicator LEDs
3. Fixed *e0* *e1* logic errors because Volume Mute not disabled
4. Fixed Vol1,Vol3,Vol4,VolOn label logic
5. *pc* or *pc*t send raw or text TouchLCD configuration data to a PC.
6. Fixed Starcodes not-found check=100
7. Used SD.h Arduino-Pico wrapper for old arduino SDlib instead of SDFS.h - could be side-effects but SD has easier setup than SDFS.
   Previous SDFS versions are still available.
8. Consolidate Mouse Controls by *codes - refer to section (m) in manual.h.
   Fix coding d99 for c99 in SendBytesStarCodes()
   *mm*udlr,UDLR,nn = Cursor moves 0 - 999 pixels Up Down Left Righ UDLR = 10 x udlr nn = 01-99 pixels move
   Monitor Corner for mouse zero: *mZ*n n=0,1,2,3 0=LB 1=LT 2=RT 3=RB Saved in Config1 as MouseZ. Default is LB = Left Bottom.
   Mouse Wiggler: *mw*nn and *mW*nn Mouse Wiggler non-blocking clockwise and blocking anti-clockwise, with n time in hours and nn time in minutes for active wiggler.
   Mouse Cursor Positioning: Add Mouse Position Cursor m0*nn at 0,0 if no nn, or at nn,nn or n,n on Taskbar.
   Note it assumes 4K max resolution monitor, and if a 2nd monitor attached it is on right-hand side. Mouse Position works by moving
   the cursor 2160 pixels up or down and 3840 pixels left or right, and then moving it to the XY n,n position up/down and right/left.
   Mouse buttons *mb*lrm,LRMd (lrm single-click,L=dRM doubleclick), and mouse scrollwheel *ms*du,DUnn nn=01-99 pixels and UD=10xscroll              	                            
9. Fixed History now saved after [k] exit and after [EXE] pressed (not after [Sav] and [Snd]
   Reverted clumsy [*Cm]+[ADD] keys fixes because of side-effects - if [ADD] pressed after [*Cm] press [*Cm] again
   Add green [s] Pad to save current entry in MacroEditor - Pads [h]istory -> [r]ecall after [s]ave pressed after at least one [ADD]
   Also minor cosmetic changes to key [Fsp]->[F+N] and shows F+0,9 not f01,9
10. MacroEditor history added to existing entries instead of replacing them
11. Additional macro processing options through first char 0xF2 and 0xF3. Add 0xF2 or 0xF3 through pressing key [F+N] in MacroEditor:
    If 0xF3 added as first char can construct a hex string in MacroEditor that will be sent unchanged (but without the first 0xF3) to the PC.
    For example construct [F+N]4x[ADD]D[ADD]F[ADD]8[ADD]9[ADD][Snd] and the hex characters 0xDA 0x89 will be sent to the PC. Can [Sav] or
    use [EXE] as well to save or execute and then save the string.
    If 0xF2 added as first char then macrokeys M1-M24, S1-S23, T1-T24 behave as nKeys i.e. they contain a filename that will be executed.
    For example file a01 has Ctrl+Shft+Esc, file m07 has filename a01 but with F2 at start i.e. m07 content 0xF2 0x61 0x30 0x31 when key [M7] pressed TaskManager opens
    Construct m07 in macroeditor set source to M07 white, press [F+N]3x[ADD] then a01 via [ADD] then [Sav]. File a01 is already saved in flash content 0xE0 0xE1 0x20 
12. Further fixed MacroBuff not cleared now working with [EXE] in MacroEditor
13. Fixed filenumbers > 24 for saved filename construction
14. Fixes for Function keys F1-F24 and all 17 KeyPad keys if they are the first action in the macro to be executed. As a result Macro strings starting with 0xF0, 0xF1, and 0xFF 
    are treated differently (for Function keys, KeyPad keys, and Modifier-byte respectively).   
15. New third option via [UDM] -> "Mod" key in MacroEditor to force the use of the Modifier byte for Control, Shift, Alt and Gui keys, instead of one or more of the 6 available 
    HID simultaneous keycode slots.
16. Added [KPd] key on MacroEditor for adding the 17 KeyPad keys. Read wiki for examples. Read line 886 in manual.h for 3 methods to execute em-dash.







