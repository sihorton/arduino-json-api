//prevent multiple includes
#ifndef JSON_SERVO_H
#define JSON_SERVO_H
//include libraries you use.
#include <Arduino.h> 
#include <ArduinoJson.h>
#include "msg_protocol.h"
#include <Servo.h>

//class
class JSON_SERVO { 
public:
	//constructor - destructor
	JSON_SERVO(MSG_PROTOCOL &protoer);
	~JSON_SERVO();
	int cmd(ArduinoJson::JsonObject&);
        int rst(String cmd,String param1,String param2);
  MSG_PROTOCOL* proto;
private:
  Servo* servos[3];
  int getPin(String pin);
  Servo myservo;
 };
#endif
