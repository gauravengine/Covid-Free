//IoT Based Pulse Heart Rate BPM and Oxygen SpO2 Monitor

#include <Wire.h>
#include <SPI.h>
#include "MAX30100_PulseOximeter.h"

#include <ESP8266WiFi.h>

//#include "Adafruit_GFX.h"
//#include "OakOLED.h"
//#include "SSD1306Ascii.h" //oled library
//#include "SSD1306AsciiWire.h" // oled library
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>


Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

const char* ssid = "NSUTNEW";// 
const char* password = "sepl@123";
//WiFiClient client;
//char server[] = "192.168.0.103";   //eg: 192.168.0.222
char server[] = "192.168.161.252";

WiFiClient client;    
#define REPORTING_PERIOD_MS 1000
#define wifiTime 2000
//OakOLED oled;
//SSD1306AsciiWire oled;



// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0
PulseOximeter pox;

float BPM, SpO2;
uint32_t tsLastReport = 0;
int cnt=0;
const unsigned char bitmap [] PROGMEM =
{
  0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x00, 0x0f, 0xe0, 0x7f, 0x00, 0x3f, 0xf9, 0xff, 0xc0,
  0x7f, 0xf9, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xf0,
  0xff, 0xf7, 0xff, 0xf0, 0xff, 0xe7, 0xff, 0xf0, 0xff, 0xe7, 0xff, 0xf0, 0x7f, 0xdb, 0xff, 0xe0,
  0x7f, 0x9b, 0xff, 0xe0, 0x00, 0x3b, 0xc0, 0x00, 0x3f, 0xf9, 0x9f, 0xc0, 0x3f, 0xfd, 0xbf, 0xc0,
  0x1f, 0xfd, 0xbf, 0x80, 0x0f, 0xfd, 0x7f, 0x00, 0x07, 0xfe, 0x7e, 0x00, 0x03, 0xfe, 0xfc, 0x00,
  0x01, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x3f, 0xc0, 0x00,
  0x00, 0x0f, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void setup()
{
    Serial.begin(115200);

    pinMode(16, OUTPUT);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
    display.display();
    delay(1000);

 
    display.clearDisplay();
    display.display();


  
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
   
    WiFi.begin(ssid, password);
    int wifi_try_count=0;
    while (WiFi.status() != WL_CONNECTED && wifi_try_count<20) {
      delay(500);
      Serial.print(".");
      wifi_try_count++;
    }
    Serial.println("");
    Serial.println("WiFi connected");
   
    // Start the server
    //  server.begin();
    Serial.println("Server started");
    Serial.print(WiFi.localIP());
    delay(1000);
    Serial.println("connecting...");
  // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
  //pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  Serial.print("Initializing Pulse Oximeter..");

  if (!pox.begin())
  {
    Serial.println("FAILED");

    for (;;);
  }
  else
  {
    Serial.println("SUCCESS in initializing max30100");
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.print("Success max");
    display.setCursor(0,0);
     display.display();
     Serial.println("SUCCESS");
  }
}

void loop()
{    
    pox.update();
  //  Blynk.run();
  
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
  //  Sending_To_phpmyadmindatabase(SpO2,BPM);
//    if (millis() - tsLastReport > wifiTime){
//      Sending_To_phpmyadmindatabase(SpO2,BPM);
//    }
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
      Serial.print("Heart rate:");
      Serial.print(BPM);
      Serial.print(" SpO2:");
      Serial.print(SpO2);
      Serial.println(" %");
      display.clearDisplay();
      display.display();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0,0);
      display.print("SPO2  = ");
      display.print(pox.getSpO2());
      display.println("%");
      display.print("BPM  ");
      display.println(pox.getHeartRate());
      display.setCursor(0,0);
      display.display(); // actually display all of the above
  //    if (millis() - tsLastReport > wifiTime){
  //      Sending_To_phpmyadmindatabase(pox.getSpO2(),pox.getHeartRate());
  //    }
  //    Sending_To_phpmyadmindatabase(SpO2,BPM);
      if(cnt>20 && cnt <22 || (SpO2>50 && BPM>30)) Sending_To_phpmyadmindatabase(SpO2,BPM);
      tsLastReport = millis();
      cnt++;
    }

  
}
void Sending_To_phpmyadmindatabase(float myspo2,float mybpm)   //CONNECTING WITH MYSQL
 {
   if (client.connect(server,80) ) {
    
    Serial.println("connected");
    if(myspo2<50 || mybpm < 20) {
        Serial.println("Incorrect data not sending");
      }
    // Make a HTTP request:
    Serial.print("GET http://localhost/testcode/mySensorData.php?spo2=");
    client.print("GET http://localhost/testcode/mySensorData.php?spo2=");     //YOUR URL
    Serial.print(myspo2);
    client.print(myspo2);
    client.print("&bpm=");
    Serial.print("&bpm=");
    client.print(mybpm);
    Serial.println(mybpm);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: Your Local IP");
    client.println("Connection: close");
    client.println();

  
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }
