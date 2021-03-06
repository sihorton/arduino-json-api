#include <Servo.h>

// Copyright Simon Horton 2015
// MIT License
//
// Arduino Json Api
// https://github.com/sihorton/arduino-json-api
//#define MSG_SUPPORT_JSON 1

String libVersion = "v0.1";

#include <ArduinoJson.h>
#include "msg_protocol.h"

#include "msg_core.h"
#include "json-servo.h"

int baudRate = 9600;

MSG_PROTOCOL proto(Serial);
MSG_CORE jcore(proto);
JSON_SERVO jservo(proto);


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
#ifdef MSG_SUPPORT_JSON
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
#endif        
    } else if (protoState == 2) {
      if(c == '\n') {
        protoBuf[protoPos++] = '\0';
        String lib;
        lib = strtok (protoBuf,"/");
        String cmd = strtok(NULL,"/");
        String param1 = strtok(NULL,"/");
        String param2 = strtok(NULL,"/");
        int handled = 0;
        if (lib == "servo") {handled = jservo.rst(cmd,param1,param2);}
        if (lib == "core") {handled = jcore.rst(cmd,param1,param2);}
        if (!handled) {
           proto.msgOpen("Error","Error");
           proto.msgAttr("ErrId",2);
           proto.msgAttr("ErrName","unknown_command");           
           proto.msgSend("Error");
         }
        protoState = 0;  
        protoPos = 0;
      } else {
        protoBuf[protoPos++] = c;
      }
    }
  }
  jcore.monitorPins();
  
}

