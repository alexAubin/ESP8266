
#include <Arduino.h>

// ######################################################################################

#define DEBUG true

extern SoftwareSerial  dbgSerial;
#define DEBUG_Serial   dbgSerial
#define DEBUG_BAUDRATE 9600

// ######################################################################################

#define ESP8266_Serial   Serial
#define ESP8266_TIMEOUT  1000
#define ESP8266_BAUDRATE 115200

#define SSID        "yourSSID"
#define PASS        "yourPassword"

// ######################################################################################

void esp8266_init();
bool esp8266_reset();
bool esp8266_check();
bool esp8266_setMode(int mode);
bool esp8266_listNetworks();
bool esp8266_resetConnection();
bool esp8266_connect();
bool esp8266_checkIP();

bool esp8266_sendCommand(String command, String acknowledge_keyword = "", bool halt_on_fail = false, int timeout = ESP8266_TIMEOUT);
bool esp8266_acknowledge(String response, String keyword);
void esp8266_halt(String msg);

// ######################################################################################

