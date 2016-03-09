#include "ESP8266.h"

// ######################################################################################
// #  Construct a ESP8266 module instance, with possibly a debug stream                 #
// ######################################################################################


SoftwareSerial ESPSerial(2,3);
ESP8266::ESP8266(HardwareSerial* debugStream_)
{
    debugStream = debugStream_;
    moduleStream = &ESPSerial;
}

// ######################################################################################
// #  Init the module by starting the TTL serial communication, calling a reset and     #
// #  making a general check of if the module is active and ready.                      #
// ######################################################################################

bool ESP8266::init(long baudRate)
{
    if (debugStream)
        debugStream->println(F("Starting serial TTL for ESP8266 module"));

    moduleStream->begin(baudRate);

    delay(1000);

    bool test = false;
    test = reset();
    if (!test) return false;

    delay(1000);

    test = checkActiveAndReady();
    if (!test) return false;

    return true;
}

// ######################################################################################
// #  Reset the module by sending AT+RST command during three attempts, and checking    #
// #  for the "ready" keyword in module's response                                      #
// ######################################################################################

bool ESP8266::reset()
{
    if (debugStream)
        debugStream->println(F("Attempting to reset ESP8266"));

    short int remainingAttempts = 3;
    while (remainingAttempts-- > 0)
    {
        if (sendCommand("AT+RST","ready", 4000)) return true;
    }

    return false;
}


// #######################################################################################
// #  Send a command to the ESP8266, check (or not) that the response contains a        #
// #  keyword, if not, halt the duino.                                                  #
// #######################################################################################

bool ESP8266::sendCommand(String  command,
                          String  acknowledge_keyword,
                          int     timeout,
                          String* output,
                          void (*responseParser)(int,SoftwareSerial*, String*))
{
    // Sending command

    if (debugStream)
    {
        debugStream->println(F("---- Sending :"));
        debugStream->println(command);
    }

    moduleStream->print(command);
    moduleStream->print("\r\n");

    // Getting response

    String response = "";

    if (responseParser == 0)
    {
        long deadline = millis() + timeout;

        while (millis() < deadline)
        if (moduleStream->available())
        {
            response += (char) moduleStream->read();
        }
    }
    else
    {
        responseParser(timeout,moduleStream,&response);
    }


    if (debugStream)
    {
        debugStream->println(F("---- Receiving :"));
        debugStream->println(response);
    }

    if (output != 0)
    {
        *output = response;
    }

    // If no ack response specified, skip all available module output.
    if (acknowledge_keyword == "")
    {
        acknowledge(response,"\n\n\n");
        return true;
    }
    // Otherwise wait for ack.
    else
    {
        bool ack = acknowledge(response,acknowledge_keyword);
        // If ack is okay, return true
        if (ack) return true;
        else     return false;
    }
}

// ######################################################################################
// #  Check that a keyword is present in the given response                             #
// ######################################################################################

bool ESP8266::acknowledge(String response, String keyword)
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

// ######################################################################################
// #  Check the module is active and ready via the "AT" command                         #
// ######################################################################################

bool ESP8266::checkActiveAndReady()
{
    if (debugStream)
        debugStream->println(F("Checking module status"));

    bool test = sendCommand("AT", "OK",  500);

    if (debugStream)
    {
        if (test) debugStream->println(F("Module is active and ready"));
        else      debugStream->println(F("Module is busy / not responding"));
    }

    return test;
}

// ######################################################################################
// #  Connect to the network given the SSID and password                                #
// ######################################################################################

bool ESP8266::connect(String SSID, String password)
{
    setWifiMode(3);

    setNetworkCredentials(SSID,password);

    short int remainingAttempts = 10;
    while (remainingAttempts-- > 0)
    {
        if (checkIP()) return true;
    }

    return false;
}

bool ESP8266::setWifiMode(int mode)
{ return sendCommand("AT+CWMODE="+String(mode), "OK", 500); }

bool ESP8266::setNetworkCredentials(String SSID, String password)
{
    return sendCommand(String("")+"AT+CWJAP=\""+SSID+"\",\""+password+"\"","OK", 5000);
}

bool ESP8266::checkIP()
{
    return sendCommand("AT+CIFSR", "APIP", 500);
}

// ######################################################################################
// #  Start a TCP connection and send a HTTP GET request                                #
// ######################################################################################

// This filter meant to be used during sendCommand filters out the header
// received after the get request
void getResponseFilter(int timeout, SoftwareSerial* moduleStream, String* response)
{
    int  buffIgnore = 300;
    char previousChar = '0';
    bool nowWeSave = false;

    long deadline = millis() + timeout;
    while (millis() < deadline)
    if (moduleStream->available())
    {
        if (nowWeSave)
        {
            (*response) += (char) moduleStream->read();
        }
        else if (buffIgnore > 0)
        {
            buffIgnore--;
            previousChar = moduleStream->read();
        }
        else
        {
            char c = moduleStream->read();
            if ((previousChar == '\n') && (c == '\r'))
            {
                nowWeSave = true;
            }
            previousChar = c;
        }
    }

}

bool ESP8266::getRequest(String ip,
                         String host,
                         String page,
                         String* output,
                         String arguments)
{

    String request = "GET "+page+" HTTP/1.1";
    if (arguments != "") request += "?"+arguments;
    request += "\r\nHost: "+host+"\r\nConnection: close\r\n\r\n";

    bool test;

    // Set mode for multiple connections
    test = sendCommand("AT+CIPMUX=0", "OK", 500);
    if (!test) return false;

    // Start connection
    String cmd = String("")+"AT+CIPSTART=\"TCP\",\""+ip+"\",80";
    test = sendCommand(cmd, "OK", 2000);
    if (!test) return false;

    // Start request sending
    cmd = request+"\r\n\r\n";
    test = sendCommand("AT+CIPSEND="+String(cmd.length()), ">", 500);
    if (!test) return false;

    // Actually send request
    test = sendCommand(cmd, "CLOSED", 10000,output,&getResponseFilter);
    if (!test) return false;

    return true;
}


// ######################################################################################

bool ESP8266::listNetworks()
{
    return sendCommand("AT+CWLAP",                "OK", 20000);
}

bool ESP8266::resetNetworkCredentials()
{
    return sendCommand("AT+CWJAP=\"\",\"\"",      "OK", 1000);
}


