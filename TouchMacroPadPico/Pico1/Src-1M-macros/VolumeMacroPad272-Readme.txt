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
Sketch uses 259332 bytes (24%) of program storage space. Maximum is 1044480 bytes.
Global variables use 62932 bytes (24%) of dynamic memory, leaving 199212 bytes for local variables. Maximum is 262144 bytes.
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
1. Various fixes to <1 - <6 handling, *up*0,1 off/on Macro Upper/Lower case added, SDCardNum Pad [n] + Pad[o] used, SDCard size + used added to data, SDCardArr[2] sent to 
   PC App, changes to [Cfg][Sav] such as A-D only saved from [A-D] not L1,L3,L4 changes.
2. MacroUL part of Config1
3, Added Custom Labels M,S,T to config  data sent to PC App.
4. Added Custom Labels Off/On *lm,s,t*0,1 switch custem labels off/on - *lm,s,t*x <> 0,1 toggle state on/off
5. Fixed Labels Folder switching in Star Codes function and added nDir = AppDir switch. Reset nDir to / after App Switch closed.
6. Added App Switch function - the PC App will send the name of the opened program that has focus and is on the PC App internal list (wip)
   *ap*appname=1,3,4 creates a folder /appname/ if it does not exist on the SDCard and assigns appname to the keys in Layout 1,3,4. Pressing the keys M1-M24 or *S1-S24 or 
   T1-T24 will then execute macros files such as t01 to t24 inside the SDCard folder /appname/. *ap*appname=0 switches appname app switch off. 
   *ap*1,3,4 assigns app switch to Layouts 1, 3, or 4 = M S T keys. 
   *ap* disables App Switch function
7. Fixed WriteTimers error. Fixed Pico 2 hhmm format it now triggers when using Macro Timers [R-C] and [O-C] using both long format and short format. Fixed mistake in using old 
function WriteMacroTimers() instead of new WriteTimers().
The set of Macro Timers have been mostly completed. What is left is to assign a function to the [Rep]eat key, decide on how to handle repeating-clock timers (i.e. should they 
ignore the date and only use the time etc), and to decide how to differentiate meaningfully between the [RcT], [R-C], [OcT], and [O-C] timers. (Note that the Time and Clock handlers 
for Pico 1 is using its HW RTC where Sunday = 0, and the Pico 2 is using TimeLib.h where Sunday = 1.) Use < t*mnnn> or < t*mnnn> to program the timer-macro link where t = timer 1-8 
and m = 0 - 5 i.e. 012345 = mstakn, For example m = macro M1-M24, same for s and t, a = macrofiles a01-a99, k = Linkfiles Knnlink, n = nKeys 001-999 or 01-99 - when executing the 
timer will use the current nChar = a-zA-Z-0-9 if a the star after the timer number is a space, and if it is an alphanumeric that will be used as the nKeys character. The additional *
is for a Link file, replace it with a space if a Link file is not used, or a different nChar. The Macrotimer runs files not macro keys i.e. to run macro S01 there must be a file s01 
either on the SDCard or Flash. After sending t*values press [Cfg]->[mCT] and then press the timer button that corresponds to the number t = 1-8. Because the same code is used in the 
macro-editor enter nn or nnn as one less than the macro key value i.e. enter nn-00 for key M01. To run one-shot timer [O-t] with K11Link which is on the SD Card and all the files 
linked in K11Link are also on the SDCard, send 4*410 and then press [O-t]. Sending 2 100 will run the repeat timer 2 [R-t] on macro S01. 2 000 will run the one-shot timer 2 [O-t] on 
macro M01 provided there is a file named m01 on the SD Card or Flash. 2 100 will do the same with macro file s01. To set the macro Clock time timers three steps are needed: 
(1) First link a macro with the one or more of the clock timers 5 - 8. For example 5 100 will link macro file S01 to Timer 5 [R-C]. 
(2) Then send 26030771410 which will set alarm timers 5 and 6 = [R-C] and [O-C] to 14h10 and 
(3) press [R-C] on the LCD (or app if the "use other keys" had been selected). 
At 14h10 macro file S01 will trigger once only, because the full date will never again occur. For a 24 hour cycle repeat use the short form hhmm instead of the full date 7 March 2026:
Press [Looad values 3x] until hhmm show, replace it with the trigger hours and minutes hhmm such as 1410, press enter, and finally press the correspomding timer key on the Macrotimer page. 
Remember the < > and also the *Commands are are not required, they are added automatically.
Use 8 100 then 26030771435 and press [OcT] then at 14h35 S01 will trigger a oneshot macro S01. Using the button [Data] in the Config tab of the PC App will show the state of the 8 timers. 
When using the shorter format hours + minutes time setting, first send 5 100 as before to link S01 with Timer 5 [R-C], then send 0854 - you only enter the hhmm = 0854 the app adds the 
<*ta* R> - and then press key [R-C], to trigger S01 at 08h54 in repeat mode. The Pico 1's RTC HW only allows for setting one alarm callback at a time.
Changed Time and Clock handlers for Pico 1 (using its RTC where Sunday=0), and Pico 2 (using TimeLib.h where Sunday=1). Power Timers Clock-Restart and Clock-PowerOff functional with a 
choice of using shorter time-set options hours and minutes as hhmm (i.e. 24 hours max time-span) by using cthhmmR,O, or using a full date + time < Pyymmddwhhmm >. To test set 
time using PC App then [Cfg]->[ROf] type in top grey box below [R-C] and [O-C] buttons current time + 3 minutes for example 1630 if time is 16h27. and press enter. Then if both 
MST and Other Keys are checked and Delay = 0 in Layout L2, press [R-C] key either in the PC App or on the TouchLCD. After the first enter the LCD will display "Restart Clock ON" 
and after [R-C] pressed it will display "Restart on Clock" - leave the LCD on and after 3 minutes it will open the run box and type the reboot command.
8. Can list files in folders with *lf*sdcardfolder or *lf*sdcardfolder+flashfolder. From the PC App use the two comboboxes in the Config tab to enter the folders or use / for the
root. Can select the listed /folder/filename and Delete or View content from the Comms tab. Use for example from the PC App Send List: 
<*lf*> lists all flash files and all sdcard files 
<*lf*/store/> lists all flash files and the sdcard files in folder /store.
<*lf*/store/+/new/> lists all flash files in the folder /new and the sdcard files in folder /store.
<*lf*/+/new/> lists all flash files in the folder /new and all the sdcard files (the folder is now root /).
<*lf*/+/> lists all flash files and all the sdcard files (the folder for both is now root /).
To change the serial start/stop markers to 02hex/03hex type 0x02 and 0x03 into the Comms Tab of the PC App and press [Change] then close the app, reopen and check if the values
shown are 02 and o3. Then change it on the Pico macropad using the macroEditor - enter *1s*002 [EXE], and also *1e*003, [EXE], exit the Editor and press [Cfg]->[Sav>].

