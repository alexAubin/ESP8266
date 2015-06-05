#include <SoftwareSerial.h>
#include <Arduino.h>

class ESP8266
{
    public :

        ESP8266(SoftwareSerial* debugStream_ = 0);

        ~ESP8266() { };

        bool init(int baudRate);
        bool connect(String SSID, String password);
        bool listNetworks();
        bool getRequest(String ip, String request);
        bool postRequest(String ip, String request);

    private :

        bool sendCommand(String command, String acknowledge_keyword, int timeout);

        bool reset();
        bool checkActiveAndReady();
        bool setWifiMode(int mode);
        bool resetNetworkCredentials();
        bool setNetworkCredentials(String SSID, String password);
        bool checkIP();
        bool acknowledge(String response, String keyword);

        SoftwareSerial* debugStream;
        HardwareSerial* moduleStream;

};

