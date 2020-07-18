# Makefiles for Arduino based sketches

Simple Makefiles to compile and upload Arduino sketches using a
command-line based workflow. It includes Over The Air updates for
WiFi-enabled boards.

These sample Makefiles use the *arduino-cli*, the Arduino Command Line
Interface, and has been developed and tested on Linux, and will not
run on Windows. Probably they will run on a Mac, but has not been
tested.

The simplicity of these Makefiles and some of the features and
limitations are related to the use of the *arduino-cli* command.

Two Makefile samples, almost identical, are available:

* one for an [Arduino Uno](./blink-arduino/Makefile) sample project;

* one for a [Wemos mini D1](./blink-wemos-ota/Makefile) sample
  project; this board includes a WiFi chip, so it is possible to do an
  OTA update;

To use one of these makefiles on another project:

* copy the Makefile on the arduino project directory (where the
  *".ino"* file is);
  
* change the *FQBN* (*Fully Qualified Board Name*) variable inside the
  Makefile, to the correct value for the project; the command
  *"arduino-cli board listall"* is available to get a list of
  available board names and their fqbn;

* for WiFi enabled boards change also the *IOT_NAME* variable inside
  the Makefile and in the source file, to the device hostname; this
  information will be used during Over The Air Update to select the
  correct device.

## Features and limitations

* each sketch must reside in his own folder with this Makefile and,
  eventually, other *".h"* files;

* the main make targets are:

  * **all**: compiles and upload via serial interface;
  
  * **compile**: compiles only;
  
  * **upload**: upload via serial port, compile if the binary file is
                not available;

  * **ota**: upload Over The Air, automatically find the device IP
             address using the IOT_NAME (device hostname) and the
             avahi-browse command;

  * **clean**:   clean the build directory;

  * **find**: find OTA updatable devices on the local subnet, using
             the avahi-browse command;
  
  * **requirements**: if the file "requirements.txt" exists it will
                     install the libraries listed in this file;

  * the default target is "all";
  
* it gets the name of the sketch using the *wildcard* make command;
  the name is *"\*.ino"*; this means that you must have **only** a file
  with *".ino"* extension, otherwise this makefile will break.  This
  also means that you can use this Makefile, almost unmodified, for
  any sketch as long as you keep a single *".ino"* file in each
  folder;

* It is possible to split a project in multiple files, using a single
  *".ino"* file and multiple *".h"* files, that can be included in the
  *".ino"* file with an *'#include "filename.h"'* directive;

## Environment Variables

Optionally some environment variables can be set to overwrite the
default Makefile values:

<dl>

<dt>FQBN</dt> <dd>Fully Qualified Board Name; if not set in the
                environment it will be assigned a value in this
                makefile</dd>

<dt>SERIAL_DEV</dt> <dd>Serial device to upload the sketch; if not set in the
                    environment it will be assigned:
                    <br/>/dev/ttyUSB0   if it exists, or
                    <br/>/dev/ttyACM0   if it exists, or
                    <br/>unknown</dd>

<dt>IOT_NAME</dt>  <dd>Name of the IOT device; if not set in the environment
                   it will be assigned a value in this makefile. This is
                   very useful for OTA update, the device will be searched
                   on the local subnet using this name</dd>

<dt>OTA_PORT</dt>  <dd>Port used by OTA update; if not set in the environment
                   it will be assigned the default value of 8266 in this
                   makefile</dd>

<dt>OTA_PASS</dt> <dd>Password used for OTA update; if not set in the environment
                  it will be assigned the default value of an empty string</dd>

<dt>V</dt>        <dd>verbose flag; can be 0 (quiet) or 1 (verbose); if not set
                  in the environment it will be assigned a default value
                  in this makefile</dd>
</dl>

## Usage examples

**Compile a sketch**

```
valerio@ubuntu-hp:~/Arduino/blink-wemos-ota$ make compile
FQBN       is [esp8266:esp8266:d1_mini]
IOT_NAME   is [esp8266-blink]
OTA_PORT   is [8266]
OTA_PASS   is []
V          is [0]
VFLAG      is []
MAKE_DIR   is [/home/valerio/Arduino/blink-wemos-ota]
BUILD_DIR  is [build/esp8266.esp8266.d1_mini]
SRC        is [blink-wemos-ota.ino]
HDRS       is [ota.h wifiinfo.h]
BIN        is [build/esp8266.esp8266.d1_mini/blink-wemos-ota.ino.bin]
SERIAL_DEV is [unknown]
arduino-cli compile -b esp8266:esp8266:d1_mini 
Executable segment sizes:
IROM   : 280240          - code in flash         (default or ICACHE_FLASH_ATTR) 
IRAM   : 27536   / 32768 - code in IRAM          (ICACHE_RAM_ATTR, ISRs...) 
DATA   : 1276  )         - initialized variables (global, static) in RAM/HEAP 
RODATA : 1204  ) / 81920 - constants             (global, static) in RAM/HEAP 
BSS    : 25456 )         - zeroed variables      (global, static) in RAM/HEAP 
Sketch uses 310256 bytes (29%) of program storage space. Maximum is 1044464 bytes.
Global variables use 27936 bytes (34%) of dynamic memory, leaving 53984 bytes for local variables. Maximum is 81920 bytes.
/usr/local/bin/arduino-manifest.pl
---> Generating manifest.txt
```

