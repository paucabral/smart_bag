#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(2, 3);      // RX, TX for ESP8266

bool DEBUG = true;   //show more logs
int responseTime = 10; //communication timeout

int print_id = 3;
String PostMessage = "";


void setup()
{
  pinMode(13,OUTPUT);  //set build in led as output
  // Open serial communications and wait for port to open esp8266:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  wifiSerial.begin(115200);

  sendToWifi("AT+RST",responseTime,DEBUG);
  //Serial.println("AT+RST");
  
  sendToWifi("AT+CWJAP=\"cabral\",\"17151613\"",responseTime,DEBUG);
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

}


void loop(){
  //Enter code here:
  PostMessage += "{\"print_id\":";
  PostMessage += "\"";
  PostMessage += print_id;
  PostMessage += "\"";
  PostMessage += "}";

  unsigned int l=PostMessage.length();

  String startMsg = "";
  startMsg += "AT+CIPSTART=0,";
  startMsg += "\"TCP\",";
  startMsg += "\"192.168.254.107\",";
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
  cipHost += "Host:192.168.254.107:5000";
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
  delay(30000);
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
