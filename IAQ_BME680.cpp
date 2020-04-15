#include "Arduino.h"
#include "IAQ_BME680.h"

#define BME680_DEBUG

IAQ_BME680::IAQ_BME680(void)
{
	
};

bool IAQ_BME680::begin(uint8_t addr)
{
	return Adafruit_BME680::begin(addr);
};

bool IAQ_BME680::update(void)
{
	if (!performReading()) {
		Serial.println(F("bme680 read failure"));
		return false;
	};
	ahumidity = calcAbsHum(temperature, humidity);
	dewpoint = calcDewPoint(temperature, humidity);

	// no gas reading yet
	if (gas_resistance == 0) {
		return true;
	};

	_vocCRes += 0.3 * ((gas_resistance * ahumidity) - _vocCRes);
	_vocBase += 0.0001 * (_vocCRes - _vocBase);
	// base line correction, low-pass cause of sensor latency 
	if (_vocBase < _vocCRes && !initialized) {
		_vocBase += 0.5 * (_vocCRes - _vocBase);
	} else if (_vocBase < _vocCRes && initialized) {
		_vocBase += 0.005 * (_vocCRes - _vocBase);
	} else if (!initialized) {
		initialized = true;
	};
	float ratio = _vocBase / _vocCRes;
	if (ratio < 1) {
		ratio = 1;
	};
	//float tV = (1250 * log(ratio)) + 125;
	tvoc = 125 + ((ratio - 1) * 1500);
	//Serial.println(_vocBase);
	//Serial.println(_vocCRes);
	//Serial.println(ratio);
	//Serial.println(tV);
	//Serial.println(tV * 3.2);
	//Serial.println(pressure / 100.0);
	return true;
};

float IAQ_BME680::calcAbsHum(float temp, float hum)
{
	double sdd, dd;
	sdd = 6.1078 * pow(10,(7.5 * temp) / (237.3 + temp));
	dd = hum / 100.0 * sdd;
	return (float) 216.687 * dd / (273.15 + temp);
};

float IAQ_BME680::calcDewPoint(float temp, float hum)
{
  double A = (hum/100) * pow(10, (7.5*temp / (237+temp)));
  return (float) 237*log10(A)/(7.5-log10(A));
};
