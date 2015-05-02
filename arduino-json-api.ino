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

//#define TAGMSG
//#define JSONMSG
//#include "logHelper.h"
#include "msg_protocol.h"

#include "json-servo.h"


//features, comment out if you dont want them

//Digital input settings.
int inputPinSize = 26;
int inputPins[26];
int inputReading;
long inputTime = millis();
long inputDebounce = 20;
  
long msgCount = 0;
int baudRate = 9600;


MSG_PROTOCOL proto(Serial);
JSON_SERVO jservo(proto);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);
  proto.msgOpen("message","log");
  proto.msgAttr("lib",libVersion);
  proto.msgAttr("baud",baudRate);
  proto.msgSend();
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
       proto.msgOpen("Error","Error");
       proto.msgAttr("ErrId",1);
       proto.msgAttr("ErrName","json_parse_fail");
       proto.msgText(charBuf);
       proto.msgSend("Error");
     } else {
       //json
       boolean handled = false;
       const String cmd = root["cmd"].asString();
       const String lib = root["lib"].asString();
       if (lib == "servo") {jservo.cmd(root);handled=true;}
       if (cmd == "echo") {
         handled = true;
         Serial.print("echo");
       }
       if (cmd == "pinMode") {
         handled = true;
         proto.msgOpen("pinMode","log");
         proto.msgAttr("pin",root["pin"].as<long>());
         if(root["val"].as<long>() == 1) {
           proto.msgAttr("val","output");
           pinMode(root["pin"].as<long>(), OUTPUT);
           inputPins[root["pin"].as<long>()] = 0;
         } else {
           proto.msgAttr("val","input");
           pinMode(root["pin"].as<long>(), INPUT);
           inputPins[root["pin"].as<long>()] = 1;
         }
         proto.msgSend("pinMode");  
       }
       if (cmd == "digitalWrite") {
         handled = true;
         proto.msgOpen("digitalWrite","log");
         proto.msgAttr("pin",root["pin"].as<long>());
         if(root["val"].as<long>() == 1) {
           proto.msgAttr("val",1);
           digitalWrite(root["pin"].as<long>(),HIGH);
         } else {
           proto.msgAttr("val",0);
           digitalWrite(root["pin"].as<long>(),LOW);  
         }
         proto.msgSend("digitalWrite");
       }
       if (cmd == "digitalRead") {
         handled = true;
         inputReading = digitalRead(root["pin"].as<long>());
         
         proto.msgOpen("digitalRead","out");
         proto.msgAttr("pin", root["pin"].as<long>());
         proto.msgAttr("val",inputReading);
         proto.msgSend("digitalRead");
       }
       if (!handled) {
           proto.msgOpen("Error","Error");
           proto.msgAttr("ErrId",2);
           proto.msgAttr("ErrName","unrecognised_command");
           proto.msgAttr("cmd",root["cmd"].asString());
           //msgText(charBuf);
           proto.msgSend("Error");
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
           proto.msgOpen("digitalRead","interrupt");
           proto.msgAttr("pin",i);
           proto.msgAttr("val",inputReading);
           proto.msgSend("digitalRead");
       }
     }
   }
   
   delay(50); // give the Arduino some breathing room.
}


