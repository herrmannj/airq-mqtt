#include "IAQ_Configuration.h"

bool Configuration::save() {
  if (SPIFFS.begin()) {
    File f = SPIFFS.open("/iaq.cfg", "w");
    if (!f) {
      DEBUG_PRINT(F("failed to write config file (1)"));
      return false;
    };
    //const size_t capacity = JSON_OBJECT_SIZE(3);
    DEBUG_PRINT(F("save config"));
    //DynamicJsonDocument doc(255); //heap
    StaticJsonDocument<512> doc;
    doc["mqtt_server"] = _mqtt_server;
    doc["mqtt_port"] = _mqtt_port;
    doc["mqtt_name"] = _mqtt_name;
    serializeJson(doc, Serial);
    serializeJson(doc, f);
    f.close();
    SPIFFS.end();
    changed = false;
    return true;
  };
  return false;
};

bool Configuration::load() {
  if (SPIFFS.begin()) {
    File f = SPIFFS.open("/iaq.cfg", "r");
    if (!f) {
      DEBUG_PRINT(F("failed to read config file (1)"));
      return false;
    };
    //const size_t capacity = JSON_OBJECT_SIZE(3);
    DEBUG_PRINT(F("read config"));
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, f);
    if (error) {
      DEBUG_PRINT(F("failed to read config file (1)"));
      return false;
    };
    strlcpy(_mqtt_server, doc["mqtt_server"], sizeof(_mqtt_server));
    strlcpy(_mqtt_port, doc["mqtt_port"], sizeof(_mqtt_port));
    strlcpy(_mqtt_name, doc["mqtt_name"], sizeof(_mqtt_name));
    f.close();
    SPIFFS.end();
    changed = false;
    return true;
  };
  return false;
};

void Configuration::get_mqtt_server(char buffer[]) {
  strlcpy(buffer, _mqtt_server, sizeof(_mqtt_server));
};

Configuration& Configuration::set_mqtt_server(const char* newValue) {
  uint16_t bsize = sizeof(_mqtt_server);
  if (strlcpy(_mqtt_server, newValue, bsize) >= bsize) {
    _mqtt_server[0] = '\0';
  };
};

void Configuration::get_mqtt_port(char buffer[]) {
  strlcpy(buffer, _mqtt_port, sizeof(_mqtt_port));
};

Configuration& Configuration::set_mqtt_port(const char* newValue) {
  uint16_t bsize = sizeof(_mqtt_port);
  if (strlcpy(_mqtt_port, newValue, bsize) >= bsize) {
    _mqtt_port[0] = '\0';
  };
};

void Configuration::get_mqtt_name(char buffer[]) {
  strlcpy(buffer, _mqtt_name, sizeof(_mqtt_name));
};

Configuration& Configuration::set_mqtt_name(const char* newValue) {
  uint16_t bsize = sizeof(_mqtt_name);
  if (strlcpy(_mqtt_name, newValue, bsize) >= bsize) {
    _mqtt_name[0] = '\0';
  };
};

const float& Configuration::led0_vLow() const {
  return _led0_vLow;
};

Configuration& Configuration::led0_vLow(const float& newValue) {
  _led0_vLow = newValue;
};

const float& Configuration::led0_vMedium() const {
  return _led0_vMedium;
};

Configuration& Configuration::led0_vMedium(const float& newValue) {
  _led0_vMedium = newValue;
};

const float& Configuration::led0_vHigh() const {
  return _led0_vHigh;
};

Configuration& Configuration::led0_vHigh(const float& newValue) {
  _led0_vHigh = newValue;
};

const float& Configuration::led0_cLow() const {
  return _led0_cLow;
};

Configuration& Configuration::led0_cLow(const float& newValue) {
  _led0_cLow = newValue;
};

const float& Configuration::led0_cMedium() const {
  return _led0_cMedium;
};

Configuration& Configuration::led0_cMedium(const float& newValue) {
  _led0_cMedium = newValue;
};

const float& Configuration::led0_cHigh() const {
  return _led0_cHigh;
};

Configuration& Configuration::led0_cHigh(const float& newValue) {
  _led0_cHigh = newValue;
};
