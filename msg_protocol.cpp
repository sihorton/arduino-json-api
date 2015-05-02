#include "msg_protocol.h" //include the declaration for this class
//#include <Time.h>
#include <Arduino.h>

//<<constructor>> setup the LED, make pin 13 an OUTPUT
MSG_PROTOCOL::MSG_PROTOCOL(Print &print){
    //pinMode(LED_PIN, OUTPUT); //make that pin an OUTPUT
  printer = &print;//operate on address of print
}

//<<destructor>>
MSG_PROTOCOL::~MSG_PROTOCOL(){/*nothing to destruct*/}

void MSG_PROTOCOL::msgOpen(String name,String type) {
  printer->print("{");
  this->comma = 0;
  this->msgAttr("id",this->msgCount++); 
  this->msgAttr("cmd",name); 
  this->msgAttr("type",type); 
  this->msgAttr("time",millis()); 
}
void MSG_PROTOCOL::msgSend(String name){
  printer->println("}");
}
void MSG_PROTOCOL::msgSend(){
  printer->println("}");
}
void MSG_PROTOCOL::msgAttr(String key,String value) {
  if (this->comma==1) {
    printer->print(",");
  }else{ 
    this->comma=1;
  }

  printer->print(key);
  printer->print(":\"");
  printer->print(value);
  printer->print("\"");
}
void MSG_PROTOCOL::msgAttr(String key,int value){
  if (this->comma==1) {
    printer->print(",");
  }else{ 
    this->comma=1;
  }
  printer->print(key);
  printer->print(":");
  printer->print(value);
  printer->print("");
}

void MSG_PROTOCOL::msgText(String text){
}
