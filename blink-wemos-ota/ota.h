// Include what is needed to implement Over The Air Update
// The WiFi connection is not done on this include file, so must be done
// in the main program file
//
// This include file provides the functions
//    setupWifi(hostname)
//       hostname can be an empty string; this function will
//       establish the wifi connections, WiFi credentials are supplied
//       in the file "wifiinfo.h", see "wifiinfo.h.sample"
//    setupOTA(hostname)
//       hostname can be an empty string; will be used to retrieve the
//       correct board to upgrade Over The Air.
//       If an empty string the default name is esp8266-[ChipID]
// The above 2 functions must be called in the setup() function
//
// in the loop function the function
//    ArduinoOTA.handle()
// must be called

#ifndef ota_h
#define ota_h
// needed include files
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// include our Network SSID and PASSWORD
#include "wifiinfo.h"

// ==================================================================
// Establish WiFi connection
// ==================================================================
void setupWifi(char hostname[]) {
  int i = 0;
  WiFi.mode(WIFI_STA);
  if (sizeof(hostname) > 0) {
    WiFi.hostname(hostname);
  }
  if (Serial) {Serial.println("Connecting ");}
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    if (Serial) {Serial.print(".");}
    delay(500);
  }
  if (Serial) {
    Serial.println(".");
    Serial.print("Connected! IP: ");
    Serial.println(WiFi.localIP());
  }
}


// ==================================================================
// Setup for OTA update
// ==================================================================

void setupOTA(char hostname[]) {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  if (sizeof(hostname) > 0) {
    ArduinoOTA.setHostname(hostname);
  }

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
      if (Serial) {Serial.println("Start");}
  });
  ArduinoOTA.onEnd([]() {
      if (Serial) {Serial.println("\nEnd");}
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      if (Serial) {Serial.printf("Progress: %u%%\r", (progress / (total / 100)));}
  });
  ArduinoOTA.onError([](ota_error_t error) {
      if (Serial) {
	Serial.printf("Error[%u]: ", error);
	if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
	else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
	else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
	else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
	else if (error == OTA_END_ERROR) Serial.println("End Failed");
      }
  });
  ArduinoOTA.begin();
  if (Serial) {
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}


#endif
