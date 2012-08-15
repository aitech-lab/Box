/*
 * A simple sketch that uses WiServer to get the hourly weather data from LAX and prints
 * it via the Serial API
 */

#include <WiServer.h>

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[]    = {10,0,0,24};   // IP address of WiShield
unsigned char gateway_ip[]  = {10,0,0, 1};   // router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0}; // subnet mask for the local network
char ssid[]                 = {"fuckoff"};   // max 32 bytes
unsigned char security_type = 3;               // 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"qazwsxed"};	// max 64 characters

// WEP 128-bit keys
prog_uchar wep_keys[] PROGMEM = { 
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,	// Key 0
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 1
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 2
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	// Key 3
};

// setup the wireless mode; infrastructure - connect to AP; adhoc - connect to another WiFi device
#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2
unsigned char wireless_mode = WIRELESS_MODE_INFRA;
unsigned char ssid_len;
unsigned char security_passphrase_len;
// End of wireless configuration parameters ----------------------------------------

boolean waitServerResponse = false;

// Function that prints data from the server
void printData(char* data, int len) {
  
  // Print the data returned by the server
  // Note that the data is not null-terminated, may be broken up into smaller packets, and 
  // includes the HTTP header. 
  while (len-- > 0) {
    Serial.print(*(data++));
  } 
  Serial.println("END");
  waitServerResponse = false;
}


// IP Address for www.weather.gov  
uint8 ip[] = {10,0,0,9};

// A request that gets the latest METAR weather data for LAX
GETrequest pingServer(ip, 8515, "10.0.0.9", "/");


void setup() {
  Serial.begin(57600);
  Serial.println("WebServer Init");
  // Initialize WiServer (we'll pass NULL for the page serving function since we don't need to serve web pages) 
  WiServer.init(NULL);
  
  // Enable Serial output and ask WiServer to generate log messages (optional)
  WiServer.enableVerboseMode(true);

  // Have the processData function called when data is returned by the server
  pingServer.setReturnFunc(printData);
}


// Time (in millis) when the data should be retrieved 


void loop(){
  static long updateTime = millis();
  // Check if it's time to get an update
  long time = millis();
  if (time >= updateTime && !waitServerResponse) {
    Serial.println("Ping Server");
    waitServerResponse = true;
    pingServer.submit();    
    // Get another update one hour from now
    updateTime = time+1000 * 10;
  }
  
  // Run WiServer
  WiServer.server_task();
 
  delay(10);
}
