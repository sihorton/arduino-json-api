//prevent multiple includes
#ifndef MSG_CORE_H
#define MSG_CORE_H
//include libraries you use.
#include <Arduino.h>

//#define MSG_SUPPORT_JSON 0

#ifdef MSG_SUPPORT_JSON
#include <ArduinoJson.h>
#endif

#include "msg_protocol.h"

#define MSG_CORE_MAX_DMONITORS 3

//class
class MSG_CORE { 
public:
  MSG_PROTOCOL* proto;
  
  //constructor - destructor
  MSG_CORE(MSG_PROTOCOL &protoer);
  ~MSG_CORE();
  
#ifdef MSG_SUPPORT_JSON
  int cmd(ArduinoJson::JsonObject&);
#endif
  int rst(String cmd,String param1,String param2);
  int monitorPins();
  
private:
  int getPin(String pin);
  String printPin(int pin);
  int monitorCount = 0;
  int monitorPin[MSG_CORE_MAX_DMONITORS];
  int monitorLVal[MSG_CORE_MAX_DMONITORS];
  int monitorSensitivity[MSG_CORE_MAX_DMONITORS];
};

#endif
