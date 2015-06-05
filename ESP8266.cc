#include "ESP8266.h"

// ######################################################################################
// #  Construct a ESP8266 module instance, with possibly a debug stream                 #
// ######################################################################################

ESP8266::ESP8266(SoftwareSerial* debugStream_)
{
    debugStream = debugStream_;      
    moduleStream = &Serial;
}

// ######################################################################################
// #  Init the module by starting the TTL serial communication, calling a reset and     #
// #  making a general check of if the module is active and ready.                      #
// ######################################################################################

bool ESP8266::init(int baudRate)
{
    if (debugStream)
        debugStream->println("Starting serial TTL for ESP8266 module");
    
    moduleStream->begin(baudRate);
 
    bool test = false;
    test = reset();
    if (!test) return false;

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
        debugStream->println("Attempting to reset ESP8266");

    short int remainingAttempts = 3;
    while (remainingAttempts-- > 0)
    {
        if (sendCommand("AT+RST","ready", 2000)) return true;
    }

    return false;
}


// #######################################################################################
// #  Send a command to the ESP8266, check (or not) that the response contains a        #
// #  keyword, if not, halt the duino.                                                  #
// #######################################################################################

bool ESP8266::sendCommand(String command, String acknowledge_keyword, int timeout)
{
    // Sending command

    if (debugStream)
    {
        debugStream->println("---- Sending :");
        debugStream->println(command);
    }

    moduleStream->print(command);
    moduleStream->print("\r\n");

    // Getting response

    String response = "";
    long deadline = millis() + timeout;
    while (millis() < deadline)
    if (moduleStream->available())
    {
        response += (char) moduleStream->read();
    }

    if (debugStream)
    {
        debugStream->println("---- Receiving :");
        debugStream->println(response);
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
        debugStream->println("Checking module status");

    bool test = sendCommand("AT", "OK",  500); 
    
    if (debugStream)
    {
        if (test) debugStream->println("Module is active and ready");
        else      debugStream->println("Module is busy / not responding");
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

bool ESP8266::getRequest(String ip, String request)
{
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
    test = sendCommand(cmd, "+IPD", 10000);
    if (!test) return false;

    return true;
}

// ######################################################################################
// #  Start a TCP connection and send a HTTP POST request                               #
// ######################################################################################

bool ESP8266::postRequest(String ip, String request)
{
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
    test = sendCommand(cmd, "+IPD", 10000);
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


