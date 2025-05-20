#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

// Pins
#define TFT_CS   10
#define TFT_DC   7
#define TFT_RST  -1
#define TFT_BL   9
#define TOUCH_CS 4

// TFT display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Touchscreen
XPT2046_Touchscreen ts(TOUCH_CS);

// Raw touch range (calibrate if needed)
#define TS_MINX  300
#define TS_MAXX  3700
#define TS_MINY  300
#define TS_MAXY  3700
#define TS_MIN_PRESSURE 10

// UI layout
#define BOXSIZE 40
#define PENRADIUS 10

int oldcolor, currentcolor;

 

void setup(void) {
  Serial.begin(115200);

  // Backlight pin
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // Initialize display
  tft.begin();
  tft.setRotation(1);           // Landscape mode
  tft.fillScreen(ILI9341_BLACK);

  // Display welcome message
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(20, 100);
  tft.print("Hello Mridul");

  delay(2000);  // Show message briefly
  tft.fillScreen(ILI9341_BLACK);  // Clear screen for drawing

  // Initialize touchscreen
  if (!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller");
    while (1);
  }
  ts.setRotation(1); // Match screen rotation

  //Draw color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
  tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
  tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
  tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
  tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);

  // Highlight default selected color
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
  currentcolor = ILI9341_RED;
}

void loop() {

  if (ts.touched()) {
    TS_Point p = ts.getPoint();

    // Ignore weak touches
    if (p.z < TS_MIN_PRESSURE) return;

    // Re-enable digital pins
    pinMode(TOUCH_CS, OUTPUT);
    digitalWrite(TOUCH_CS, HIGH);

    // Convert raw touch coordinates to screen coordinates
    int y = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    int x = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    Serial.print("Touch at: ");
    Serial.print(x); Serial.print(", ");
    Serial.println(y);

    if (y < BOXSIZE) {
      oldcolor = currentcolor;

      if (x < BOXSIZE) {
        currentcolor = ILI9341_RED;
      } else if (x < BOXSIZE * 2) {
        currentcolor = ILI9341_YELLOW;
      } else if (x < BOXSIZE * 3) {
        currentcolor = ILI9341_GREEN;
      } else if (x < BOXSIZE * 4) {
        currentcolor = ILI9341_CYAN;
      } else if (x < BOXSIZE * 5) {
        currentcolor = ILI9341_BLUE;
      } else if (x < BOXSIZE * 6) {
        currentcolor = ILI9341_MAGENTA;
      }

      // Highlight current color
      if (oldcolor != currentcolor) {
        tft.drawRect(0, 0, BOXSIZE, BOXSIZE, oldcolor);
        int index = (currentcolor == ILI9341_YELLOW) ? 1 :
                    (currentcolor == ILI9341_GREEN)  ? 2 :
                    (currentcolor == ILI9341_CYAN)   ? 3 :
                    (currentcolor == ILI9341_BLUE)   ? 4 :
                    (currentcolor == ILI9341_MAGENTA)? 5 : 0;
        tft.drawRect(index * BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
      }
    }

    // Draw only if below color bar area
    if ((y > BOXSIZE + PENRADIUS) && (y < tft.height() - PENRADIUS)) {
      tft.fillCircle(x, y, PENRADIUS, currentcolor);
    }

    delay(100);
  }
}
