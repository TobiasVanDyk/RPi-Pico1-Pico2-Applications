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
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad332.ino.elf"
Sketch uses 246156 bytes (11%) of program storage space. Maximum is 2088960 bytes.
Global variables use 59420 bytes (11%) of dynamic memory, leaving 464868 bytes for local variables. Maximum is 524288 bytes.
C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed-1/python3 -I C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0/tools/uf2conv.py --serial COM3 --family RP2040 --deploy I:\Data\Win10\Arduino/VolumeMacroPad332.ino.uf2 
Resetting COM3
Converting to uf2, output size: 570368, start address: 0x2000
Scanning for RP2040 devices
Flashing D: (RP2350)
Wrote 570368 bytes to D:/NEW.UF2
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
1. Can list files in folders with *lf*sdcardfolder or *lf*sdcardfolder+flashfolder. From the PC App use the two comboboxes in the Config tab to enter the folders or use / for the
root. Can select the listed /folder/filename and Delete or View content from the Comms tab. Use for example from the PC App Send List: 
<*lf*> lists all flash files and all sdcard files 
<*lf*/store/> lists all flash files and the sdcard files in folder /store.
<*lf*/store/+/new/> lists all flash files in the folder /new and the sdcard files in folder /store.
<*lf*/+/new/> lists all flash files in the folder /new and all the sdcard files (the folder is now root /).
<*lf*/+/> lists all flash files and all the sdcard files (the folder for both is now root /).
To change the serial start/stop markers to 02hex/03hex type 0x02 and 0x03 into the Comms Tab of the PC App and press [Change] then close the app, reopen and check if the values
shown are 02 and o3. Then change it on the Pico macropad using the macroEditor - enter *1s*002 [EXE], and also *1e*003, [EXE], exit the Editor and press [Cfg]->[Sav>].
2. Added S1-S24 keys double-spacing printng filter - use *cr*0-3 to filter i.e. remove, CR 0D \n and LF 0A \r during sending nKeys or the S1-S24 keys large SDCard text files.
3. Serial Comms Start and End Markers can now be changed to a different ASCII character or to any value 0-255 from the Pico and also on the PC App also to any byte value 
between 0 and 255 such as using 0x02 and 0x03 for Hex values, or enter < or > as text, in the textboxes in the new Change Start and End Marker section on the Config Tab. 
Use *1s*char for the Start marker and *1e^char for the End marker, or use *1s*charchar or *1e*charchar to change both start and end characters, or use *1s* or *1e* to reset 
both to the <> pair, or use *1s,e*000-255 to set to any value between 0 and 255 such as the 02/03 hexadecimal start/stop transmission pair. When it is changed on the Pico change
it on the PC App before syncing, and when changed on the PC App via <*1s,e*char> instead of using the single <*1s*charchar> command, rememberto keep on using < data >, as 
the translation to the new start char is done automatically when sending the *command to change the end character. Because the Pico Start and End marker settings are saved in 
the Config1 file, the values will be 0 after loading the new firmware - this condition where both are 0x00 are handled by setting them to the default < and >. Otherwiae use the 
Macro editor on the Pico and enter *1s* and save with the [Cfg]->[Sav] config button, before opening the PC App, or enter as *1s*< and *1e*> and save.
4. Added <f filecontent > to <F filecontent > from using the [Select and Send Files] multi-file select button on the bKeys tab with nKeys checked - this then saves as nChar+0+1-9 if<10 or 
nChar+10-999. Sending MST files using <1-6 data > to the SDCard is now limited to 6144 bytes instead of 200 bytes.
5. Fixed *sx*filename and *sx*folder code.
6. *sx*name where name = filename or /foldername/ or // folder = "" or ** filename = "" or if no name reset filenumber to 1. One or many files can be copied from the PC App to the 
Pico Macropad using the [Select and Send Files] button on the Comms tab. This button has a dual-function: After an intial selection of one or more files, the combobox next to the 
button is filled with the list of files sent. If one of these are selected, or a new file and its path typed into the combobox, the button when pressed will not first open a dialog 
box to select files but will send the single file selected immediately to the MacroPad.  The Pico macropad will name these files numerically as file1 to file 9999. A filename sync 
will be implemented later, for the time being use <*rn*file1=a01> for example, or use the [Ren] function in the Pico Editor itself to rename the new files. Note that the SDCard must 
be the destination i.e. A-D must be brown, these files should not be saved to Flash directly as they can be large. Use *sx* to reset the number count to 1. Use *sx*flename or *sx*/foldername/ 
to change the name used or path where files are saved - but these setting are not saved. The Pico is not a PC so when dragging more than ten or twenty files they should be small, or for 
larger files (maximum size is 6144 bytes), copy less than five at a time. This functionality is ideal for uploading a set of nKeys - for example first upload a set of 9 keys where you used 
<*sx*n0> to set the base filename, then upload the rest (up to about 980 more), with a base filename <*sx*n>. The filecount will not reset between uploads unless you use <*sx*>. To reset 
filename to null use *sx*** and use *sx*// to set foldername to null.
7. *wa* Wakeup dimmed LCD. 

