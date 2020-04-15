#ifndef __IAQ_LED_H__
#define __IAQ_LED_H__
#include "Arduino.h"
#include "IAQ_Configuration.h"
#include <Adafruit_NeoPixel.h>

#ifndef LED_PIN
#define LED_PIN D6  // D6 12
#endif

class IAQ_LED
{
  public:
    IAQ_LED(uint8_t ledNum = 0);
    void setBri(uint16_t lux);
    void sign(float val);
    void show(void);
    //void setColorCode(uint16_t vBlue, uint16_t vGreen, uint16_t vRed);
    //void setValue(uint16_t val);
    uint32_t index2color(uint16_t colorIndex);

    float brightness;
    uint16_t r;
    uint16_t g;
    uint16_t b;
  protected:
    uint8_t _ledNum;
};

#endif
