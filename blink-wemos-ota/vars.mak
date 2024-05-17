#
# ----- setup for ESP32 LilyGo T3 v 1.6.1
#FQBN        ?= esp32:esp32:lilygo_t_display_s3

# ----- setup wor Wemos D1 mini -----
FQBN       ?= esp8266:esp8266:d1_mini

# ----- setup wor ESP32 NodeMCU -----
#FQBN        ?= esp32:esp32:esp32

# ----- setup for Arduino Uno
#FQBN        ?= arduino:avr:uno

# ----- following setup for WiFi enabled devices
IOT_NAME   ?= esp8266-blink

# ----- OTA_PORT is 8266 for esp8266 and 3232 for esp32
OTA_PORT   ?= 8266
OTA_PASS   ?=

# optional SERIAL_DEV when is not autodetected
SERIAL_DEV  ?= /dev/ttyUSB0
