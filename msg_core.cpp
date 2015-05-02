/**
* Json API for access to core arduino library
* http://www.arduino.cc/en/Tutorial/Foundations
*
**/

#include "msg_core.h"

//<<constructor>>
MSG_CORE::MSG_CORE(MSG_PROTOCOL &protoer){
    proto = &protoer;//operate on address of print
}

//<<destructor>>
MSG_CORE::~MSG_CORE(){/*nothing to destruct*/}

//process the command
int MSG_CORE::cmd(ArduinoJson::JsonObject& cmd){
  int handled = 0;
  String mycmd = cmd["cmd"].asString();
  
  if(mycmd == "api") {
    handled = 1;
    proto->printer->println("api match");
    proto->msgOpen("servo", cmd["cmd"].asString());
    proto->msgAttr("api","test");
    proto->msgSend();  
  }
  if (mycmd == "pinMode") {
    handled = true;
     
    proto->msgOpen("pinMode","log");
    proto->msgAttr("pin",cmd["pin"].as<long>());
    proto->msgSend("pinMode");  
     
    pinMode(cmd["pin"].as<long>(), cmd["val"].as<long>());
  }
  if (mycmd == "digitalWrite") {
    handled = true;
       
    proto->msgOpen("digitalWrite","log");
    proto->msgAttr("pin",cmd["pin"].as<long>());
    proto->msgAttr("val",cmd["val"].as<long>());
    proto->msgSend("digitalWrite");
       
    digitalWrite(cmd["pin"].as<long>(), cmd["val"].as<long>());
  }
  if (mycmd == "digitalRead") {
    handled = true;
    int inputReading = digitalRead(cmd["pin"].as<long>());
         
    proto->msgOpen("digitalRead","out");
    proto->msgAttr("pin", cmd["pin"].as<long>());
    proto->msgAttr("val",inputReading);
    proto->msgSend("digitalRead");
  }
  return handled;
}
