#include <SoftwareSerial.h>
#include "HAL_esp8266.h"

SoftwareSerial dbgSerial(2,3);

// ######################################################################################

bool esp8266_reset()
{ 
    #ifdef DEBUG
        DEBUG_Serial.println("Resetting ESP8266");
    #endif
    return esp8266_sendCommand("AT+RST","ready",true, 2000); 
}

bool esp8266_check()  
{ return esp8266_sendCommand("AT",                      "OK",  true,  500); }

bool esp8266_setMode(int mode)    
{ return esp8266_sendCommand("AT+CWMODE="+String(mode), "OK", false, 1000); }

bool esp8266_listNetworks()     
{ return esp8266_sendCommand("AT+CWLAP",                "OK", false, 20000); }

bool esp8266_resetConnection()  
{ return esp8266_sendCommand("AT+CWJAP=\"\",\"\"",      "OK", false, 1000); }

bool esp8266_connect()          
{ return esp8266_sendCommand(String("")+"AT+CWJAP=\""+SSID+"\",\""+PASS+"\"","OK", false, 1000); }

bool esp8266_checkIP()          
{ return esp8266_sendCommand("AT+CIFSR",                "OK", false, 2000); }

// ######################################################################################

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
bool esp8266_sendCommand(String command, String acknowledge_keyword, bool halt_on_fail, int timeout)
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
        // Else return false (or completely halt arduino if halt_on_fail)
        if (halt_on_fail) esp8266_halt("Critical command failed");
        return false;
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

// #####################################################################################

void esp8266_halt(String msg)
{
    #ifdef DEBUG
        DEBUG_Serial.println(msg);
        DEBUG_Serial.println("HALT");
    #endif

    delay(5000);

    #ifdef DEBUG
        DEBUG_Serial.println("Attempting reinit");
    #endif

    esp8266_reset();
    while(true)
    {
        esp8266_sendCommand("AT",     "OK",  true,  500);
        break;
    };
}



