#include <SoftwareSerial.h>
#include <Arduino.h>

class ESP8266
{
    public :

        ESP8266(HardwareSerial* debugStream_ = 0);

        ~ESP8266() { };

        bool init(long baudRate);
        bool connect(String SSID, String password);
        bool listNetworks();
        bool getRequest(String ip,
                        String host,
                        String page,
                        String* output = 0,
                        String arguments = "");


        bool sendCommand(String  command,
                         String  acknowledge_keyword,
                         int     timeout,
                         String* output = 0,
                         void    (*responseParser)(int, SoftwareSerial*, String*) = 0);
    private :

        bool reset();
        bool checkActiveAndReady();
        bool setWifiMode(int mode);
        bool resetNetworkCredentials();
        bool setNetworkCredentials(String SSID, String password);
        bool checkIP();
        bool acknowledge(String response, String keyword);

        HardwareSerial* debugStream;
        SoftwareSerial* moduleStream;

};

