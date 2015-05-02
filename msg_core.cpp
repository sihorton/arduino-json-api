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

//convert json to ordered parameters call.
int MSG_CORE::cmd(ArduinoJson::JsonObject& cmd){
  String mycmd = cmd["cmd"].asString();
  
  if (mycmd == "pinMode") return this->rst(mycmd,(String)cmd["pin"].as<long>(), (String)cmd["val"].as<long>());
  if (mycmd == "digitalWrite") return this->rst(mycmd,(String)cmd["pin"].as<long>(), (String)cmd["val"].as<long>());
  if (mycmd == "digitalRead") return this->rst(mycmd,(String)cmd["pin"].as<long>(), "");
  
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
