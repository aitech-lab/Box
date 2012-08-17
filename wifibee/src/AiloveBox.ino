/*
 * A simple sketch that uses WiServer to get the hourly weather data from LAX and prints
 * it via the Serial API
 */

#include <WiServer.h>

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[]    = {192,168,4,162};   // IP address of WiShield
unsigned char gateway_ip[]  = {192,168,4,  1};   // router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0}; // subnet mask for the local network
char ssid[]                 = {"ailove_g"   };   // max 32 bytes
unsigned char security_type = 3;               // 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"shaiShei5do8ev9u"};	// max 64 characters

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
  led_B(16); led_G(0);
  waitServerResponse = false;
}


// IP Address for www.weather.gov  
uint8 ip[] = {193,232,102,70};

// A request that gets the latest METAR weather data for LAX
GETrequest pingServer(ip, 8515, "193.232.102.70", "/");

void led_rgbrgb(int rep=1){
	for(int j=0; j<rep; j++) {
		for(int i=5; i<=7; i++) {
			//Serial.print("Pin "+String(i)+" on ");
			pinMode(i, OUTPUT);      
			digitalWrite(i, HIGH);  
			delay(200);
			digitalWrite(i, LOW);  
		} 
	}
}

#define R 5
#define G 7
#define B 6
#define RED_BUTTON 15

inline void led_R(int val) {
	analogWrite(R, val); 
}

inline void led_G(int val) {
	analogWrite(G, val); 
}

inline void led_B(int val) {
	analogWrite(B, val); 
}

void setup() {
  Serial.begin(57600);
  Serial.println("WebServer Init");
  led_rgbrgb(3);
  led_R(16);
  // Initialize WiServer (we'll pass NULL for the page serving function since we don't need to serve web pages) 
  WiServer.init(NULL);
  
  led_rgbrgb(3);
  led_B(16);
  Serial.println("WebServer Init complete");
  
  // Enable Serial output and ask WiServer to generate log messages (optional)
  WiServer.enableVerboseMode(true);

  // Have the processData function called when data is returned by the server
  pingServer.setReturnFunc(printData);
}


// Time (in millis) when the data should be retrieved 


void loop() {
  int buttonState = digitalRead(RED_BUTTON);
  // Check if it's time to get an update
  if(buttonState == LOW && !waitServerResponse) {
	Serial.println("Ping Server");
	waitServerResponse = true;
	pingServer.submit();    
	led_G(255); led_B(0);
  }
  // Run WiServer
  WiServer.server_task();
 
  delay(10);
}