Previous changes
1. Added S1-S24 keys double-spacing printng filter - use *cr*0-3 to filter i.e. remove, CR 0D \n and LF 0A \r during sending nKeys or the S1-S24 keys large SDCard text files.
2. Serial Comms Start and End Markers can now be changed to a different ASCII character or to any value 0-255 from the Pico and also on the PC App also to any byte value 
between 0 and 255 such as using 0x02 and 0x03 for Hex values, or enter < or > as text, in the textboxes in the new Change Start and End Marker section on the Config Tab. 
Use *1s*char for the Start marker and *1e^char for the End marker, or use *1s*charchar or *1e*charchar to change both start and end characters, or use *1s* or *1e* to reset 
both to the <> pair, or use *1s,e*000-255 to set to any value between 0 and 255 such as the 02/03 hexadecimal start/stop transmission pair. When it is changed on the Pico change
it on the PC App before syncing, and when changed on the PC App via <*1s,e*char> instead of using the single <*1s*charchar> command, rememberto keep on using < data >, as 
the translation to the new start char is done automatically when sending the *command to change the end character. Because the Pico Start and End marker settings are saved in 
the Config1 file, the values will be 0 after loading the new firmware - this condition where both are 0x00 are handled by setting them to the default < and >. Otherwiae use the 
Macro editor on the Pico and enter *1s* and save with the [Cfg]->[Sav] config button, before opening the PC App, or enter as *1s*< and *1e*> and save.
3. Added <f filecontent > to <F filecontent > from using the [Select and Send Files] multi-file select button on the bKeys tab with nKeys checked - this then saves as nChar+0+1-9 if<10 or 
nChar+10-999. Sending MST files using <1-6 data > to the SDCard is now limited to 6144 bytes instead of 200 bytes.
4. Fixed *sx*filename and *sx*folder code.
5. *sx*name where name = filename or /foldername/ or // folder = "" or ** filename = "" or if no name reset filenumber to 1. One or many files can be copied from the PC App to the 
Pico Macropad using the [Select and Send Files] button on the Comms tab. This button has a dual-function: After an intial selection of one or more files, the combobox next to the 
button is filled with the list of files sent. If one of these are selected, or a new file and its path typed into the combobox, the button when pressed will not first open a dialog 
box to select files but will send the single file selected immediately to the MacroPad.  The Pico macropad will name these files numerically as file1 to file 9999. A filename sync 
will be implemented later, for the time being use <*rn*file1=a01> for example, or use the [Ren] function in the Pico Editor itself to rename the new files. Note that the SDCard must 
be the destination i.e. A-D must be brown, these files should not be saved to Flash directly as they can be large. Use *sx* to reset the number count to 1. Use *sx*flename or *sx*/foldername/ 
to change the name used or path where files are saved - but these setting are not saved. The Pico is not a PC so when dragging more than ten or twenty files they should be small, or for 
larger files (maximum size is 6144 bytes), copy less than five at a time. This functionality is ideal for uploading a set of nKeys - for example first upload a set of 9 keys where you used 
<*sx*n0> to set the base filename, then upload the rest (up to about 980 more), with a base filename <*sx*n>. The filecount will not reset between uploads unless you use <*sx*>. To reset 
filename to null use *sx*** and use *sx*// to set foldername to null.
6. *wa* Wakeup dimmed LCD.  


