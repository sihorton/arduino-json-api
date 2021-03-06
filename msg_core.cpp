/**
* Json API for access to core arduino library
* http://www.arduino.cc/en/Tutorial/Foundations
*
/core/dpin/8/w             -- set pin 7 to write mode
/core/dpin/8/1        -- set pin 7 to output HIGH
/core/dpin/8/0        -- set pin 7 to output LOW
/core/apin/8/128          -- output power to pin 8 (needs to be PWM pin)
/core/apin/8/255          -- output full power to pin 8 (needs to be PWM pin)
/core/apin/8/0            -- output zero power to pin 8 (turn off)

/core/dpin/2/r             -- set pin 2 to read mode
/core/dpin/2/p             -- set pin 2 to INPUT_PULLUP mode
/core/dpin/2           -- read digital value of pin 2
/core/apin/A0           -- read analog value of pin A0

/core/mpin/4     -- start monitoring pin 4 and output when its value changes
/core/mpin/4/-  -- stop monitoring pin 4
/core/mpin/-      -- stop monitoring all pins
/core/mpin/*      -- list all monitored pins
**/

#include "msg_core.h"

//<<constructor>>
MSG_CORE::MSG_CORE(MSG_PROTOCOL &protoer){
    proto = &protoer;//operate on address of print
}

//<<destructor>>
MSG_CORE::~MSG_CORE(){/*nothing to destruct*/}

#ifdef MSG_SUPPORT_JSON
//convert json to ordered parameters call.
int MSG_CORE::cmd(ArduinoJson::JsonObject& cmd){
  String mycmd = cmd["cmd"].asString();
  
  if (mycmd == "pinMode") {
    const char* m = cmd["mode"];
    return this->rst("dpin",(String)cmd["pin"].as<long>(), m);
  } 
  if (mycmd == "digitalWrite") return this->rst("dpin",(String)cmd["pin"].as<long>(), (String)cmd["val"].as<long>());
  if (mycmd == "digitalRead") return this->rst("dpin",(String)cmd["pin"].as<long>(), "");
  if (mycmd == "analogRead") return this->rst("apin",(String)cmd["pin"].as<long>(), "");
  if (mycmd == "analogWrite") return this->rst("apin",(String)cmd["pin"].as<long>(), (String)cmd["val"].as<long>());
  if (mycmd == "monitorPin") return this->rst("mpin",(String)cmd["pin"].as<long>(), "");
  if (mycmd == "unmonitorPin") return this->rst("mpin",(String)cmd["pin"].as<long>(), "-");
  if (mycmd == "clearMonitorPins") return this->rst("mpin","-", "");
  if (mycmd == "listMonitorPins") return this->rst("mpin","-", "*");
  
  return false;
}
#endif

