#ifndef __IAQ_BME680_H__
#define __IAQ_BME680_H__

#include <Adafruit_Sensor.h>              // Adafruit unified sensor library
#include <Adafruit_BME680.h>

class IAQ_BME680: public Adafruit_BME680
{
  public:
    IAQ_BME680(void);
    bool begin(uint8_t addr = BME680_DEFAULT_ADDRESS);
    bool update(void);
    float calcAbsHum(float temp, float hum);
    float calcDewPoint(float temp, float hum);

    float ahumidity;
    float dewpoint;
    float tvoc = 125;

    float _vocBase;
    float _vocCRes;
    bool initialized;   // voc abc stable
    bool present = true;
};

#endif
