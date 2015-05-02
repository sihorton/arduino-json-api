#include "json-servo.h" //include the declaration for this class

//<<constructor>> 
JSON_SERVO::JSON_SERVO(MSG_PROTOCOL &protoer){
    proto = &protoer;//operate on address of print
}

//<<destructor>>
JSON_SERVO::~JSON_SERVO(){/*nothing to destruct*/}

//process the command
void JSON_SERVO::cmd(ArduinoJson::JsonObject& cmd) {
  proto->msgOpen("servo","hi");
  proto->msgSend();
  //msgOpen("servo","log");
  //msgSend("servo");
  //Serial.println("servo cmd");
	//digitalWrite(LED_PIN,HIGH); //set the pin HIGH and thus turn LED on
}
