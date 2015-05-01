//code for inline debug statments...
//just do DEBUG_PRINT(xxx) instead of Serial.print
#define DEBUG
#ifdef DEBUG
 #define DEBUG_PRINT(x)  Serial.print (x)
#else
 #define DEBUG_PRINT(x)
#endif