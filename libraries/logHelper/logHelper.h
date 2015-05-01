//code for inline debug statments...
//just do DEBUG_PRINT(xxx) instead of Serial.print

#ifdef DEBUG
 #define DEBUG_PRINT(x)  Serial.print (x)
#else
 #define DEBUG_PRINT(x)
#endif

#ifdef TAGLOG
int tagLog_headSent = 0;
 #define tagLog(x) \
   if(!tagLog_headSent) { \
     tagLog_headSent = 1; \
     Serial.print("*");\
     Serial.print(millis()); \
     Serial.print(": "); \
   } \
   Serial.print (x);
 #define tagLogln(x) \
   if(!tagLog_headSent) { \
     Serial.print("*");\
     Serial.print(millis()); \
     Serial.print(": "); \
   } \
   Serial.print (x); \
   Serial.print ("\n"); \
   tagLog_headSent = 0;
#else
 #define tagLog(x)
 #define tagLogln(x)
#endif

#ifdef JSONLOG
  long jsonLog_msgCount; 
  #define jsonNewMsg(name) \
  Serial.print("{\"id\":"); \
  Serial.print(jsonLog_msgCount++); \
  Serial.print(","); \
  Serial.print("\"msg\":\""); \
  Serial.print(name); \
  Serial.print("\",");  
 #define jsonMsg(key,val) \
   Serial.print("\""); \
   Serial.print(key); \
   Serial.print("\":"); \
   Serial.print(val); \
   Serial.print(","); 
 #define jsonMsgln(key,val) \
   Serial.print("\""); \
   Serial.print(key); \
   Serial.print("\":"); \
   Serial.print(val); \
   Serial.println("}"); 
#else
 #define jsonNewMsg(name)
 #define jsonMsg(x)
 #define jsonMsgln(x) 
#endif