**Upload a sketch via Serial Port**

```
valerio@ubuntu-hp:~/Arduino/blink-wemos-ota$ make upload
FQBN       is [esp8266:esp8266:d1_mini]
IOT_NAME   is [esp8266-blink]
OTA_PORT   is [8266]
OTA_PASS   is []
V          is [0]
VFLAG      is []
MAKE_DIR   is [/home/valerio/Arduino/blink-wemos-ota]
BUILD_DIR  is [build/esp8266.esp8266.d1_mini]
SRC        is [blink-wemos-ota.ino]
HDRS       is [ota.h wifiinfo.h]
BIN        is [build/esp8266.esp8266.d1_mini/blink-wemos-ota.ino.bin]
SERIAL_DEV is [/dev/ttyUSB0]
---> Uploading sketch

No new serial port detected.
esptool.py v2.8
Serial port /dev/ttyUSB0
Connecting....
Chip is ESP8266EX
Features: WiFi
Crystal is 26MHz
MAC: 48:3f:da:0d:e9:e9
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Auto-detected Flash size: 4MB
Compressed 314416 bytes to 227814...
Wrote 314416 bytes (227814 compressed) at 0x00000000 in 5.2 seconds (effective 487.9 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
```

**Upload a sketch Over The Air**

```
valerio@ubuntu-hp:~/Arduino/blink-wemos-ota$ make ota
FQBN       is [esp8266:esp8266:d1_mini]
IOT_NAME   is [esp8266-blink]
OTA_PORT   is [8266]
OTA_PASS   is []
V          is [0]
VFLAG      is []
MAKE_DIR   is [/home/valerio/Arduino/blink-wemos-ota]
BUILD_DIR  is [build/esp8266.esp8266.d1_mini]
SRC        is [blink-wemos-ota.ino]
HDRS       is [ota.h wifiinfo.h]
BIN        is [build/esp8266.esp8266.d1_mini/blink-wemos-ota.ino.bin]
SERIAL_DEV is [/dev/ttyUSB0]
PLAT_PATH  is [/home/valerio/.arduino15/packages/esp8266/hardware/esp8266/2.7.2]
PY_PATH:   is [/home/valerio/.arduino15/packages/esp8266/tools/python3/3.7.2-post1]
IOT_IP:    is [192.168.2.91]
BINFILE:   is [build/esp8266.esp8266.d1_mini/blink-wemos-ota.ino.bin]
---> Uploading Over The Air
Uploading........................................................................................................................................................................................................................

```

**Install required libraries for a sketch**

```
valerio@ubu20:~/Arduino/meteo-persiceto/DisplayTemperature$ cat requirements.txt 
Adafruit RGB LCD Shield Library
OneWire
DallasTemperature
valerio@ubu20:~/Arduino/meteo-persiceto/DisplayTemperature$ make requirements
FQBN       is [esp8266:esp8266:d1_mini]
IOT_NAME   is [esp8266-meteo]
OTA_PORT   is [8266]
OTA_PASS   is []
V          is [0]
VFLAG      is []
MAKE_DIR   is [/home/valerio/Arduino/meteo-persiceto/DisplayTemperature]
BUILD_DIR  is [build/esp8266.esp8266.d1_mini]
SRC        is [DisplayTemperature.ino]
HDRS       is [ota.h wifiinfo.h]
BIN        is [build/esp8266.esp8266.d1_mini/DisplayTemperature.ino.bin]
SERIAL_DEV is [/dev/ttyUSB0]

---> Installing  "Adafruit RGB LCD Shield Library"
Adafruit RGB LCD Shield Library depends on Adafruit RGB LCD Shield Library@1.2.0
Downloading Adafruit RGB LCD Shield Library@1.2.0...
Adafruit RGB LCD Shield Library@1.2.0 already downloaded
Installing Adafruit RGB LCD Shield Library@1.2.0...

---> Installing  "OneWire"
OneWire depends on OneWire@2.3.5
Downloading OneWire@2.3.5...
OneWire@2.3.5 already downloaded
Installing OneWire@2.3.5...

---> Installing  "DallasTemperature"
DallasTemperature depends on DallasTemperature@3.8.0
Downloading DallasTemperature@3.8.0...
DallasTemperature@3.8.0 already downloaded
Installing DallasTemperature@3.8.0...

```

# Quickstart

