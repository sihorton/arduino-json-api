//prevent multiple includes
#ifndef MSG_PROTOCOL_H
#define MSG_PROTOCOL_H
//include libraries you use.
#include <ArduinoJson.h>

//class
class MSG_PROTOCOL { 
public:
	//constructor - destructor
	MSG_PROTOCOL(Print &print);
	~MSG_PROTOCOL();
	void cmd();
  void msgOpen(String name,String type);
  void msgSend(String name);
  void msgSend();
  void msgAttr(String key,String value);
  void msgAttr(String key,int value);
  void msgText(String text);
  Print* printer;
  long msgCount;
private:  
  int comma;
};

#endif
