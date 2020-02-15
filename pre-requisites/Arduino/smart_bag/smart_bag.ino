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


#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SIM900.h"
#include "sms.h"
SMSGSM sms;

#define mySerial Serial1
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

int solenoid = 6;

int numdata;
boolean started=false;
char smsbuffer[160];
char n[20];

void setup(){
  Serial.begin(9600);
  Serial.println("GSM Shield testing.");
  delay(100);
  
  if (gsm.begin(2400)){
    Serial.println("\nstatus=READY");
    started=true;  
  }
  else Serial.println("\nstatus=IDLE");
  
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

  pinMode(solenoid, OUTPUT);
  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

void textSuccess(){
  if(started){
    if (sms.SendSMS("09953148842", "Bag was opened successfully by a recognized fingerprint."))
      Serial.println("\nSMS sent OK");
  }  
}

void textFailed(){
  if(started){
    if (sms.SendSMS("09953148842", "Failed attempt to open the bag due to unrecognized fingerprint."))
      Serial.println("\nSMS sent OK");
  }  
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
    textFailed();
    digitalWrite(solenoid, LOW);
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
  textSuccess();
  digitalWrite(solenoid, HIGH);
  delay(2500);
  digitalWrite(solenoid, LOW);
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
    textFailed();
    digitalWrite(solenoid, LOW);
    return -1;
    }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK){
    Serial.println("No match found!");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("No match found!");
    display.display();
    textFailed();
    digitalWrite(solenoid, LOW);
    return -1;
    }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK){
    Serial.println("No match found!");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("No match found!");
    display.display();
    textFailed();
    digitalWrite(solenoid, LOW);
    return -1;
    }

  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Found ID #"); display.display(); display.print(finger.fingerID); display.display(); 
  textSuccess();
  digitalWrite(solenoid, HIGH);
  delay(2500);
  return finger.fingerID; 
}