//process rest command
int MSG_CORE::rst(String cmd,String param1,String param2) {
  if (cmd == "pinMode") {
    proto->msgOpen("pinMode","log");
    proto->msgAttr("lib","core");
    proto->msgAttr("pin",param1);
    proto->msgAttr("mode",param2);
    proto->msgSend("pinMode");  
       
    pinMode(getPin(param1), param2.toInt());
    return true;
  } 
  if (cmd == "dpin") {
    proto->msgOpen("dpin","");
    proto->msgAttr("pin", param1);
    if(param2 == "") {
      //read the value.
      int inputReading = digitalRead(getPin(param1));   
      proto->msgAttr("val",inputReading);
    } else if(param2 == "r") {
      pinMode(getPin(param1), INPUT); 
      proto->msgAttr("mode", "r");
    } else if(param2 == "p") {
      pinMode(getPin(param1), INPUT_PULLUP); 
      proto->msgAttr("mode", "p");
    } else if(param2 == "w") {
      proto->msgAttr("mode", "w");
      pinMode(getPin(param1), OUTPUT);
    } else {
      proto->msgAttr("val", param2);
      digitalWrite(getPin(param1), param2.toInt()); 
    } 
    proto->msgSend("dpin");
    return true;
  }
  if (cmd == "apin") {
    proto->msgOpen("apin","");
    proto->msgAttr("pin", param1);
    if(param2 == "") {
      int inputReading = analogRead(getPin(param1));
      proto->msgAttr("val",inputReading);
    } else {
      proto->msgAttr("val",param2);     
      analogWrite(getPin(param1), param2.toInt());
    }
    proto->msgSend("apin");
    return true;
  }
  //core/mpin/4
  //core/mpin/4/-
  //core/mpin/-
  //core/mpin/*
  if (cmd == "mpin") {
    if(param1 == "*") {
      //list all monitors
      int i;
      for(i=0;i<monitorCount;i++) {
        proto->msgOpen("dpin","monitor*");
        proto->msgAttr("pin",printPin(monitorPin[i]));
        proto->msgAttr("val",monitorLVal[i]);
        proto->msgSend();
      }
    } else if(param1 == "-") {
      //remove all monitors
      int i;
      for(i=0;i<monitorCount;i++) {
          proto->msgOpen("unmonitorPin","log");
          proto->msgAttr("pin",printPin(monitorPin[i]));
          proto->msgAttr("monitors", monitorCount-i-1);
          proto->msgAttr("maxMonitors", MSG_CORE_MAX_DMONITORS+1);
          proto->msgSend("unmonitorPin");
          monitorPin[i] = 0;
      }
      monitorCount = 0;
    } else if(param2=="-") {
      //remove a monitor
      proto->printer->println("remove monitor " + param1);
      int i;
      int p = getPin(param1);
      for(i=0;i<monitorCount;i++) {
        if(monitorPin[i] == p){
          monitorPin[i] = monitorPin[monitorCount];
          monitorPin[monitorCount] = 0;
          monitorCount--;
          proto->msgOpen("unmonitorPin","log");
          proto->msgAttr("pin",param1);
          proto->msgAttr("monitors", monitorCount);
          proto->msgAttr("maxMonitors", MSG_CORE_MAX_DMONITORS+1);
          proto->msgSend("unmonitorPin");
          break;
        }
      } 
    } else {
      //add a monitor
      //you need to check the board has PWM marker by the pin for this to work.
      //loop through all pins, if already monitoring then do nothing
      //if not found then add it at any with value 0.
      int i;
      for(i=0;i<monitorCount;i++) {
        if(monitorPin[i] == getPin(param1)){
          //already monitoring..
          if(param2 != "") monitorSensitivity[i] = param2.toInt();
          return true;
        } 
      }  
      if (i<MSG_CORE_MAX_DMONITORS+1) {
        //space to add this.
        monitorPin[i] = getPin(param1);
        monitorCount = i+1;
        if(param2=="")param2="0";
        monitorSensitivity[i] = param2.toInt();
        
        proto->msgOpen("mpin","log");
        proto->msgAttr("pin",param1);
        proto->msgAttr("sensitivity",param2);
        proto->msgAttr("monitors", monitorCount);
        proto->msgAttr("maxMonitors", MSG_CORE_MAX_DMONITORS+1);
        proto->msgSend("mpin");
      
      } else {
        //no space..
        proto->msgOpen("Error","Error");
        proto->msgAttr("lib","core");
         proto->msgAttr("ErrId",4);
         proto->msgAttr("ErrName","max_pin_monitors");           
         proto->msgAttr("MaxMonitors",MSG_CORE_MAX_DMONITORS);
         proto->msgSend("Error");
      }  
    } 
    return true;
  }
  
  return false;
}
int MSG_CORE::getPin(String pin) {
  if(pin[0] == 'A') {
    return A0 + pin.substring(1).toInt();
  } else {
    return pin.toInt();      
  }
}
String MSG_CORE::printPin(int pin) {
  if(pin > A0) {
    return "A" + String(pin - A0);
  } else {
    return String(pin);      
  }
}
int MSG_CORE::monitorPins() {
  for(int i=0; i<monitorCount; i++) {
    if (monitorPin[i] > 0) {
      boolean changed = false;
      int inputReading;
      if (monitorPin[i] > A0) {
        inputReading = analogRead(monitorPin[i]);
        
        //smooth the values so that 0 and 1023 always show up
        if(inputReading < monitorSensitivity[i]) inputReading = 0;
        if(inputReading > 1023-monitorSensitivity[i]) inputReading = 1023;
        
        if (inputReading > monitorLVal[i] + monitorSensitivity[i] || inputReading < monitorLVal[i] - monitorSensitivity[i]) changed = true;
      } else {
        inputReading = digitalRead(monitorPin[i]);
         if(inputReading != monitorLVal[i]) changed = true;
       }
       if(changed) {
         monitorLVal[i] = inputReading; 
         proto->msgOpen("dpin","monitor");
         proto->msgAttr("pin", printPin(monitorPin[i]));
         proto->msgAttr("val",inputReading);
         proto->msgAttr("sensitivity",monitorSensitivity[i]);
         proto->msgSend("dpin");
         monitorLVal[i] = inputReading; 
       }
     }
   }
  
  return true;
}
