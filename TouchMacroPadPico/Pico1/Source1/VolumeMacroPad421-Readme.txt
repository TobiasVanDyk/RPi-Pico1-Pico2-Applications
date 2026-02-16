Compiled with Pico SDK 2.21-develop, Arduino Pico 5.5.5 and included Adafruit_TinyUSB_Arduino 3.7.2, and TFTeSPI 2.5.43
Pico 1 RP2040 and 3.5inch Touch Display Module for 200MHz Raspberry Pi Pico included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
Using library Adafruit_TinyUSB_Arduino at version 3.7.2 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.5.0\libraries\SdFat 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad422.ino.elf"
Sketch uses 253428 bytes (24%) of program storage space. Maximum is 1044480 bytes.
Global variables use 58496 bytes (22%) of dynamic memory, leaving 203648 bytes for local variables. Maximum is 262144 bytes.
Resetting COM9
Converting to uf2, output size: 579072, start address: 0x2000
Scanning for RP2040 devices
Flashing D: (RPI-RP2)
Wrote 579072 bytes to D:/NEW.UF2
----------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.
If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

NB: Use 2MB Flash option with 1MB Sketch 1 MB FS 
    Use USB Stack Adafruit TinyUSB
    Use 200 or 125 MHz CPU when higher SPI read is used:  #define SPI_FREQUENCY      35000000  // 50MHz causes volume artifacts
                                                          #define SPI_READ_FREQUENCY 15000000  // 20 MHz also ok

New changes:
1. *wa* Wakeup dimmed LCD. 

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







