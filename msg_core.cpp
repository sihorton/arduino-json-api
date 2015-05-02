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
  if (mycmd == "analogRead") return this->rst(mycmd,(String)cmd["pin"].as<long>(), "");
  if (mycmd == "analogWrite") return this->rst(mycmd,(String)cmd["pin"].as<long>(), (String)cmd["val"].as<long>());
  
  return false;
}

//process rest command
int MSG_CORE::rst(String cmd,String param1,String param2) {
  if (cmd == "pinMode") {
    proto->msgOpen("pinMode","log");
    proto->msgAttr("pin",param1);
    proto->msgAttr("mode",param2);
    proto->msgSend("pinMode");  
       
    pinMode(getPin(param1), param2.toInt());
    return true;
  }  
  if (cmd == "digitalWrite") {
    proto->msgOpen("digitalWrite","log");
    proto->msgAttr("pin",param1);
    proto->msgAttr("val",param2);
    proto->msgSend("digitalWrite");
       
    digitalWrite(getPin(param1), param2.toInt());
    return true;
  }
  if (cmd == "digitalRead") {
    int inputReading = digitalRead(getPin(param1));
         
    proto->msgOpen("digitalRead","out");
    proto->msgAttr("pin", param1);
    proto->msgAttr("val",inputReading);
    proto->msgSend("digitalRead");
    return true;
  }
  if (cmd == "analogRead") { 
   int inputReading = analogRead(getPin(param1));
         
    proto->msgOpen("analogRead","out");
    proto->msgAttr("pin", param1);
    proto->msgAttr("val",inputReading);
    proto->msgSend("digitalRead");
    return true;
  }
  if (cmd == "analogWrite") { 
    //you need to check the board has PWM marker by the pin for this to work.
    int inputReading = analogRead(getPin(param1));
    proto->msgOpen("analogWrite","log");
    proto->msgAttr("pin",param1);
    proto->msgAttr("val",param2);
    proto->msgSend("digitalWrite");
       
    analogWrite(getPin(param1), param2.toInt());
    return true;
  }
  return false;
}
int MSG_CORE::getPin(String pin) {
  if(pin[0] == 'A') {
    return A0 + pin.substring(1).toInt();
  } else {
    return pin.toInt();      
  }
}
