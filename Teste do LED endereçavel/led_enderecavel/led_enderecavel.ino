#include <Adafruit_NeoPixel.h>

#define PIN 2
#define NUMLED 8


Adafruit_NeoPixel pixels(NUMLED, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYLOOP 100

void setup() 
{
  pixels.begin();
}

void loop() 
{
  pixels.clear();

  for (int i = 0; i < NUMLED; i++) 
  {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));

    pixels.show();

    delay(DELAYLOOP);
  }
}
