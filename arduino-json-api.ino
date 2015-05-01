#include <ArduinoJson.h>
String libVersion = "Protocol v0.1";

#define DEBUG
#define TAGLOG
#define JSONLOG
#include "logHelper.h"

//Digital input settings.
int inputPinSize = 26;
int inputPins[26];
int inputReading;
long inputTime = millis();
long inputDebounce = 20;
  
long msgCount = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tagLogln("<message>protocol running " + libVersion + "</message>");
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
       tagLog("<Error id='1' name='json_parse_fail'>");
       tagLog(charBuf);
       tagLogln("</Error>");
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
         tagLog("<pinMode pin='");
         tagLog(root["pin"].as<long>());
         tagLog("' val='");
         if(root["val"].as<long>() == 1) {
           tagLog("output'");
           pinMode(root["pin"].as<long>(), OUTPUT);
           inputPins[root["pin"].as<long>()] = 0;
         } else {
           tagLog("input'");
           pinMode(root["pin"].as<long>(), INPUT);
           inputPins[root["pin"].as<long>()] = 1;
         }
         tagLogln("/>");  
       }
       if (cmd == "digitalWrite") {
         handled = true;
         tagLog("<digitalWrite pin='");
         tagLog(root["pin"].as<long>());
         tagLog("' ");
         if(root["val"].as<long>() == 1) {
           tagLog(" val='high'");
           digitalWrite(root["pin"].as<long>(),HIGH);
         } else {
           tagLog(" val='low'");
           digitalWrite(root["pin"].as<long>(),LOW);  
         }
         tagLogln("/>");
       }
       if (!handled) {
         tagLog("<error id='2' name='unrecognised_command' val='");   
          tagLog(root["cmd"].asString());
          tagLog("' >");
          tagLog(charBuf);
          tagLog("</error>");
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
           tagLog("<digitalRead pin='");
           tagLog(i);
           tagLog("'>");
           tagLog(inputReading);
           tagLogln("</digitalRead>");

           jsonNewMsg("digitalRead");
           jsonMsg("pin",i);
           jsonMsgln("val",inputReading);
       }
     }
   }
   
   delay(50); // give the Arduino some breathing room.
}


