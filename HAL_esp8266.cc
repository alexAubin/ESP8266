#include <SoftwareSerial.h>
#include "HAL_esp8266.h"

SoftwareSerial dbgSerial(2,3);

// ######################################################################################

bool esp8266_reset()
{
    #ifdef DEBUG
        DEBUG_Serial.println("Resetting ESP8266");
    #endif
    bool ready = false;
    while (!ready)
    {
        ready = esp8266_sendCommand("AT+RST","ready", 2000);
    }

    return true;
}

bool esp8266_check()
{ return esp8266_sendCommand("AT",                      "OK",  500); }

bool esp8266_setMode(int mode)
{ return esp8266_sendCommand("AT+CWMODE="+String(mode), "OK", 1000); }

bool esp8266_listNetworks()
{ return esp8266_sendCommand("AT+CWLAP",                "OK", 20000); }

bool esp8266_resetNetworkCredentials()
{ return esp8266_sendCommand("AT+CWJAP=\"\",\"\"",      "OK", 1000); }

bool esp8266_setNetworkCredentials()
{ return esp8266_sendCommand(String("")+"AT+CWJAP=\""+ESP8266_SSID+"\",\""+ESP8266_PASS+"\"","OK", 8000); }

bool esp8266_checkIP()  
{ return esp8266_sendCommand("AT+CIFSR",                "APIP", 500); }

// ######################################################################################

bool esp8266_testGetRequest()
{
    bool test;

    // Set mode for multiple connections
    test = esp8266_sendCommand("AT+CIPMUX=0", "OK", 500);
    if (!test) return false;
    // Start connection
    String cmd = String("")+"AT+CIPSTART=\"TCP\",\""+ESP8266_TEST_IP+"\",80";
    test = esp8266_sendCommand(cmd, "OK", 2000);
    if (!test) return false;
    
    // Start request sending
    cmd = String()+ESP8266_TEST_GETREQUEST+"\r\n\r\n";
    test = esp8266_sendCommand("AT+CIPSEND="+String(cmd.length()), ">", 500);
    if (!test) return false;
    
    // Actually send request
    test = esp8266_sendCommand(cmd, "+IPD", 10000);
    if (!test) return false;

    return true;
}

bool esp8266_testPostRequest()
{
    bool test;

    // Set mode for multiple connections
    test = esp8266_sendCommand("AT+CIPMUX=0", "OK", 500);
    if (!test) return false;
    // Start connection
    String cmd = String("")+"AT+CIPSTART=\"TCP\",\""+ESP8266_TEST_IP+"\",80";
    test = esp8266_sendCommand(cmd, "OK", 2000);
    if (!test) return false;
    
    // Start request sending
    cmd = String()+ESP8266_TEST_POSTREQUEST+"\r\n\r\n";
    test = esp8266_sendCommand("AT+CIPSEND="+String(cmd.length()), ">", 500);
    if (!test) return false;
    
    // Actually send request
    test = esp8266_sendCommand(cmd, "+IPD", 10000);
    if (!test) return false;

    return true;
}



// ######################################################################################

bool esp8266_connect()
{

    esp8266_setNetworkCredentials();

    while (!esp8266_checkIP())
    {
        delay(1000);
    }

    return true;

}

void esp8266_init()
{
    #ifdef DEBUG
        DEBUG_Serial.begin(DEBUG_BAUDRATE);
    #endif

    // Init serial
    #ifdef DEBUG
        DEBUG_Serial.println("Opening serial for ESP8266");
    #endif
    ESP8266_Serial.begin(ESP8266_BAUDRATE);

    esp8266_reset();
    esp8266_check();

    #ifdef DEBUG
    DEBUG_Serial.println("ESP8266 is ready");
    #endif

}

// #####################################################################################
//  Send a command to the ESP8266, check (or not) that the response contains a keyword,
//  if not, halt the duino.
// #####################################################################################
bool esp8266_sendCommand(String command, String acknowledge_keyword, int timeout)
{
    // Sending command

    #ifdef DEBUG
        DEBUG_Serial.println("---- Sending :");
        DEBUG_Serial.println(command);
    #endif

    ESP8266_Serial.print(command);
    ESP8266_Serial.print("\r\n");

    // Getting response

    String response = "";
    long deadline = millis() + timeout;
    while (millis() < deadline)
        if (ESP8266_Serial.available())
        {
            response += (char) ESP8266_Serial.read();
        }

    #ifdef DEBUG
        DEBUG_Serial.println("---- Receiving :");
        DEBUG_Serial.println(response);
    #endif

    // If no ack response specified, skip all available module output.
    if (acknowledge_keyword == "")
    {
        esp8266_acknowledge(response,"\n\n\n");
        return true;
    }
    // Otherwise wait for ack.
    else
    {
        bool ack = esp8266_acknowledge(response,acknowledge_keyword);
        // If ack is okay, return true
        if (ack) return true;
        else     return false;
    }
}

// #####################################################################################

bool esp8266_acknowledge(String response, String keyword)
{
    int foundpos = -1;
    for (int i = 0; (i < response.length() - keyword.length()); i++)
    {
        if (response.substring(i,keyword.length()+i) == keyword)
        {
            foundpos = i;
        }
    }
    if (foundpos == -1) return false;
    else return true;
}

