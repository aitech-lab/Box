#include "Arduino.h"
#include "Wire.h"

#define SEND_BUFFER_SIZE 128

#define CRX14_ADDR    0x50

#define CMD_INIT     120 // cmd: 0x00 0x01
#define CMD_INITATE  121 // cmd: 0x01 0x02 0x06 0x00
#define CMD_SLOT     122 // cmd: 0x03
#define CMD_SELECT   123 // cmd: 0x01 0x02 0x0E
#define CMD_GET_UID  124 // cmd: 0x01 0x01 0x0B
#define CMD_COMP     125 // cmd: 0x01 0x01 0x0F
#define CMD_READ     126 // cmd: 0x01
#define CMD_CLOSE    127 // cmd: 0x00 0x00
#define CMD_UNKNOWN  128

#define CRX14_PARAMETER_REGISTER      0x00
#define CRX14_IO_FRAME_REGISTER       0x01
#define CRX14_AUTHENTICATE_REGISTER   0x02
#define CRX14_SLOT_MARKER_REGISTER    0x03

struct _tag_rfid {
	unsigned char CHIP_ID;
	unsigned char UID[8];
};

void setup() {

    delay (1000);
    Serial.begin(57600);  
   
    Wire.begin();
    	
	_tag_rfid tag_rfid[16];
	check_rfid_devices(tag_rfid);
    
}

int i2cerror;

inline void print_data(unsigned char *data, unsigned char nb_byte) {
    for(unsigned char i=0; i<nb_byte; i++) { Serial.print(data[i], HEX); Serial.print(" "); };
}

inline int writecheck(unsigned char addr_i2c, unsigned char *data, unsigned char nb_byte) {
    Serial.print("[W ");
    print_data(data, nb_byte);
	Serial.print(":");
    Wire.beginTransmission(addr_i2c);
    Wire.write(data, nb_byte);
	int r = Wire.endTransmission();
    
	Serial.print(r);
	Serial.println("]");
	
	return !r;
}

inline int readcheck(unsigned char addr_i2c, unsigned char *data, unsigned char nb_byte) {
    Serial.print("[R ");
    Wire.requestFrom(addr_i2c, nb_byte);    // request 6 bytes from slave device #2
    unsigned char i = 0;
    while(Wire.available()) { 
        unsigned char c = Wire.read();    // receive a byte as character
        data[i++] = c;
		Serial.print(c, HEX);
		Serial.print(" ");
    }
	Serial.println("]");
}

/****************************************************************************/
/*  Turn ON electromagnetic field and initialize CRX14                      */
/*  Function : init_rfid                                                    */
/*      Parameters                                                          */
/*          Input   :   Nothing                                             */
/*          Output  :   Return the completion of turning ON                 */
/****************************************************************************/
inline unsigned char init_rfid(void) {
	
	//Turn OFF RF
    close_rfid();
	delay(1);
	
	//Turn ON RF
    Serial.println("init_rfid");
	unsigned char cmd[] = {CRX14_PARAMETER_REGISTER, 0x10};
 	if(!writecheck(CRX14_ADDR, cmd, 2)) return false;
    //Read answer tab
    if(!readcheck(CRX14_ADDR, cmd, 1)) return false;
    //Check if module is ON
	return cmd[0] == 0x10;
}



/****************************************************************************/
/*  Turn OFF electromagnetic field                                          */
/*  Function : close_rfid                                                   */
/*      Parameters                                                          */
/*          Input   :   Nothing                                             */
/*          Output  :   Return the completion of turning OFF                */
/****************************************************************************/
inline unsigned char close_rfid(void) {

 	//Unselect tags
    completion_rfid();
	delay(1);
	
	//Turn OFF RF
 	Serial.println("close_rfid");
	unsigned char dummy_tab[]={CRX14_PARAMETER_REGISTER, 0x00};
    if(!writecheck(CRX14_ADDR, dummy_tab,2)) return false;
    //Read answer tab
    if(!readcheck(CRX14_ADDR, dummy_tab,1)) return false;
    //Check if module is OFF
	return dummy_tab[0] == 0x00;
}

/****************************************************************************/
/*  RFID command to deactived TAGs                                          */
/*  Function : completion_rfid                                              */
/*      Parameters                                                          */
/*          Input   :   Nothing                                             */
/*          Output  :   Nothing                                             */
/****************************************************************************/
inline unsigned char completion_rfid(void) {
    
    Serial.println("completion_rfid");
    unsigned char dummy_tab[] = {CRX14_IO_FRAME_REGISTER, 0x01, 0x0F};
    if(!writecheck(CRX14_ADDR,dummy_tab, 3)) return false;
    return true;
}

