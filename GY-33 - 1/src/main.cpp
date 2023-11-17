#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_SSD1306.h>
#include<SPI.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

const int cTol = 50;
const int gMax = 200;

void setup() {
  Serial.begin(9600);
  
  Wire.begin();

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No GY-33 found, check connections");
    while (1);
  }

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; 
  }
  display.display();
  delay(500); 
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setRotation(0);
}

void loop() {            
  uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  display.clearDisplay();
  display.setCursor(0, 0);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - "); 
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - "); 
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");


  // if (r>b && r > 200 && r-b > cTol && g < gMax) {
  if (r>b && r > 200 && r-b > cTol && g < gMax  && c <1100) {
    Serial.println("Red Ball: " + String(r));
    display.println("Red Ball: " + String(r));
  }

  // else if (b>r && b > 200 && b-r > cTol && g < gMax) {
  else if (b>r && b > 200 && b-r > cTol && c <1100 ) {
    Serial.println("Blue Ball: " + String(b));
    display.println("Blue Ball: " + String(b));
  }
  else{
    Serial.println("NONE");
    display.println("NONE");
  }

  display.display();
  delay(1500);
}
