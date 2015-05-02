#include "json-servo.h" //include the declaration for this class
/**
* Json API for access to arduino servo library
* http://www.arduino.cc/en/Reference/Servo
*
**/

//<<constructor>> 
JSON_SERVO::JSON_SERVO(MSG_PROTOCOL &protoer){
    proto = &protoer;//operate on address of print
}

//<<destructor>>
JSON_SERVO::~JSON_SERVO(){/*nothing to destruct*/}

//process the command
int JSON_SERVO::cmd(ArduinoJson::JsonObject& cmd) {
  int handled = 0;
  //have to convert to a String so cmd tests will work.
  String mycmd = cmd["cmd"].asString();
  if(mycmd == "api") {
    handled = 1;
    proto->printer->println("api match");
    proto->msgOpen("servo", cmd["cmd"].asString());
    proto->msgAttr("api","test");
    proto->msgSend();  
  }
  return handled;
}
