#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

// Pins
#define TFT_CS   10
#define TFT_DC   7
#define TFT_RST  -1
#define TFT_BL   9
#define TOUCH_CS 4

// TFT
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Touchscreen
XPT2046_Touchscreen ts(TOUCH_CS);

void setup() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  Serial.begin(9600);
  tft.begin();
  ts.begin();
  ts.setRotation(1);
  
  tft.fillScreen(ILI9341_BLUE);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(40, 120);
  tft.println("Touch to Test");
}

void loop() {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.println(p.y);

    tft.fillCircle(map(p.x, 0, 240, 0, 240), map(p.y, 0, 320, 0, 320), 3, ILI9341_RED);
    delay(200);
  }
}
