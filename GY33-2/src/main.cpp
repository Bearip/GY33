#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_SSD1306.h>
#include <SPI.h>

Adafruit_TCS34725 colorSensor(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_SSD1306 display(128, 64, &Wire);

const int colorTolerance = 50;
const int greenMax = 200;

void initializeColorSensor();
void initializeDisplay();
void readColorAndDisplay();

void setup() {
  Serial.begin(9600);
  
  Wire.begin();

  initializeColorSensor();
  initializeDisplay();
}

void loop() {
  readColorAndDisplay();
  delay(1500);
}

void initializeColorSensor() {
  if (colorSensor.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No GY-33 found, check connections");
    while (1);
  }
}

void initializeDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(500); 
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setRotation(0);
}

void readColorAndDisplay() {
  uint16_t r, g, b, clear, colorTemp, lux;

  colorSensor.getRawData(&r, &g, &b, &clear);
  colorTemp = colorSensor.calculateColorTemperature(r, g, b);
  lux = colorSensor.calculateLux(r, g, b);

  display.clearDisplay();
  display.setCursor(0, 0);

  // Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - "); 
  // Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - "); 
  // Serial.print("R: "); Serial.print(red, DEC); Serial.print(" ");
  // Serial.print("G: "); Serial.print(green, DEC); Serial.print(" ");
  // Serial.print("B: "); Serial.print(blue, DEC); Serial.print(" ");
  // Serial.print("C: "); Serial.print(clear, DEC); Serial.print(" ");
  // Serial.println(" ");

  if (r > b && r > 200 && r - b> colorTolerance && g < greenMax && clear < 1100) {
    Serial.println("Red Ball: " + String(r));
    display.println("Red Ball: " + String(r));
  } else if (b > r && b > 200 && b - r > colorTolerance && clear < 1100) {
    Serial.println("Blue Ball: " + String(b));
    display.println("Blue Ball: " + String(b));
  } else {
    Serial.println("NONE");
    display.println("NONE");
  }

  display.display();
}