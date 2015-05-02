//prevent multiple includes
#ifndef MSG_CORE_H
#define MSG_CORE_H
//include libraries you use.
#include <Arduino.h> 
#include <ArduinoJson.h>
#include "msg_protocol.h"

//class
class MSG_CORE { 
public:
  //constructor - destructor
  MSG_CORE(MSG_PROTOCOL &protoer);
  ~MSG_CORE();
  int cmd(ArduinoJson::JsonObject&);
  int rst(String cmd,String param1,String param2);
  MSG_PROTOCOL* proto;
private:
  int getPin(String pin);

};

#endif
