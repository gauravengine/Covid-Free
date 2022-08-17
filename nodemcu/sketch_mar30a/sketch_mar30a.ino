//IoT Based Pulse Heart Rate BPM and Oxygen SpO2 Monitor

#include <Wire.h>
#include <SPI.h>
#include "MAX30100_PulseOximeter.h"
//#define BLYNK_PRINT Serial
//#include <Blynk.h>
#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h>
//#include "Adafruit_GFX.h"
//#include "OakOLED.h"
//#include "SSD1306Ascii.h" //oled library
//#include "SSD1306AsciiWire.h" // oled library
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

#define REPORTING_PERIOD_MS 1000
//OakOLED oled;
//SSD1306AsciiWire oled;

//char auth[] = "xxxxx-xxxx-xxx";             // Authentication Token Sent by Blynk
//char ssid[] = "xxxxx-xxxx-xxx";        //WiFi SSID
//char pass[] = "xxxxx-xxxx-xxx";        //WiFi Password

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

//void printToScreen() {
//  display.clearDisplay();
//  display.display();
////  oled.clear();
////  float spo2=pox.getSpO2();
////  float heartRate=pox.getHeartRate();
////  oled.setCursor(0,0);
////  if(1) {
////    oled.print(F("HR: ")); oled.println(BPM, DEC);
////    oled.print(F("SPO2: ")); oled.println(SpO2, DEC);
////  } else {
////    oled.print(F("Not valid"));
////  }
////    oled.print(F("HR: ")); oled.println(BPM, DEC);
////    oled.print(F("SPO2: ")); oled.println(SpO2, DEC);
//  display.println("SPO2");
//  display.println("BPM");
//  display.display();
//}

//void onBeatDetected()
//{
//  Serial.println("Beat Detected!");
//  oled.drawBitmap( 60, 20, bitmap, 28, 28, 1);
//  oled.display();
//}

void setup()
{
  Serial.begin(115200);
//  oled.begin();
//oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
//  oled.clearDisplay();
//  oled.setTextSize(1);
//  oled.setTextColor(1);
//  oled.setCursor(0, 0);

//  oled.println("Initializing pulse oximeter..");
//  oled.display();
//    oled.begin(&Adafruit128x64, 0x3C);
//    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
//    oled.setFont(Arial14);
//    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
//    display.display();
//     display.clearDisplay();
//      display.display();
    pinMode(16, OUTPUT);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
    display.display();
    delay(1000);

  // Clear the buffer.
    display.clearDisplay();
    display.display();
//  Blynk.begin(auth, ssid, pass);

  Serial.print("Initializing Pulse Oximeter..");

  if (!pox.begin())
  {
    Serial.println("FAILED");
//    oled.clearDisplay();
//    oled.setTextSize(1);
//    oled.setTextColor(1);
//    oled.setCursor(0, 0);
//      oled.print(F("FAILED"));
//    oled.println("FAILED");
//    oled.display();
    for (;;);
  }
  else
  {
//    oled.clearDisplay();
//    oled.setTextSize(1);
//    oled.setTextColor(1);
//    oled.setCursor(0, 0);
//    oled.println("SUCCESS");
//    oled.display();
//    Serial.println("SUCCESS");
//      oled.print(F("SUCCESS"));
//      pox.setOnBeatDetectedCallback(onBeatDetected);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Success max");
  display.setCursor(0,0);
  display.display();
     Serial.println("SUCCESS");
  }

  // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
  //pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

}

void loop()
{
  pox.update();
//  Blynk.run();

  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
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
//    printToScreen();
//    Blynk.virtualWrite(V1, BPM);
//    Blynk.virtualWrite(V2, SpO2);
//    display.clearDisplay();
//    display.setTextSize(1);
//    display.setTextColor(SSD1306_WHITE);
//    display.setCursor(0,0);
//    oled.clearDisplay();
//    oled.setTextSize(1);
//    oled.setTextColor(1);
//    oled.setCursor(0, 16);
//    oled.println(pox.getHeartRate());
//
//    oled.setTextSize(1);
//    oled.setTextColor(1);
//    oled.setCursor(0, 0);
//    oled.println("Heart BPM");
//
//    oled.setTextSize(1);
//    oled.setTextColor(1);
//    oled.setCursor(0, 30);
//    oled.println("Spo2");
//
//    oled.setTextSize(1);
//    oled.setTextColor(1);
//    oled.setCursor(0, 45);
//    oled.println(pox.getSpO2());
//    oled.display();

    tsLastReport = millis();
  }
}
