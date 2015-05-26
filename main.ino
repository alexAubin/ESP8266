#include <SoftwareSerial.h>
#include "HAL_esp8266.h"

String sendData(String command, const int timeout, boolean debug);

void setup()
{
    esp8266_init();
    esp8266_setMode(3);
    //esp8266_listNetworks();
    esp8266_connect();
    esp8266_checkIP();
}
 
void loop()
{
  /*
  if(Serial.available()) // check if the esp is sending a message 
  {
    
    //while(esp8266.available())
    //{
      // The esp has data so display its output to the serial window 
    //  char c = esp8266.read(); // read the next character.
    //  Serial.write(c);
    //}
    
    if (Serial.find("+IPD,"))
    {
     delay(1000);
 
     int connectionId = Serial.read()-48; // subtract 48 because the read() function returns 
                                           // the ASCII decimal value and 0 (the first decimal number) starts at 48
     
     String webpage = "<h1>Hello</h1>&lth2>World!</h2><button>LED1</button>";
 
     String cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
     
     webpage="<button>LED2</button>";
     
     cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
 
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     
     sendData(closeCommand,3000,DEBUG);
    }
  }
  */
}
 
