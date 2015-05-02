//prevent multiple includes
#ifndef MSG_CORE_H
#define MSG_CORE_H
//include libraries you use.
#include <Arduino.h> 
#include <ArduinoJson.h>
#include "msg_protocol.h"

#define MSG_CORE_MAX_DMONITORS 3

//class
class MSG_CORE { 
public:
  MSG_PROTOCOL* proto;
  
  //constructor - destructor
  MSG_CORE(MSG_PROTOCOL &protoer);
  ~MSG_CORE();
  
  int cmd(ArduinoJson::JsonObject&);
  int rst(String cmd,String param1,String param2);
  int monitorPins();


private:
  int getPin(String pin);
  int monitorCount = 0;
  int monitorPin[MSG_CORE_MAX_DMONITORS];
  int monitorPinVal[MSG_CORE_MAX_DMONITORS];
};

#endif