/****************************************************************************/
/*  RFID command to initiate all TAGs present in the electromagnetic field  */
/*  Function : initiate_rfid                                                */
/*      Parameters                                                          */
/*          Input   :   Nothing                                             */
/*          Output  :   Nothing                                             */
/****************************************************************************/
unsigned char initiate_rfid(void) {
	
	Serial.println("initiate_rfid");
	unsigned char dummy_tab[4] = { CRX14_IO_FRAME_REGISTER, 0x02, 0x06, 0x00 };
	if(!writecheck(CRX14_ADDR,dummy_tab,4)) return false;
	return true;
}

/****************************************************************************/
/*  RFID command to read frame buffer of CRX14                              */
/*  Function : read_frame_rfid                                              */
/*      Parameters                                                          */
/*          Input   :   pointer to the data to returned                     */
/*          Input   :   number of bytes to read                             */
/*          Output  :   Nothing                                             */
/****************************************************************************/
unsigned char read_frame_rfid(unsigned char *data, unsigned char nb_bytes) {
	unsigned char dummy_tab[1];
	unsigned char dummy_cmpt;

	//Blank answer tab
	for(dummy_cmpt=0; dummy_cmpt<nb_bytes; dummy_cmpt++)
		*(data+dummy_cmpt)=0x00;
		
	Serial.println("read_frame_rfid");
	//Select frame register
	dummy_tab[0]=CRX14_IO_FRAME_REGISTER;
	if(!writecheck(CRX14_ADDR, dummy_tab, 1)) return false;
	//Read answer tab
	if(!readcheck(CRX14_ADDR, data, nb_bytes)) return true;
	return true;
}

/****************************************************************************/
/*  Check if RFID tags are present                                          */
/*  Function : check_rfid_devices                                           */
/*      Parameters                                                          */
/*          Input   :   Pointer to the structure containing for each tag    */
/*                     the CHIP_ID (1 byte, random) and UID (8 bytes, fixed)*/
/*          Output  :   Return the number of tags detected (maximum 16)     */
/****************************************************************************/
unsigned char check_rfid_devices(struct _tag_rfid *p_tag_rfid) {
  
	unsigned char dummy_tab[20];
	unsigned short dummy_short;
	unsigned char dummy_cmpt;
	unsigned char cmpt_tags=0;

	//Turn ON RFID
	init_rfid();
	//Delay for RFID, no answer from device if ommited, due to delay after I2C stop I think
	delay(100);

	//Run Initiate Command to tags
	initiate_rfid();
	//Delay for RFID, no answer from device if ommited, due to delay after I2C stop I think
	delay(100);
	
	//Read reception buffer
	read_frame_rfid(dummy_tab, 2);
	delay(100);
	
	//return if no tag detected
	if(dummy_tab[0]==0x00) {
		Serial.println("NO TAGS DETECTED");
		
		//Turn OFF RFID
		close_rfid();
		return false;
	}
	
	/*
	//Turn on detected sequence of tags
	slot_marker_rfid();
	//Delay for RFID, no answer from device if ommited, due to delay after I2C stop I think
	DelayMs(1);
	//Read reception buffer
	read_frame_rfid(dummy_tab, 19);

	//Check detected devices => CHIP_ID
	dummy_short = (dummy_tab[2]<<8) + dummy_tab[1];
	for(dummy_cmpt=0; dummy_cmpt<16; dummy_cmpt++)
	{
	if(dummy_short & 0x0001)
	{
	    (p_tag_rfid+cmpt_tags)->CHIP_ID=dummy_tab[dummy_cmpt+3];
	    cmpt_tags++;
	}
	dummy_short>>=1;
	}

	//Get UID of detected tags
	for(dummy_cmpt=0; dummy_cmpt<cmpt_tags; dummy_cmpt++)
	{
	unsigned char dummy_cmpt_2;
	//Select Tag
	select_tag_rfid((p_tag_rfid+dummy_cmpt)->CHIP_ID);
	//Delay for RFID, no answer from device if ommited, due to delay after I2C stop I think
	DelayMs(1);
	//Read reception buffer
	read_frame_rfid(dummy_tab, 2);

	//Get UID of selected tag
	get_uid_rfid();
	//Delay for RFID, no answer from device if ommited, due to delay after I2C stop I think
	DelayMs(1);
	//Read reception buffer
	read_frame_rfid(dummy_tab, 9);

	//Copy UID to struct
	for(dummy_cmpt_2=0; dummy_cmpt_2<8; dummy_cmpt_2++)
	  (p_tag_rfid+dummy_cmpt)->UID[dummy_cmpt_2]=dummy_tab[7-dummy_cmpt_2+1];

	__no_operation();
  }

  //return the number of tags detected
  return cmpt_tags;
  */
}


void loop() {


    delay(1500);

}
