//R307 VCC Red 5V
//R307 GND Blk
//R307 Yellow - INPUT (2)
//R307 White - OUTPUT (3)
//Relay VCC 5V S
//Relay + Output (4)
//Relay - GND
//RelayIN COM - VCC (5V, 9V)
//Solenoid COM, GND
//I2C VCC 5V
//I2C GND
//I2C SDA A4
//I2C SCL A5


#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)  
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

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

int solenoid = 4;

void setup()  
{
  Serial.begin(9600);
  delay(100);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  
  Serial.println("\n\nAdafruit finger detect test");
  display.println("Adafruit finger \ndetect test");
  display.display();
  delay(2000); 
  
  // set the data rate for the sensor serial port
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

void loop(){
  getFingerprintID();
  delay(50);            //don't ned to run this at full speed.
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

  // OK success!

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
  
  // OK converted!
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
    return p;
  } else {
    Serial.println("Unknown error");
    digitalWrite(solenoid, LOW);
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Found ID #"); display.display(); display.print(finger.fingerID); display.display();

  
  digitalWrite(solenoid, HIGH);
  delay(2500);
  digitalWrite(solenoid, LOW);
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK){
    Serial.println("No match found!");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("No match found!");
    display.display();
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
    digitalWrite(solenoid, LOW);
    return -1;
    }
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Found ID #"); display.display(); display.print(finger.fingerID); display.display(); 
  
  digitalWrite(solenoid, HIGH);
  delay(2500);
  return finger.fingerID; 
}
