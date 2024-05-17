# Makefile for Arduino based scketches
#
# Copyright 2020 Valerio Di Giampietro http://va.ler.io v@ler.io
# MIT License - see License.txt file
#
# This Makefile uses the arduino-cli, the Arduino command line interface
# and has been designed and tested to run on Linux, not on Windows.
# Probably it will run on a Mac, but it has not been tested.
#
# Please note that:
#
#   1. each sketch must reside in his own folder with this Makefile
#
#   2. the main make targets are:
#      - all     compiles and upload
#      - compile compiles only
#      - upload  upload via serial port, compile if the binary file is
#                not available
#      - ota     upload Over The Air, automatically find the device
#                IP address using the IOT_NAME (device hostname)
#      - clean   clean the build directory
#      - find    find OTA updatable devices on the local subnet
#      - requirements it the file "requirements.txt" exists it will
#                     install the libraries listed in this file
#
#      default is "all"
#
#   3. it gets the name of the sketch using the wildcard make command;
#      the name is *.ino; this means that you must have ONLY a file
#      with .ino extension, otherwise this makefile will break.  This
#      also means that you can use this Makefile, almost unmodified,
#      for any sketch as long as you keep a single .ino file in each
#      folder
#
#   4. you can split your project in multiple files, if you wish,
#      using a single .ino file and multiple .h files, that you can
#      include in the .ino file with an '#include "myfile.h"'
#      directive
#
# Optionally some environment variables can be set:
#
#   FQBN        Fully Qualified Board Name; if not set in the environment
#               it will be assigned a value in this makefile
#
#   SERIAL_DEV  Serial device to upload the sketch; if not set in the
#               environment it will be assigned:
#               /dev/ttyUSB0   if it exists, or
#               /dev/ttyACM0   if it exists, or
#               unknown
#
#   IOT_NAME    Name of the IOT device; if not set in the environment
#               it will be assigned a value in this makefile. This is
#               very useful for OTA update, the device will be searched
#               on the local subnet using this name
#
#   OTA_PORT    Port used by OTA update; if not set in the environment
#               it will be assigned the default value of 8266 in this
#               makefile
#
#   OTA_PASS    Password used for OTA update; if not set in the environment
#               it will be assigned the default value of an empty string
#
#   V           verbose flag; can be 0 (quiet) or 1 (verbose); if not set
#               in the environment it will be assigned a default value
#               in this makefile
#
#   CUSTOM_LIBS optional custom libraris in the from
#               "--libraries libpath1,libpath2,..." by default it is
#               the empty string or the "--libraries libraries" if this
#               dir exists

OSFAMILY    := $(shell ( uname | sed "s/-.*//" ))
MAKE_DIR    := $(PWD)
GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always --tags)
#
# in vars.mak usually some or all of the following variables are defined
#    FQBN
#    IOT_NAME
#    OTA_PORT
#    OTA_PASS
#    SERIAL_DEV
-include vars.mak

#
# ----- setup for ESP32 LilyGo T3 v 1.6.1
#FQBN        ?= esp32:esp32:lilygo_t_display_s3

# ----- setup wor Wemos D1 mini -----
#FQBN       ?= esp8266:esp8266:d1_mini

# ----- setup wor ESP32 NodeMCU -----
#FQBN        ?= esp32:esp32:esp32

# ----- setup for Arduino Uno
FQBN        ?= arduino:avr:uno

#IOT_NAME    ?= blink-arduino
#OTA_PORT   ?= 3232
#OTA_PASS   ?=
#
# --- Upload Speed Label: the "arduino-cli upload" command uses "baud" or UploadSpeed to specify
#     the board upload speed, the following variable will heve the correct value
SPEEDLABEL = $(shell arduino-cli board details -f -b $(FQBN) --format json | egrep -i 'baud|UploadSpeed' |tr '"' ' ' | awk '{print $$3}')

ifdef GIT_VERSION
  CFLAGS = --build-property compiler.cpp.extra_flags=-DMYVERSION=\"$(GIT_VERSION)\"
else
  CFLAGS =
endif

V          ?= 0
VFLAG      =

ifeq "$(V)" "1"
VFLAG      =-v
endif


ifndef SERIAL_DEV
  ifneq (,$(wildcard /dev/ttyUSB0))
    SERIAL_DEV = /dev/ttyUSB0
  else ifneq (,$(wildcard /dev/ttyACM0))
    SERIAL_DEV = /dev/ttyACM0
  else ifneq (,$(wildcard /dev/cu.usbserial*))
    SERIAL_DEV = $(wildcard /dev/cu.usbserial*)
  else
    SERIAL_DEV = unknown
  endif
endif



ifndef CUSTOM_LIBS
  ifneq (,$(wildcard libraries))
    CUSTOM_LIBS = --libraries libraries
  else
    CUSTOM_LIBS = 
  endif
endif

BUILD_DIR  := $(subst :,.,build/$(FQBN))

SRCINO     := $(wildcard *.ino)
SRC        := $(wildcard *.ino *.c *.cpp mylib/*/*.ino) 
HDRS       := $(wildcard *.h mylib/*/*.h)
BIN        := $(BUILD_DIR)/$(SRCINO).bin
ELF        := $(BUILD_DIR)/$(SRCINO).elf

