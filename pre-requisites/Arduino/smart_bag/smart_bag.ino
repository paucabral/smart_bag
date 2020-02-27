//R307 VCC Red 5V 
//R307 GND Blk
//R307 Yellow - INPUT (TX19)
//R307 White - OUTPUT (RX18)
//Relay VCC 5V S
//Relay + Output (6)
//Relay - GND
//RelayIN COM - VCC (5V, 9V)
//Solenoid COM, GND
//Solenoid VCC, NO
//I2C VCC 5V
//I2C GND
//I2C SDA SDA20
//I2C SCL SCL21
//GSM TX 10
//GSM RX 11
//ESP VCC 3.3 V
//ESP GND GND
//ESP TX TBA 12
//ESP RX TBA 13

#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SIM900.h"
#include "sms.h"
#include <SoftwareSerial.h>

SMSGSM sms;

#define mySerial Serial1
//#define wifiSerial Serial2

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUMFLAKES     10
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };


SoftwareSerial wifiSerial(12, 13); //RX, TX for ESP8266 - (For Uno, change for Mega)

int solenoid = 6;

int numdata;
boolean started=false;
char smsbuffer[160];
char n[20];

bool DEBUG = true;   //show more logs
int responseTime = 10; //communication timeout

String PostMessage = "";

void setup(){
  //Start Wifi Module
  // Open serial communications and wait for port to open esp8266:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  wifiSerial.begin(115200);

  sendToWifi("AT+RST",responseTime,DEBUG);
  //Serial.println("AT+RST");
  
  sendToWifi("AT+CWJAP=\"palit\",\"12345678\"",responseTime,DEBUG);
  //Serial.println("AT+CWJAP=\"cabral\",\"17151613\"");
  delay(4000);
  
  sendToWifi("AT+CWJAP?",responseTime,DEBUG);
  //Serial.println("AT+CWJAP?");
  delay(4000);
  
  sendToWifi("AT+CIFSR",responseTime,DEBUG);
  //Serial.println("AT+CIFSR");
  delay(1000);
  
  sendToWifi("AT+CIPMUX=1",responseTime,DEBUG);
  //Serial.println("AT+CIPMUX=1");
  delay(4000);
  
  sendToUno("Wifi connection is running!",responseTime,DEBUG);
  
  Serial.begin(9600);
  Serial.println("GSM Shield testing.");
  delay(100);
  //End Wifi Module

  //Start GSM Module
  if (gsm.begin(2400)){
    Serial.println("\nstatus=READY");
    started=true;  
  }
  else Serial.println("\nstatus=IDLE");
  //End GSM Module

  //Start OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  
  Serial.println("\n\nAdafruit finger detect test");
  display.println("Adafruit finger \ndetect test");
  display.display();
  delay(2000); 
  //End OLED

  //Start Fingerprint
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    display.clearDisplay();
    Serial.println("Found fingerprint sensor!");
    display.setCursor(0, 10);
    display.println("Found fingerprint \nsensor!");
    display.display();
    delay(2000); 
    display.clearDisplay();
    
  } else {
    display.clearDisplay();
    Serial.println("Did not find fingerprint sensor.");
    display.setCursor(0, 10);
    display.println("Did not find \nfingerprint sensor!");
    display.display();
    delay(2000);
    display.clearDisplay();
    while (1) { delay(1); }
  }
  //End Fingerprint

  //Start Solenoid
  pinMode(solenoid, OUTPUT);
  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  //End Solenoid
}

void textSuccess(){
  if(started){
    if (sms.SendSMS("09061771646", "Bag was opened successfully by a recognized fingerprint."))
      Serial.println("\nSMS sent OK");
  }  
}

void textFailed(){
  if(started){
    if (sms.SendSMS("09061771646", "Failed attempt to open the bag due to unrecognized fingerprint."))
      Serial.println("\nSMS sent OK");
  }  
}

//Success WiFi POST
void foundPOST(int id){ //Enter finger id as argument
  int print_id = id;
  PostMessage += "{\"print_id\":";
  PostMessage += "\"";
  PostMessage += print_id;
  PostMessage += "\"";
  PostMessage += "}";

  unsigned int l=PostMessage.length();

  String startMsg = "";
  startMsg += "AT+CIPSTART=0,";
  startMsg += "\"TCP\",";
  startMsg += "\"192.168.43.134\",";
  startMsg += "5000";
  sendToWifi(startMsg, responseTime, DEBUG);
  startMsg = "";
  delay(4000);
  
  String cipMsg = "";
  cipMsg += "AT+CIPSEND=0,134";
  sendToWifi(cipMsg, responseTime, DEBUG);
  cipMsg = "";
  delay(4000);

  String cipPOST = "";
  cipPOST += "POST /post/notif HTTP/1.1";
  sendToWifi(cipPOST, responseTime, DEBUG);
  cipPOST = "";

  String cipHost = "";
  cipHost += "Host:192.168.43.134:5000";
  sendToWifi(cipHost, responseTime, DEBUG);
  cipHost = "";

  String conLMsg = "";
  conLMsg = "Content-Length:16";
  sendToWifi(conLMsg, responseTime, DEBUG);
  conLMsg = "";

  String contentMsg = "Content-Type:application/json\n";
  sendToWifi(contentMsg, responseTime, DEBUG);
  contentMsg = "";
  
  sendToWifi(PostMessage, responseTime, DEBUG);

  PostMessage = "";

  String closeMsg = "";
  closeMsg += "AT+CIPCLOSE=0,";
  sendToWifi(closeMsg, responseTime, DEBUG);
}

