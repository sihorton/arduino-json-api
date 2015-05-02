//prevent multiple includes
#ifndef JSON_SERVO_H
#define JSON_SERVO_H
//include libraries you use.
#include <Servo.h> 

//class
class JSON_SERVO { 
public:
	//constructor - destructor
	JSON_SERVO();
	~JSON_SERVO();
	void cmd();
};

#endif