## Install arduino-cli

The *arduino-cli* is a single executable file, written in *Go* and
statically linked; you can download this executable from the
[arduino-cli installation
page](https://arduino.github.io/arduino-cli/installation/) and put it
in a directory on your PATH, for example in */usr/local/bin*

Update the *arduino-cli* board index with the command:

```
$ arduino-cli core update-index
Updating index: package_index.json downloaded   
```

## Install support for your board

By default, unless you have already installed the Arduino IDE, you
don't have support files for your board. You can search available boards
for your device with a command similar to the following:

```
$ arduino-cli core search arduino
ID                Version Name                                             
Intel:arc32       2.0.4   Intel Curie Boards                               
arduino-beta:mbed 1.2.1   Arduino mbed-enabled Boards                      
arduino:avr       1.8.3   Arduino AVR Boards                               
arduino:mbed      1.1.4   Arduino nRF528x Boards (Mbed OS)                 
arduino:megaavr   1.8.6   Arduino megaAVR Boards                           
arduino:nrf52     1.0.2   Arduino nRF52 Boards                             
arduino:sam       1.6.12  Arduino SAM Boards (32-bits ARM Cortex-M3)       
arduino:samd      1.8.6   Arduino SAMD Boards (32-bits ARM Cortex-M0+)     
arduino:samd_beta 1.6.25  Arduino SAMD Beta Boards (32-bits ARM Cortex-M0+)
littleBits:avr    1.0.0   littleBits Arduino AVR Modules
```

If, for example, we have an "Arduino UNO" board we have to install
support files for "Arduino AVR Boards" with the command:

```
$ arduino-cli core install arduino:avr
Downloading packages...
arduino:avr-gcc@7.3.0-atmel3.6.1-arduino7 downloaded                                                                                                            
arduino:avrdude@6.3.0-arduino17 downloaded                                                                                                                      
arduino:arduinoOTA@1.3.0 downloaded                                                                                                                             
arduino:avr@1.8.3 downloaded                                                                                                                                    
Installing arduino:avr-gcc@7.3.0-atmel3.6.1-arduino7...
arduino:avr-gcc@7.3.0-atmel3.6.1-arduino7 installed
Installing arduino:avrdude@6.3.0-arduino17...
arduino:avrdude@6.3.0-arduino17 installed
Installing arduino:arduinoOTA@1.3.0...
arduino:arduinoOTA@1.3.0 installed
Installing arduino:avr@1.8.3...
arduino:avr@1.8.3 installed
```

## Install support files for esp8266 boards or other boards

In the default board index many boards made, by third parties, are not
included. To include them we have add the related index urls to the
config file.

1. First of all we have to generate the config file, otherwise
   *arduino-cli* will continue to use his defaults. We can generate it
   with the following command:

   ```
   $ arduino-cli config init
   Config file written to: /home/valerio/.arduino15/arduino-cli.yaml
   ```

2. Then we have to modify this configuration file to add the esp8266
   URL in the board_manager section, as shown below:

   ```
   board_manager:
     additional_urls:
     - http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```

3. Now we need to update again the board index with the command:

   ```
   $ arduino-cli core update-index
   Updating index: package_index.json downloaded
   Updating index: package_esp8266com_index.json downloaded
   ```

4. We can search our board:

   ```
   $ arduino-cli core search esp8266
   ID              Version Name   
   esp8266:esp8266 2.7.2   esp8266
   ```

4. And finally we can install the support files for our ESP8266 based
   board:

   ```
   $ arduino-cli core install esp8266:esp8266 
   Downloading packages...
   esp8266:xtensa-lx106-elf-gcc@2.5.0-4-b40a506 downloaded
   esp8266:mkspiffs@2.5.0-4-b40a506 downloaded
   esp8266:mklittlefs@2.5.0-4-fe5bb56 downloaded
   esp8266:python3@3.7.2-post1 downloaded
   esp8266:esp8266@2.7.2 downloaded
   Installing esp8266:xtensa-lx106-elf-gcc@2.5.0-4-b40a506...
   esp8266:xtensa-lx106-elf-gcc@2.5.0-4-b40a506 installed
   Installing esp8266:mkspiffs@2.5.0-4-b40a506...
   esp8266:mkspiffs@2.5.0-4-b40a506 installed
   Installing esp8266:mklittlefs@2.5.0-4-fe5bb56...
   esp8266:mklittlefs@2.5.0-4-fe5bb56 installed
   Installing esp8266:python3@3.7.2-post1...
   esp8266:python3@3.7.2-post1 installed
   Installing esp8266:esp8266@2.7.2...
   esp8266:esp8266@2.7.2 installed
   ```

If we want to use some other boards we have to repeat, and adapt, some
of the previous steps.

Our command line based development is now ready, it is possible to
clone this repository and start compiling the two sample project
available here withe the *make* command.


   


