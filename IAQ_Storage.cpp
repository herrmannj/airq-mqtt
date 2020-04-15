#include "IAQ_Storage.h"


const uint16_t& Storage::co2() const {
  return _co2;
};

Storage& Storage::co2(const uint16_t& newValue) {
  _co2 = newValue;
};

const float& Storage::temperature() const {
  return _temperature;
};

Storage& Storage::temperature(const float& newValue) {
  _temperature = newValue;
};

const float& Storage::humidity() const {
  return _humidity;
};

Storage& Storage::pressure(const float& newValue) {
  _pressure = newValue;
};
const float& Storage::pressure() const {
  return _pressure;
};

Storage& Storage::humidity(const float& newValue) {
  _humidity = newValue;
};

const float& Storage::ahumidity() const {
  return _ahumidity;
};

Storage& Storage::ahumidity(const float& newValue) {
  _ahumidity = newValue;
};

const float& Storage::dewpoint() const {
  return _dewpoint;
};

Storage& Storage::dewpoint(const float& newValue) {
  _dewpoint = newValue;
};

const float& Storage::tvoc() const {
  return _tvoc;
};

Storage& Storage::tvoc(const float& newValue) {
  _tvoc = newValue;
};

const uint16_t& Storage::brightness() const {
  return _brightness;
};

Storage& Storage::brightness(const uint16_t& newValue) {
  _brightness = newValue;
};
void Storage::toJson() {
  //StaticJsonDocument<512> doc;
  const size_t capacity = JSON_OBJECT_SIZE(8);
  DynamicJsonDocument doc(capacity);
  doc["temperature"] = _temperature;
  doc["humidity"] = round(_humidity * 100) / 100;
  doc["pressure"] = round(_pressure * 100) / 100;
  doc["ahumidity"] = round(_ahumidity * 100) / 100;
  doc["dewpoint"] = round(_dewpoint * 100) / 100;
  doc["tvoc"] = int(round(_tvoc));
  doc["co2"] = _co2;
  doc["brightness"] = _brightness;
  serializeJsonPretty(doc, Serial);
};
