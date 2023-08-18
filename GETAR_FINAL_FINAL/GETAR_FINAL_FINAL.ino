#include <FastLED.h>
#include <Arduino.h>

#define DATA_PIN 3
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 40
CRGB leds[NUM_LEDS];

#define BRIGHTNESS 96

void setup() {
  Serial.begin(9600);
  delay(3000);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}


double magicGradientR(int val)
{
  return 0.1 + 0.9 * abs(sin((double)val * 0.05));
}

double magicGradientB(int val)
{
  return 0.1 + 0.9 * abs(sin((double)val * 0.09));
}

double magicGradientG(int val)
{
  return 0.1 + 0.9 * abs(sin((double)val * 0.14));
}


int prevLoudness = 0;
int ctr = 0;
void loop() {
  ctr += 1;
  int newSig = analogRead(A0) - 512;
  // normalize the signal, the loudness is now from zero to 512.
  int newSigAbs = abs(newSig);
  // take the louder of the last signal or the current signal
  prevLoudness = max(newSigAbs, prevLoudness);

  // decay
  prevLoudness -= 2;
  if(prevLoudness < 0)
  {
    prevLoudness = 0;
  }
  Serial.println(prevLoudness);


// normal  mode
  for(int i = 0; i != NUM_LEDS; ++i)
  {
    leds[i].setRGB(
      min(256, prevLoudness),
      min(256, prevLoudness),
      min(256, prevLoudness));
  }

// fancy mode
  for(int i = 0; i != NUM_LEDS; ++i)
  {
    leds[i].setRGB(
      min(256, (int)(double(prevLoudness) * magicGradientR(ctr+i))),
      min(256, (int)(double(prevLoudness) * magicGradientG(ctr+i))),
      min(256, (int)(double(prevLoudness) * magicGradientB(ctr+i)))
      );
  }
// end off fancy mode
  
  delay(10);
  FastLED.show();
}


