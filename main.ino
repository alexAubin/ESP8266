#include <SoftwareSerial.h>
#include "ESP8266.h"

String sendData(String command, const int timeout, boolean debug);

ESP8266 wifiModule(&Serial);

void setup()
{
    Serial.begin(9600);

    wifiModule.init(9600);

    wifiModule.listNetworks();

    wifiModule.connect("SSID","password");

    wifiModule.getRequest("1.2.3.4", "page.php", "argument=value&argument2=value2");

}
 
void loop()
{
}



