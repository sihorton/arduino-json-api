// Copyright Simon Horton 2015
// MIT License
//
// Arduino Json Api
// https://github.com/sihorton/arduino-json-api
String libVersion = "v0.1";

#include <ArduinoJson.h>
#define DEBUG
#define TAGLOG
#define JSONLOG

#define TAGMSG
//#define JSONMSG
#include "logHelper.h"


//Digital input settings.
int inputPinSize = 26;
int inputPins[26];
int inputReading;
long inputTime = millis();
long inputDebounce = 20;
  
long msgCount = 0;
int baudRate = 9600;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);
  msgOpen("message","log");
  msgAttr("lib",libVersion);
  msgAttr("baud",baudRate);
  msgSend("message");
}

void loop() {
  //declare input buffer and json buffer sizes.
  StaticJsonBuffer<500> jsonBuffer;
  char charBuf[500];
   
  int i = 0;
  while (Serial.available()) {
     delay(3); //delay to allow buffer to fill
     if(Serial.available() > 0) {
       charBuf[i++] = (char)Serial.read();
     }
   }
   if(i > 0) {
     JsonObject& root = jsonBuffer.parseObject(charBuf);
     if (!root.success()) {
       msgOpen("Error","Error");
       msgAttr("ErrId",1);
       msgAttr("ErrName","json_parse_fail");
       msgText(charBuf);
       msgSend("Error");
     } else {
       //json
       boolean handled = false;
       const String cmd = root["cmd"].asString();
       if (cmd == "echo") {
         handled = true;
         Serial.print("echo");
       }
       if (cmd == "pinMode") {
         handled = true;
         msgOpen("pinMode","log");
         msgAttr("pin",root["pin"].as<long>());
         if(root["val"].as<long>() == 1) {
           msgAttr("val","output");
           pinMode(root["pin"].as<long>(), OUTPUT);
           inputPins[root["pin"].as<long>()] = 0;
         } else {
           msgAttr("val","input");
           pinMode(root["pin"].as<long>(), INPUT);
           inputPins[root["pin"].as<long>()] = 1;
         }
         msgSend("pinMode");  
       }
       if (cmd == "digitalWrite") {
         handled = true;
         msgOpen("digitalWrite","log");
         msgAttr("pin",root["pin"].as<long>());
         if(root["val"].as<long>() == 1) {
           msgAttr("val",1);
           digitalWrite(root["pin"].as<long>(),HIGH);
         } else {
           msgAttr("val",0);
           digitalWrite(root["pin"].as<long>(),LOW);  
         }
         msgSend("digitalWrite");
       }
       if (cmd == "digitalRead") {
         handled = true;
         inputReading = digitalRead(root["pin"].as<long>());
         
         msgOpen("digitalRead","out");
         msgAttr("pin", root["pin"].as<long>());
         msgAttr("val",inputReading);
         msgSend("digitalRead");
       }
       if (!handled) {
           msgOpen("Error","Error");
           msgAttr("ErrId",2);
           msgAttr("ErrName","unrecognised_command");
           msgAttr("cmd",root["cmd"].asString());
           //msgText(charBuf);
           msgSend("Error");
       }
     }
   }
   //check any input pins.
   for(int i=0; i<inputPinSize; i++) {
     if (inputPins[i] > 0) {
       inputReading = digitalRead(i);
       if(inputReading != (inputPins[i] - 1) && (millis() - inputTime > inputDebounce)) {
           inputTime = millis();
           inputPins[i] = 1 + inputReading;
           msgOpen("digitalRead","interrupt");
           msgAttr("pin",i);
           msgAttr("val",inputReading);
           msgSend("digitalRead");
       }
     }
   }
   
   delay(50); // give the Arduino some breathing room.
}