$(info OSFAMILY    is [${OSFAMILY}])
$(info GIT_VERSION is [${GIT_VERSION}])
$(info FQBN        is [${FQBN}])
$(info IOT_NAME    is [${IOT_NAME}])
$(info IOT_IP      is [${IOT_IP}])
$(info OTA_PORT    is [${OTA_PORT}])
$(info OTA_PASS    is [${OTA_PASS}])
$(info V           is [${V}])
$(info VFLAG       is [${VFLAG}])
$(info MAKE_DIR    is [${MAKE_DIR}])
$(info BUILD_DIR   is [${BUILD_DIR}])
$(info SRCINO      is [${SRCINO}])
$(info SRC         is [${SRC}])
$(info HDRS        is [${HDRS}])
$(info BIN         is [${BIN}])
$(info SERIAL_DEV  is [${SERIAL_DEV}])
$(info CUSTOM_LIBS is [${CUSTOM_LIBS}])
$(info SPEEDLABEL  is [${SPEEDLABEL}])


all: $(ELF) upload
.PHONY: all

compile: $(ELF)
.PHONY: compile

$(ELF): $(SRC) $(HDRS)
	arduino-cli compile -b $(FQBN) --build-path $(BUILD_DIR) $(VFLAG) $(CUSTOM_LIBS) $(CFLAGS)
	@if which arduino-manifest.pl; \
	then echo "---> Generating manifest.txt"; \
	arduino-manifest.pl -b $(FQBN) $(SRC) $(HDRS) > manifest.txt.new; \
	if diff manifest.txt manifest.txt.new > /dev/null; \
	then echo "---> manifest.txt is up to date (has not changed)"; \
	rm -f manifest.txt.new; \
	else mv -f manifest.txt.new manifest.txt; \
	fi; \
	else echo "---> If you want to generate manifest.txt, listing used libraries and their versions,"; \
	echo "---> please install arduino-manifest, see https://github.com/digiampietro/arduino-manifest"; \
	fi

upload: compile
	@if [ ! -c $(SERIAL_DEV) ] ; \
	then echo "---> ERROR: Serial Device not available, please set the SERIAL_DEV environment variable" ; \
	else echo "---> Uploading sketch\n"; \
	arduino-cli upload -b $(FQBN):$(SPEEDLABEL)=115200 -p $(SERIAL_DEV)  --input-dir $(BUILD_DIR) $(VFLAG) ; \
	fi

ota: compile
	@PLAT_PATH=`arduino-cli compile -b $(FQBN) --show-properties | grep '^runtime.platform.path' | awk -F= '{print $$2}'` ; \
	   PY_PATH=`which python3 | xargs dirname` ; \
	if [[ "$(OSFAMILY)" = "Linux" && "$(IOT_IP)" = "" ]] ; \
	then IOT_IP=`avahi-browse _arduino._tcp --resolve --parsable --terminate|grep -i ';$(IOT_NAME);'|grep ';$(OTA_PORT);'| awk -F\; '{print $$8}'|head -1`; \
	elif [[ "$(OSFAMILY)" = "Darwin" && "$(IOT_IP)" = "" ]] ; \
	then IOT_IP=`(timeout 3 dns-sd -G v4 $(IOT_NAME).local | grep $(IOT_NAME).local)  | awk '{print $$6}'` ; \
	fi ; \
	BINFILE=$(wildcard $(BIN)); \
	echo "PLAT_PATH   is [$$PLAT_PATH]" ; \
	echo "PY_PATH:    is [$$PY_PATH]"  ; \
	echo "IOT_IP:     is [$$IOT_IP]"   ; \
	echo "BINFILE:    is [$$BINFILE]"  ; \
	if [ "$$IOT_IP" = "" ] ; \
	then if [ "$(OSFAMILY)" = "Linux" -o "$(OSFAMILY)" = "Darwin" ] ; \
	then echo "Unable to find device IP. Check that the IOT_NAME environment variable is correctly set. Use 'make find' to search devices"; \
	else echo "IOT_IP variable not set, you have to set this variable to the IP address of your device"; \
	fi; \
	else echo "---> Uploading Over The Air"; \
	$$PY_PATH/python3 $$PLAT_PATH/tools/espota.py -i $$IOT_IP -p $(OTA_PORT) --auth=$(OTA_PASS) -f $$BINFILE ;\
	fi

clean:
	@echo "---> Cleaning the build directory"
	rm -rf build

find:
	@if [ "$(OSFAMILY)" = "Linux" ] ;   \
	then  avahi-browse _arduino._tcp --resolve --parsable --terminate ; \
	elif [ "$(OSFAMILY)" = "Darwin" ] ; \
	then  timeout 5 dns-sd -B _arduino._tcp ; echo; \
	else  echo "---> In this OS *find* is not supported; lease set the IOT_IP environment variable to the IP address of your device" ; \
	fi

requirements:
	@if [ -e requirements.txt ]; \
	then while read -r i ; do echo ; \
	  echo "---> Installing " '"'$$i'"' ; \
	  arduino-cli lib install "$$i" ; \
	done < requirements.txt ; \
	else echo "---> MISSING requirements.txt file"; \
	fi



