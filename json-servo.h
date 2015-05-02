//prevent multiple includes
#ifndef JSON_SERVO_H
#define JSON_SERVO_H
//include libraries you use.
#include <ArduinoJson.h>
#include "msg_protocol.h"

//class
class JSON_SERVO { 
public:
	//constructor - destructor
	JSON_SERVO(MSG_PROTOCOL &protoer);
	~JSON_SERVO();
	int cmd(ArduinoJson::JsonObject&);
        int rst(char*);
  MSG_PROTOCOL* proto;
};

#endif
