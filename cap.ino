//#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define NUM_LEDS 256
#define DATA_PIN 5

#include "WiFi.h"

// Horizontal layout
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, DATA_PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS    + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800
);

// ESP8266WiFiMulti wifiMulti;

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(255, 255, 0), matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 255)
};
int pixelPerChar = 5; // Width of Standard Font Characters is 8X6 Pixels
int x = 32; // Width of the Display
int pass = 0; // Counter
int i = 0; // Counter
int clr = 0; // Counter for Indexing Array of Colors

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(20);
  matrix.setTextColor(matrix.Color(80,255,0));


  // Setup Wifi Server
  setupServer();

  // If using WPA2 Enterprise
  //  set_up_wpa2();

  //  wifiMulti.addAP(WiFi_SSID, WiFi_PASS);
  //
  //  while (wifiMulti.run() != WL_CONNECTED) {
  //    delay(1000);
  //    Serial.print('.');
  //  }
  //
  //  Serial.print("Connected to ");
  //  Serial.println(WiFi.SSID());             // Tell us what network we're connected to
  //  Serial.print("IP address:\t");
  //  Serial.print(WiFi.localIP());
}

int count = 0;

void loop() {
  runServer();

  count += 1;

  if (count >= 100) {
    writeText(printMessage);
    count = 0;
  }
}

/* @author adavegetable
 *  @link https://forums.adafruit.com/viewtopic.php?t=115535
 */
void writeText(String msg) {
  int arrSize = sizeof(colors) / sizeof(*colors); // Array of Text Colors;
  int msgSize = (msg.length() * pixelPerChar) + (2 * pixelPerChar); // CACULATE message length;
  int scrollingMax = (msgSize) + matrix.width(); // ADJUST Displacement for message length;

  x = matrix.width(); // RESET Cursor Position and Start Text String at New Position on the Far Right;
  clr = 0; // RESET Color/Repeat Index;

  while (clr <= arrSize) {
    /* Change Color with Each Pass of Complete Message */
    matrix.setTextColor(colors[clr]);

    matrix.fillScreen(0); // BLANK the Entire Screen;
    matrix.setCursor(x, 0); // Set Starting Point for Text String;
    matrix.print(msg); // Set the Message String;

    /* SCROLL TEXT FROM RIGHT TO LEFT BY MOVING THE CURSOR POSITION */
    if (--x < -scrollingMax ) {
      /*  ADJUST FOR MESSAGE LENGTH  */
      // Decrement x by One AND Compare New Value of x to -scrollingMax;
    // This Animates (moves) the text by one pixel to the Left;

      x = matrix.width(); // After Scrolling by scrollingMax pixels, RESET Cursor Position and Start String at New Position on the Far Right;
      ++clr; // INCREMENT COLOR/REPEAT LOOP COUNTER AFTER MESSAGE COMPLETED;
    }
    matrix.show(); // DISPLAY the Text/Image
    delay(80); // SPEED OF SCROLLING or FRAME RATE;
  }
  clr = 0; // Reset Color/Loop Counter;

/* LATHER - RINSE - REPEAT - Why coders have such nice hair */
}
