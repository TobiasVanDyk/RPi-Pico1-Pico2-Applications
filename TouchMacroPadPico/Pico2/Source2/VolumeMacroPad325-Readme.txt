Compiled with Pico SDK 2.21-develop, Arduino Pico 5.5.5 and included Adafruit_TinyUSB_Arduino 3.7.2, and TFTeSPI 2.5.43
Pico 2 RP2350 and 3.5inch Touch Display Module for 150MHz Raspberry Pi Pico 2 included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
RP2350-E9: Adding absolute block to UF2 targeting 0x10ffff00
Multiple libraries were found for "SD.h"
 Used: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SD
 Not used: C:\Program Files (x86)\Arduino\libraries\SD
 Not used: C:\Users\Tobias\Documents\Arduino\libraries\SD
Using library Adafruit_TinyUSB_Arduino at version 3.7.2 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\LittleFS 
Using library SD at version 2.0.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SD 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SdFat 
Using library Time at version 1.6.1 in folder: C:\Users\Tobias\Documents\Arduino\libraries\Time 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad324.ino.elf"
Sketch uses 243292 bytes (11%) of program storage space. Maximum is 2088960 bytes.
Global variables use 47312 bytes (9%) of dynamic memory, leaving 476976 bytes for local variables. Maximum is 524288 bytes.
C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed-1/python3 -I C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0/tools/uf2conv.py --serial COM3 --family RP2040 --deploy I:\Data\Win10\Arduino/VolumeMacroPad324.ino.uf2 
Resetting COM3
Converting to uf2, output size: 552960, start address: 0x2000
Scanning for RP2040 devices
Flashing D: (RP2350)
Wrote 552960 bytes to D:/NEW.UF2
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
1. PC App can now send json symbolBanks 0-9 to Pico macropad. Edit macrobanks as json files on PC.
2. Added Alt+Esc option instead of manual focus change for PC App using "*" for delay value after key pressed on PC app. 
   For example <k103*> is sent from the PC App when key [S1] is pressed with checkbox selected with Delay = *. This will then send (in my case) 
   a textstring to the next visible app which is notepad using Alt+Esc to move the focus to notepad and printthe textstring. This automatic change of focus applies to nKeys, Glyphs and to the normal M,S,T and the other keys in layouts 1 to 4. You can also test these by using the Comms tab and the Send list button, and then sending <g2211*1> with Word open - the Sum glyphs should be written in word. Test the normal keys by sending <k103*> and the [S1] will be sent to Word.
3. Glyph code sent to macropad with selected delay = d in <gHHHds> HHH = unicode hexcode for glyph s = 0 LCD send 1 PC app send
4. Added <*lx*s> <*lx*m> <*lx*n> <*lx*0> <*lx*k> switch directly to 5 Pads in Layout 2
5. Execute nKey when pressed on PC app executes the command on the LCD sends <npppkkkd> ppp = Page number 001-833 kkk=key number 001-996 d = delay same as for the M,S,T keys
6. Additional nKeys *commands: 
   *nd*nnd send raw keys 1-17 -> 0-16 to LCD d = delay*1000 mS (optional)
   *nf*xmmm x = nChar mmm = nKeyNumber  Send content of nkeyfile to PC App - can also use to get content of any other textfile as m = m-mmm and a-Z,0-9 and x = any character
   *np*nnn switch LCD to nKeys page on command from App - switch off with a second *np*nnn
7. Fixed furher Media *en* logic and expanded config to PC
8. Added press keys M,S,T1-M,S,T24 on TouchLCD from PC with <kabcd> a=key1--6 b=LayerAD 0-3 c=Layout 1-4 d=delay (0 to 5 seconds for focus change if text string sent). Note that if LCD is dimmed it
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







