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

#include "msg_core.h"
#include "json-servo.h"

int baudRate = 9600;

//features, comment out if you dont want them

//Digital input settings.
int inputPinSize = 26;
int inputPins[26];
int inputReading;
long inputTime = millis();
long inputDebounce = 20;

MSG_PROTOCOL proto(Serial);
JSON_SERVO jservo(proto);
MSG_CORE jcore(proto);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);
  proto.msgOpen("message","log");
  proto.msgAttr("lib",libVersion);
  proto.msgAttr("baud",baudRate);
  proto.msgSend();
}

int protoState = 0;
int protoPos = 0;
static char protoBuf[500];
void loop() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if(protoState == 0) {
      //scanning for a new message...
      if(c == '{') {
        //json message
        protoState = 1;
        protoBuf[0] = c;
        protoPos=1;
      } else if(c=='/') {
        //rest call
        protoState = 2;  
      }
    } else if (protoState == 1) {
        protoBuf[protoPos++] = c;
        if (c=='}') {
          protoBuf[protoPos++] = '\0';
          //end of message, process it..
          StaticJsonBuffer<500> jsonBuffer;
          JsonObject& root = jsonBuffer.parseObject(protoBuf);
          if (!root.success()) {
             proto.msgOpen("Error","Error");
             proto.msgAttr("ErrId",1);
             proto.msgAttr("ErrName","json_parse_fail");
             proto.msgSend("Error");
           } else {
             //json
             boolean handled = false;
             const String lib = root["lib"].asString();
             if (lib == "servo") {handled = jservo.cmd(root);}
             if (lib == "core") {handled = jcore.cmd(root);}
             if (!handled) {
               proto.msgOpen("Error","Error");
               proto.msgAttr("ErrId",2);
               proto.msgAttr("ErrName","unrecognised command");
               proto.msgAttr("inpute",root["cmd"].asString());
               
               proto.msgSend("Error");
             }
          }  
          protoState = 0;  
          protoPos = 0;
        }
    } else if (protoState == 2) {
      
    }
  }
}
/*
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
       
       if (lib == "servo") {handled = jservo.cmd(root);}
       if (lib == "core") {handled = jcore.cmd(root);}
       
       if (!handled) {
           proto.msgOpen("Error","Error");
           proto.msgAttr("ErrId",2);
           proto.msgAttr("ErrName","unrecognised_command");
           proto.msgAttr("lib",root["lib"].asString());
           proto.msgAttr("incmd",root["cmd"].asString());
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
*/

