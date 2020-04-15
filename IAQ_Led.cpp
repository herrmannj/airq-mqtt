#include "IAQ_Led.h"

extern Configuration configuration;

Adafruit_NeoPixel *pixels;
bool pixelsRequireUpdate = false;

IAQ_LED::IAQ_LED(uint8_t ledNum) {
  if (pixels == NULL) {
    pixels = new Adafruit_NeoPixel(2, LED_PIN, NEO_GRB + NEO_KHZ800);
    pixels->begin();
  };
  _ledNum = ledNum;
};

void IAQ_LED::sign(float val) {
  
  float vLow, vMedium, vHigh; // values
  float cLow, cMedium, cHigh; // color index
  
  switch (_ledNum) {
    case 0:
      vLow = configuration.led0_vLow();
      vMedium = configuration.led0_vMedium();
      vHigh = configuration.led0_vHigh();
      cLow = configuration.led0_cLow();
      cMedium = configuration.led0_cMedium();
      cHigh = configuration.led0_cHigh();
      break;
    case 1:
      vLow = 400;
      vMedium = 800;
      vHigh = 1000;
      cLow = 510;
      cMedium = 1020;
      cHigh = 1530;
      break;
  };

  float cSignal;

  if (val <= vLow) {
    cSignal = cLow;
  } else if (val <= vMedium) {
    cSignal = ((val - vLow) / (vMedium - vLow) * (cMedium - cLow)) + cLow;
  } else if (val <= vHigh) {
    cSignal = ((val - vMedium) / (vHigh - vMedium) * (cHigh - cMedium)) + cMedium;
  } else {
    cSignal = cHigh;
  };
  index2color(round(cSignal));
};

// 0..1529 -> (HUE)359..0 -> / factor 4.25
uint32_t IAQ_LED::index2color(uint16_t colorIndex) {
  //uint8_t r,g,b;
  colorIndex %= 1530;
  // red to magenta
  if (colorIndex < (255)) {
    r = 255; g = 0; b = colorIndex;
    // magenta to blue
  } else if (colorIndex < (510)) {
    r = 510 - colorIndex; g = 0; b = 255;
    // blue to cyan
  } else if (colorIndex < (765)) {
    r = 0; g = colorIndex - 510; b = 255;
    // cyan to green
  } else if (colorIndex < (1020)) {
    r = 0; g = 255; b = 1020 - colorIndex;
    // green to yellow
  } else if (colorIndex < (1275)) {
    r = colorIndex - 1020; g = 255; b = 0;
    // yellow to red
  } else if (colorIndex < (1530)) {
    r = 255; g = 1530 - colorIndex; b = 0;
  };
  return uint32_t (r << 16 | g << 8 | b);
};

// pre-defined led brightness adaption profiles: low, high, gamma
float LED_PROFILE[3][3] =
{
  { -1.0, 300.0, 1.5},  // #0
  {0.2, 280.0, 1.3},      // #1
  { -0.2, 1024.0, 1.5}      // #2
};

void IAQ_LED::setBri(uint16_t lux) {

  uint8_t _profile = 0;
  float low = LED_PROFILE[_profile][0];
  float high = LED_PROFILE[_profile][1];
  float gamma = LED_PROFILE[_profile][2];

  float bri = (lux > low) ? pow((static_cast<float>(lux) - low) / (high - low), 1.0 / gamma) : 0;
  bri = (bri > 1) ? 1 : bri;

  if (bri != brightness) {
    pixelsRequireUpdate = true;   // update if brightness has changed
  };

  brightness += 0.1 * (bri - brightness);
  uint8_t rOut = round(r * brightness);
  uint8_t gOut = round(g * brightness);
  uint8_t bOut = round(b * brightness);

  pixels->setPixelColor(_ledNum, pixels->Color(gOut, rOut, bOut));
};

void IAQ_LED::show(void) {
  if (pixelsRequireUpdate) {
    pixels->show();
    pixelsRequireUpdate = false;
  };
};
