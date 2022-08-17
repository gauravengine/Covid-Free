//IoT Based Pulse Heart Rate BPM and Oxygen SpO2 Monitor

#include <Wire.h>
#include <SPI.h>
#include "MAX30100_PulseOximeter.h"


#include <WiFi.h>


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>






Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

char* ssid = "Schrodinger-fi";// 
char* password = "gengine123";
//WiFiClient client;
const char* host = "192.168.0.103";   //eg: 192.168.0.222


WiFiClient client;    
#define REPORTING_PERIOD_MS 1000
#define wifiTime 2000
//OakOLED oled;
//SSD1306AsciiWire oled;



// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0
PulseOximeter pox;

float BPM, SpO2;
uint32_t tsLastReport = 0;

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

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
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
  if (millis() - tsLastReport > wifiTime){
    Sending_To_phpmyadmindatabase(SpO2,BPM);
  }
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
    tsLastReport = millis();
  }
}
void Sending_To_phpmyadmindatabase(float myspo2,float mybpm)   //CONNECTING WITH MYSQL
 {
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

 


    // This will send the request to the server
  client.print(String("GET http://192.168.0.103/testcode/connect.php?") + 
                          ("&spo2=") + myspo2 +
                          ("&bpm=") + mybpm +
                          " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 1000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        
    }

    Serial.println();
    Serial.println("closing connection");
  
 }
