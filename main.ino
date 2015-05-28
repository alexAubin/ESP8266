#include <SoftwareSerial.h>
#include "HAL_esp8266.h"

String sendData(String command, const int timeout, boolean debug);

void setup()
{

    // Init serial connections, reset module and check it is ready
    esp8266_init();

    // Set mode to both Sta and AP 
    // (whatever that means, I'm not networking expert, but it works)
    esp8266_setMode(3);
    
    // For debug : print list of networks
    //esp8266_listNetworks();
    
    // Connect to the network
    esp8266_connect();
    
    // Test with a GET request
    //esp8266_testGetRequest();
    
    // Test with a POST request
    // TODO / FIXME this does not work for me
    esp8266_testPostRequest();

}
 
void loop()
{
}
 
