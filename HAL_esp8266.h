
#include <Arduino.h>

// ######################################################################################

#define DEBUG true

// Debug goes on a software serial
extern SoftwareSerial  dbgSerial;
#define DEBUG_Serial   dbgSerial
#define DEBUG_BAUDRATE 9600

// ######################################################################################

// ESP8266 is configured on standard Serial
#define ESP8266_Serial   Serial
#define ESP8266_BAUDRATE 115200

#define ESP8266_SSID     "yourNetworkName"
#define ESP8266_PASS     "yourPassword"

#define ESP8266_TEST_IP          "1.2.3.4"
#define ESP8266_TEST_GETREQUEST  "GET / HTTP/1.1\r\nHost: yourHostName.com"
#define ESP8266_TEST_POSTREQUEST "POST /page.php HTTP/1.1\r\nHost: yourHostName.info\r\nfield=whatever"


// ######################################################################################

bool esp8266_setNetworkCredentials();
void esp8266_init();
bool esp8266_reset();
bool esp8266_check();
bool esp8266_setMode(int mode);
bool esp8266_listNetworks();
bool esp8266_resetNetworkCredentials();
bool esp8266_connect();
bool esp8266_checkIP();
bool esp8266_testGetRequest();
bool esp8266_testPostRequest();

bool esp8266_sendCommand(String command, String acknowledge_keyword = "", int timeout = 1000);
bool esp8266_acknowledge(String response, String keyword);

// ######################################################################################

