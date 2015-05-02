// Copyright Simon Horton 2015
// MIT License
//
// little helper library for formatting output messages to tags or json.

int logHelper_msgCount = 0;

#ifdef TAGMSG
  int tagmsg_state = 0;
#define msgOpen(name,type) \
  tagmsg_state = 1; \
  Serial.print ("<"); \
  Serial.print(name); \
  msgAttr("type",type); \
  msgAttr("id",logHelper_msgCount++); \
  msgAttr("time",millis()); 
  
#define msgAttr(key,val) \
  Serial.print(" "); \
  Serial.print(key); \
  Serial.print("='"); \
  Serial.print(val); \
  Serial.print("'"); 

#define msgText(x) \
  if (tagmsg_state == 1) Serial.print(">"); \
  tagmsg_state = 2; \
  Serial.print(x);

#define msgSend(tname) \
    if (tagmsg_state == 1) { Serial.println("/>"); } \
    if(tagmsg_state == 2){ Serial.print("</");Serial.print(tname);Serial.println('>');} 
#else
#endif

#ifdef JSONMSG
  int jsonmsg_state = 0;
#define msgOpen(name,type) \
  jsonmsg_state = 1; \
  Serial.print ("{"); \
  msgAttr("id",logHelper_msgCount++); \
  msgAttr("cmd",name); \
  msgAttr("type",type); \
  msgAttr("time",millis()); 
  
#define msgAttr(key,val) \
  Serial.print(key); \
  Serial.print("=\""); \
  Serial.print(val); \
  Serial.print("\" "); 

#define msgText(x) \
  if (jsonmsg_state == 1) Serial.println("}"); Serial.print("{name=\"text\" val=\"");Serial.print(x);Serial.println("\"}"); \
  jsonmsg_state = 2; \
  Serial.print(x);

#define msgSend(tname) \
    if (jsonmsg_state == 1) { Serial.println("}"); } \
    if(jsonmsg_state == 2){ } 

#endif


