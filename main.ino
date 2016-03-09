#include <SoftwareSerial.h>
#include "ESP8266.h"

String sendData(String command, const int timeout, boolean debug);

ESP8266 wifiModule(&Serial);

void setup()
{
    Serial.begin(115200);

    delay(1000);

    wifiModule.init(9600);

    delay(1000);

    wifiModule.listNetworks();

    delay(1000);

    wifiModule.connect("yourNetwordSSID","yourNetworkPassword");

    delay(1000);

}

void loop()
{
    String output = "";

    wifiModule.getRequest("111.222.333.444", "domainName.tld",
                          "/some/uri", &output);

    Serial.println(output);

    delay(30000);
}