//Error WiFi POST
void notfoundPOST(){ //print_id is 0 when the fingerprint was not recognized
  int print_id = 0;
  PostMessage += "{\"print_id\":";
  PostMessage += "\"";
  PostMessage += print_id;
  PostMessage += "\"";
  PostMessage += "}";

  unsigned int l=PostMessage.length();

  String startMsg = "";
  startMsg += "AT+CIPSTART=0,";
  startMsg += "\"TCP\",";
  startMsg += "\"192.168.43.134\",";
  startMsg += "5000";
  sendToWifi(startMsg, responseTime, DEBUG);
  startMsg = "";
  delay(4000);
  
  String cipMsg = "";
  cipMsg += "AT+CIPSEND=0,134";
  sendToWifi(cipMsg, responseTime, DEBUG);
  cipMsg = "";
  delay(4000);

  String cipPOST = "";
  cipPOST += "POST /post/notif HTTP/1.1";
  sendToWifi(cipPOST, responseTime, DEBUG);
  cipPOST = "";

  String cipHost = "";
  cipHost += "Host:192.168.43.134:5000";
  sendToWifi(cipHost, responseTime, DEBUG);
  cipHost = "";

  String conLMsg = "";
  conLMsg = "Content-Length:16";
  sendToWifi(conLMsg, responseTime, DEBUG);
  conLMsg = "";

  String contentMsg = "Content-Type:application/json\n";
  sendToWifi(contentMsg, responseTime, DEBUG);
  contentMsg = "";
  
  sendToWifi(PostMessage, responseTime, DEBUG);

  PostMessage = "";

  String closeMsg = "";
  closeMsg += "AT+CIPCLOSE=0,";
  sendToWifi(closeMsg, responseTime, DEBUG);
}


void loop(){
  getFingerprintID();
  delay(50);
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      digitalWrite(solenoid, LOW);
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("No finger detected");
      display.display();
      digitalWrite(solenoid, LOW);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      digitalWrite(solenoid, LOW);
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      digitalWrite(solenoid, LOW);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      digitalWrite(solenoid, LOW);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      digitalWrite(solenoid, LOW);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      digitalWrite(solenoid, LOW);
      return p;
    default:
      Serial.println("Unknown error");
      digitalWrite(solenoid, LOW);
      return p;
  }

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    digitalWrite(solenoid, LOW);
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("No match found!");
    display.display();
    
    digitalWrite(solenoid, LOW);
    notfoundPOST();
    delay(1000);
    textFailed();
    return p;
  } else {
    Serial.println("Unknown error");
    digitalWrite(solenoid, LOW);
    return p;
  }   
  
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Found ID #"); display.display(); display.print(finger.fingerID); display.display();
  
  digitalWrite(solenoid, HIGH);
  delay(2500);
  digitalWrite(solenoid, LOW);
  delay(1000);
  foundPOST(finger.fingerID);
  delay(4000);
  textSuccess();
  return finger.fingerID;
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK){
    Serial.println("No match found!");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("No match found!");
    display.display();
    
    digitalWrite(solenoid, LOW);
    notfoundPOST();
    delay(1000);
    textFailed();
    return -1;
    }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK){
    Serial.println("No match found!");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("No match found!");
    display.display();
    
    digitalWrite(solenoid, LOW);
    notfoundPOST();
    delay(1000);
    textFailed();
    return -1;
    }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK){
    Serial.println("No match found!");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("No match found!");
    display.display();
    
    digitalWrite(solenoid, LOW);
    notfoundPOST();
    delay(1000);
    textFailed();
    return -1;
    }

  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Found ID #"); display.display(); display.print(finger.fingerID); display.display(); 
  
  digitalWrite(solenoid, HIGH);
  delay(2500);
  digitalWrite(solenoid, LOW);
  delay(1000);
  foundPOST(finger.fingerID);
  delay(4000);
  textSuccess();
  return finger.fingerID; 
}

/*
* Name: sendData
* Description: Function used to send string to tcp client using cipsend
* Params: 
* Returns: void
*/
void sendData(String str){
  String len="";
  len+=str.length();
  sendToWifi("AT+CIPSEND=0,"+len,responseTime,DEBUG);
  delay(100);
  sendToWifi(str,responseTime,DEBUG);
  delay(100);
  sendToWifi("AT+CIPCLOSE=5",responseTime,DEBUG);
}


/*
* Name: find
* Description: Function used to match two string
* Params: 
* Returns: true if match else false
*/
boolean find(String string, String value){
  return string.indexOf(value)>=0;
}


/*
* Name: readSerialMessage
* Description: Function used to read data from Arduino Serial.
* Params: 
* Returns: The response from the Arduino (if there is a reponse)
*/
String  readSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(Serial.available()>0){
    value[index_count]=Serial.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}



/*
* Name: readWifiSerialMessage
* Description: Function used to read data from ESP8266 Serial.
* Params: 
* Returns: The response from the esp8266 (if there is a reponse)
*/
String  readWifiSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(wifiSerial.available()>0){
    value[index_count]=wifiSerial.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}



/*
* Name: sendToWifi
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendToWifi(String command, const int timeout, boolean debug){
  String response = "";
  wifiSerial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(wifiSerial.available())
    {
    // The esp has data so display its output to the serial window 
    char c = wifiSerial.read(); // read the next character.
    response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}

/*
* Name: sendToUno
* Description: Function used to send data to Arduino.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendToUno(String command, const int timeout, boolean debug){
  String response = "";
  Serial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(Serial.available())
    {
      // The esp has data so display its output to the serial window 
      char c = Serial.read(); // read the next character.
      response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}
