#ifndef __IAQ_STORAGE_H__
#define __IAQ_STORAGE_H__

#include "Arduino.h"
#include <ArduinoJson.h>

class Storage
{
  public:
    const uint16_t& co2() const;
    Storage& co2(const uint16_t& newValue);
    const float& temperature() const;
    Storage& temperature(const float& newValue);
    const float& humidity() const;
    Storage& humidity(const float& newValue);
    const float& pressure() const;
    Storage& pressure(const float& newValue);
    const float& ahumidity() const;
    Storage& ahumidity(const float& newValue);
    const float& dewpoint() const;
    Storage& dewpoint(const float& newValue);
    const float& tvoc() const;
    Storage& tvoc(const float& newValue);
    const uint16_t& brightness() const;
    Storage& brightness(const uint16_t& newValue);
    void toJson(void);
  protected:
    uint16_t _co2;
    float _temperature;
    float _humidity;
    float _pressure;
    float _ahumidity;
    float _dewpoint;
    float _tvoc;
    uint16_t _brightness;
};

#endif
