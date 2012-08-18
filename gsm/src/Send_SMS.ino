#include <SoftwareSerial.h>
 
SoftwareSerial gprsSerial(7, 8);
 
int btnPin = 12;
boolean prevBtn = LOW;
// +CSCS: ("IRA","GSM","UCS2","HEX","PCCP","PCDN","885AT+CMGF=1

 
void setup() {
    Serial.begin(57600);
    Serial.println("Setup");
    gprsSerial.begin(19200);
    delay(500);
    
    gprsSerial.println("AT+CSCS=?");
    delay(100); // даём время на усваивание команды
    ShowSerialData();
    // Мegafon 
    // +79219909090
    // +79262909090
    // +CSCA: "+79262909090",145

    // • МТС Москва:
    //+70957699100 (получение всех типов уведомлений) , 
    //+70957699101 (получение уведомления если сообщение не доставлено адресату) ,
    //+70957699102 ( уведомления не приходят)
    //+70957699800 служебный 
    
    // +CSCA: "+79168999100",145

    gprsSerial.println("AT+CSCA?");
    delay(100); 
    ShowSerialData();
     
    gprsSerial.println("AT+CMGF=?");
    delay(100); // даём время на усваивание команды
    ShowSerialData();
    
    
}
 
void loop() {
    boolean currBtn = digitalRead(btnPin);
    if (prevBtn != currBtn && currBtn == HIGH) {
        sendTextMessage();
    }
    prevBtn = currBtn;
}
 
/*
 * Функция отправки SMS-сообщения
 */
void sendTextMessage() {

   // Устанавливает текстовый режим для SMS-сообщений
    gprsSerial.print("AT+CMGF=1\r");
    delay(100); // даём время на усваивание команды
    // Устанавливаем адресата: телефонный номер в международном формате
    gprsSerial.println("AT + CMGS = \"+79253200641\"");
    delay(100);
    // Пишем текст сообщения
    gprsSerial.println("ALARM!");
    delay(100);
    // Отправляем Ctrl+Z, обозначая, что сообщение готово
    gprsSerial.println((char)26);
    
    return;
    
    // seems like everything is ok
    Serial.println("SEND");
    gprsSerial.println("AT+CMGF=0");
    delay(1000);
    ShowSerialData();
    
    gprsSerial.println("AT+CMGW=25");
    delay(1000);
    ShowSerialData();
    
    gprsSerial.print("0011000B919752230046F10000AA0CC8F71D14969741F977FD07");
    delay(1000);
    ShowSerialData();
    
    // Отправляем Ctrl+Z, обозначая, что сообщение готово
    gprsSerial.println((char)26);
    delay(2000);
    ShowSerialData();
 
    return
    //Устанавливает текстовый режим для SMS-сообщений
    //gprsSerial.print("AT+CMGF=1\r");
    delay(100); // даём время на усваивание команды
    //gprsSerial.print("AT+CSCS=\"UCS2\"\r");
    delay(100); // даём время на усваивание команды
    //gprsSerial.print("AT+CSMP=17,167,0,25\r");
    delay(100); // даём время на усваивание команды
    //ShowSerialData();
    
    // Устанавливаем адресата: телефонный номер в международном формате
    gprsSerial.println("AT + CMGS = \"*105*00#\"");
    delay(1000);
    //ShowSerialData();
   
    // Пишем текст сообщения
    gprsSerial.println("04 1F 04 40 04 38 04 32 04 35 04 42");
    delay(100);
    // Отправляем Ctrl+Z, обозначая, что сообщение готово
    gprsSerial.println((char)26);
    delay(2000);
    ShowSerialData();
}

void ShowSerialData()
{
  while(gprsSerial.available()!=0)
    Serial.write(gprsSerial.read());
}