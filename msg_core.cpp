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

//process json command
int MSG_CORE::cmd(ArduinoJson::JsonObject& cmd){
  String mycmd = cmd["cmd"].asString();
  
  if(mycmd == "api") {
    handled = 1;
    proto->printer->println("api match");
    proto->msgOpen("servo", cmd["cmd"].asString());
    proto->msgAttr("api","test");
    proto->msgSend();  
    return true;
  }
  if (mycmd == "pinMode") {    
    proto->msgOpen("pinMode","log");
    proto->msgAttr("pin",cmd["pin"].as<long>());
    proto->msgSend("pinMode");  
     
    pinMode(cmd["pin"].as<long>(), cmd["val"].as<long>());
    return true;
  }
  if (mycmd == "digitalWrite") {
    proto->msgOpen("digitalWrite","log");
    proto->msgAttr("pin",cmd["pin"].as<long>());
    proto->msgAttr("val",cmd["val"].as<long>());
    proto->msgSend("digitalWrite");
       
    digitalWrite(cmd["pin"].as<long>(), cmd["val"].as<long>());
    return true;
  }
  if (mycmd == "digitalRead") {
    int inputReading = digitalRead(cmd["pin"].as<long>());
         
    proto->msgOpen("digitalRead","out");
    proto->msgAttr("pin", cmd["pin"].as<long>());
    proto->msgAttr("val",inputReading);
    proto->msgSend("digitalRead");
    return true;
  }
  return false;
}
//process rest command
int MSG_CORE::rst(String cmd,String param1,String param2) {
  if (cmd == "pinMode") {
    proto->msgOpen("pinMode","log");
    proto->msgAttr("pin",param1);
    proto->msgAttr("mode",param2);
    proto->msgSend("pinMode");  
       
    pinMode(param1.toInt(), param2.toInt());
    return true;
  }  
  if (cmd == "digitalWrite") {
    proto->msgOpen("digitalWrite","log");
    proto->msgAttr("pin",param1);
    proto->msgAttr("val",param2);
    proto->msgSend("digitalWrite");
       
    digitalWrite(param1.toInt(), param2.toInt());
    return true;
  }
  if (cmd == "digitalRead") {
    int inputReading = digitalRead(param1.toInt());
         
    proto->msgOpen("digitalRead","out");
    proto->msgAttr("pin", param1);
    proto->msgAttr("val",inputReading);
    proto->msgSend("digitalRead");
    return true;
  }
  return false;
}
