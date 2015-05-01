# arduino-json-api
JSON serial api for controlling arduino

I needed to get a log of actions and readings the arduino took as well as being able to interactively program the arduino from nodejs. I therefore put together this code to control the arduino functions using a json protocol. Open the sketch and then upload to the arduino, open the serial port and then copy and paste in the api commands.

Initial API:

## pinMode

Set a pin on arduino board to be input (0) or output (1):
    
     {"cmd":"pinMode","pin":7,"val":1}
     

## digitalWrite

Output HIGH (1) or LOW (0) from an output pin:
    
     {"cmd":"digitalWrite","pin":7,"val":1}
     
    
# API Responses

## read a push button (interrupt style)
Currently if a pin is set to be an input then the arduino will monitor its value, whenver it changes it will then output json messages with the new value. The time given is the number of milliseconds since the board was started.

    {"id":0,"msg":"digitalRead","pin":2,"val":0,"time":13802}
    {"id":1,"msg":"digitalRead","pin":2,"val":1,"time":14093}
    {"id":2,"msg":"digitalRead","pin":2,"val":0,"time":16036}


