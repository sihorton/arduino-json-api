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

int JSON_SERVO::rst(String cmd,String param1,String param2) {
  if (cmd == "attach") {
    proto->msgOpen("attach","log");
    proto->msgAttr("lib","servo");
    proto->msgAttr("servo",param1);
    proto->msgAttr("pin",param2);
    proto->msgSend("attach");  
    
    //servos[getPin(param1)]->attach(getPin(param2));
    myservo.attach(getPin(param2));
    return true;
  }  
  if (cmd == "write") {
    proto->msgOpen("write","log");
    proto->msgAttr("lib","servo");
    proto->msgAttr("servo",param1);
    proto->msgAttr("val",param2);
    proto->msgSend("write");  
    
    //servos[getPin(param1)]->write(param2.toInt());
    myservo.write(param2.toInt());
    return true;
  }
  if (cmd == "read") {
    //int inputReading = servos[getPin(param1)]->read();
    int inputReading = myservo.read();
     
    proto->msgOpen("read","log");
    proto->msgAttr("lib","servo");
    proto->msgAttr("servo",param1);
    proto->msgAttr("val",inputReading);
    proto->msgSend("read");  
    
    return true;
  }
  if (cmd == "detach") {
    proto->msgOpen("detach","log");
    proto->msgAttr("lib","servo");
    proto->msgAttr("servo",param1);
    proto->msgSend("detach");  
    
    //servos[getPin(param1)]->detach();
    myservo.detach();
    return true;
  } 
  return false;
}

int JSON_SERVO::getPin(String pin) {
  if(pin[0] == 'A') {
    return A0 + pin.substring(1).toInt();
  } else {
    return pin.toInt();      
  }
}
