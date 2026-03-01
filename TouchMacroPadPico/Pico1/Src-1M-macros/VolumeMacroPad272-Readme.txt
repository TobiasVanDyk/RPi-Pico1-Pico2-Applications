Compiled with Pico SDK 2.21-develop, Arduino Pico 5.5.5 and included Adafruit_TinyUSB_Arduino 3.7.2, and TFTeSPI 2.5.43
Pico 1 RP2040 and 3.5inch Touch Display Module for 200MHz Raspberry Pi Pico included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
Using library Adafruit_TinyUSB_Arduino at version 3.7.2 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SdFat 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad272.ino.elf"
Sketch uses 256068 bytes (24%) of program storage space. Maximum is 1044480 bytes.
Global variables use 58672 bytes (22%) of dynamic memory, leaving 203472 bytes for local variables. Maximum is 262144 bytes.
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
