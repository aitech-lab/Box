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

/*
unsigned char local_ip[]    = {10,0,0,15};   // IP address of WiShield
unsigned char gateway_ip[]  = {10,0,0, 1};   // router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0}; // subnet mask for the local network
char ssid[]                 = {"fuckoff"   };   // max 32 bytes
unsigned char security_type = 3;               // 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"qazwsxed"};	// max 64 characters
*/
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

bool waitServerResponse = false;
bool serverSaysOK = false;
int  waitCounter = 0;
 
char resetCounter = 0;
void (*resetFunc)(void)   = 0;

// Function that prints data from the server
void printData(char* data, int len) {
  // Print the data returned by the server
  // Note that the data is not null-terminated, may be broken up into smaller packets, and 
  // includes the HTTP header. 
  while (len-- > 0) {
    static char p = ' ';
    char c = *(data++);
    if (p=='@' && c== '#') {
        serverSaysOK = true;
    }    
    p = c;
    Serial.print(c);
  }
}

// CHANGE IT
//uint8 ip[] = {10,0,0,9};
uint8 ip[] = {192,168,4,139};

// A request that gets the latest METAR weather data for LAX
GETrequest pingServer(ip, 8515, "192.168.4.139", "/");

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
  
  led_rgbrgb(1);
  
  Serial.println("WebServer Init complete");
  
  // Enable Serial output and ask WiServer to generate log messages (optional)
  WiServer.enableVerboseMode(true);

  // Have the processData function called when data is returned by the server
  pingServer.setReturnFunc(printData);
}


// Time (in millis) when the data should be retrieved 


void loop() {
  
  static const char cos_a[] = {
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 15, 15, 
    15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 14, 14, 14, 14, 14, 14, 
    13, 13, 13, 13, 13, 13, 13, 12, 12, 12, 12, 12, 12, 11, 11, 11, 
    11, 11, 10, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 8, 8, 8, 
    8, 8, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 5, 5, 5, 5, 
    5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 2, 2, 
    2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 
    3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 
    5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 
    8, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 
    11, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 14, 14, 
    14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
  static unsigned char cos_id = 0;
  int buttonState = digitalRead(RED_BUTTON);
  // Check if it's time to get an update
  
  if (!waitServerResponse) {
    cos_id++;
    led_B(cos_a[cos_id]);
    led_R(0);
    //led_R(cos_a[(unsigned char)(cos_id+128)]);
  } else {
    if (serverSaysOK) {
        waitServerResponse = false;
        led_G(255);
        delay(333);
        led_G(  0);
    } else {
        if( waitCounter++ >=1000 && !serverSaysOK) {
            led_B(0);
            led_R(32); 
            delay(333); 
            led_R( 0); 
            resetCounter++;
            if(resetCounter >= 3){
                Serial.println("REBOOT");
                led_R(32);
                delay(1000);
                resetCounter = 0;
                resetFunc();
            }
            waitServerResponse = false;
        } else {
            if(waitCounter % 50 == 0) Serial.print('.');
            // wait purple blink
            cos_id+=8;
            led_B(cos_a[cos_id]);
            led_R(cos_a[cos_id]);
        }
    }
  }
  
  if(buttonState == LOW && !waitServerResponse) {
	Serial.println("Ping Server");
	waitServerResponse = true;
	serverSaysOK = false;
	waitCounter = 0;
	pingServer.submit();
  }
  // Run WiServer
  WiServer.server_task();
 
  delay(10);
}