Previous changes
1. New rules for <x > serial port commands to enable <m,s,t > from both SDCard and Flash modes. Refer to manual but previous m s t Flash now M S I and
   Timeset <T > for example sensordata now with <S > not <s >, Foobar now M not m. This means all <x > data the same to both SDCard and Flash modes.
1. *sf*filename send file contents of file filename over serial to PC App in filtered readable format - maximum size 6144 bytes - A-D white = flash or brown = SDCard choose media
   *sF*filename send file contents of file filename over serial to PC App in raw format - maximum size 6144 bytes. Use A-D white = flash or brown = SDCard to choose media
2. Can send Label.json files to macropad via PC App. Use the converter to convert from Arduino format to json file. Checkbox in config tab 
   if LabelM,S,T file must be automatically updated when new labelfiles are uploaded. labelfiles are saved as label1,2,3 depending if L1,l2,L3 upload used.
3. *rm*filename or use <*rm*filename> = remove filename. If //folder but folder must be empty. Delete files from Comms tab filelist (use the Config tab to s
   end the filelist), by selecting filename, right-click and select Delete File. Brown A-D file on SDCard, white A-D file on Flash.
4. PC App can now send json symbolBanks 0-9 to Pico macropad. Edit macrobanks as json files on PC.
5. Added Alt+Esc option instead of manual focus change for PC App using "*" for delay value after key pressed on PC app. 
   For example <k103*> is sent from the PC App when key [S1] is pressed with checkbox selected with Delay = *. This will then send (in my case) 
   a textstring to the next visible app which is notepad using Alt+Esc to move the focus to notepad and printthe textstring. This automatic change of focus applies to nKeys, 
   Glyphs and to the normal M,S,T and the other keys in layouts 1 to 4. You can also test these by using the Comms tab and the Send list button, and then sending <g2211*1> with 
   Word open - the Sum glyphs should be written in word. Test the normal keys by sending <k103*> and the [S1] will be sent to Word.
6. Glyph code sent to macropad with selected delay = d in <gHHHds> HHH = unicode hexcode for glyph s = 0 LCD send 1 PC app send
7. Added <*lx*s> <*lx*m> <*lx*n> <*lx*0> <*lx*k> switch directly to 5 Pads in Layout 2
8. Execute nKey when pressed on PC app executes the command on the LCD sends <npppkkkd> ppp = Page number 001-833 kkk=key number 001-996 d = delay same as for the M,S,T keys
9. Additional nKeys *commands: 
   *nd*nnd send raw keys 1-17 -> 0-16 to LCD d = delay*1000 mS (optional)
   *nf*xmmm x = nChar mmm = nKeyNumber  Send content of nkeyfile to PC App - can also use to get content of any other textfile as m = m-mmm and a-Z,0-9 and x = any character
   *np*nnn switch LCD to nKeys page on command from App - switch off with a second *np*nnn
10. Fixed furher Media *en* logic and expanded config to PC
11. Added press keys M,S,T1-M,S,T24 on TouchLCD from PC with <kabcd> a=key1--6 b=LayerAD 0-3 c=Layout 1-4 d=delay (0 to 5 seconds for focus change if text string sent). 
    Note that if LCD is dimmed it requires one keypress to wake, i.e. two keypresses before it acts. Delay is 1=100mS and 9=5 seconds. Use the checkbox + combobox in Layout 2 to configure. 










