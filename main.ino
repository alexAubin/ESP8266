#include <SoftwareSerial.h>
#include "ESP8266.h"

String sendData(String command, const int timeout, boolean debug);

SoftwareSerial debugSerial(2,3);
ESP8266        wifiModule(&debugSerial);

void setup()
{
    Serial.begin(9600);
 
    wifiModule.init((int) 115200);

    wifiModule.connect("whatever","password");
    
    wifiModule.getRequest("1.2.3.4", "GET / HTTP/1.1\r\nHost: yourHostName.com");

    wifiModule.postRequest("1.2.3.4", "POST /page.php HTTP/1.1\r\nHost: yourHostName.info\r\nfield=whatever");
}
 
void loop()
{
}
 
