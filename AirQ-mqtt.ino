#include "IAQ_Globals.h"
//#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for WiFiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>
//#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#include "Tasker.h"
#include "IAQ_Configuration.h"
#include "IAQ_Storage.h"
#include "IAQ_BME680.h"
#include "IAQ_Led.h"
#include "MHZ.h"
#include <Wire.h>
#include <BH1750FVI.h>

Configuration configuration;
Storage storage;
Tasker tasker;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
IAQ_LED led;
IAQ_BME680 bme680;

MHZ co2(MH_Z19_RX, MH_Z19_TX, MHZ19B);
BH1750FVI::eDeviceMode_t DEVICEMODE = BH1750FVI::k_DevModeContHighRes2;
BH1750FVI LightSensor(DEVICEMODE);

//callback notifying us of the need to save config
void saveConfigCallback () {
  DEBUG_PRINT(F("Should save config"));
  configuration.changed = true;
};

void getMhz19Readings() {
  if (co2.isPreHeating()) {
    storage.co2(400);
    return;
  };
  uint16_t ppm_uart = co2.readCO2UART();

  if (ppm_uart > 0) {
    //Serial.print(ppm_uart);
    storage.co2(ppm_uart);
    //led.index2color(ppm_uart);
  } else {
    //Serial.print(400);
  };
};

// tasker callbacks
unsigned long readIAQ(uint8_t index) {
  getMhz19Readings();
  if (bme680.update()) {
    storage.temperature(bme680.temperature);
    storage.humidity(bme680.humidity);
    storage.pressure(bme680.pressure / 100);
    storage.ahumidity(bme680.ahumidity);
    storage.dewpoint(bme680.dewpoint);
    storage.tvoc(bme680.tvoc);
    //led.index2color(storage.co2());
    led.sign(storage.co2());
  };
  storage.toJson();
  DEBUG_PRINT(ESP.getFreeHeap());
  return 10000;
};

//unsigned long setLed(uint8_t index) {
//  uint16_t ci = random(400,1200);
//  led.index2color(storage.co2());
//  //led.setBri(1);
//  return 500;
//};

unsigned long readLux(uint8_t index) {
  uint16_t l = LightSensor.GetLightIntensity();
  // gain (case)
  l *= 0.5;
  storage.brightness(l);
  led.setBri(l);
  led.show();
  return 20;
};

// https://randomnerdtutorials.com/install-esp8266-filesystem-uploader-arduino-ide/
// https://forum.arduino.cc/index.php?topic=544478.0 ntp
void setup() {
  DEBUG_BEGIN; //for terminal debugging
  DEBUG_PRINT();

  char mqtt_server[MAX_MQTT_SERVER_SIZE];
  char mqtt_port[MAX_MQTT_PORT_SIZE];
  char mqtt_name[MAX_MQTT_NAME_SIZE];
  
  char ap_name[16];
  sprintf(ap_name, "IAQ-%X", ESP.getChipId());

  Serial.println(WiFi.SSID());
  Serial.println(WiFi.psk());

  //led, sign setup (purple)
  led.index2color(255);
  led.setBri(255);
  led.show();
  
  //preload config form
  configuration.load();
  configuration.get_mqtt_server(mqtt_server);
  configuration.get_mqtt_port(mqtt_port);
  configuration.get_mqtt_name(mqtt_name);

  if (mqtt_name[0] == '\0') {
    strlcpy(mqtt_name, ap_name, sizeof(mqtt_name));
  };
  
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, MAX_MQTT_SERVER_SIZE);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, MAX_MQTT_PORT_SIZE);
  WiFiManagerParameter custom_mqtt_name("name", "mqtt device-id", mqtt_name, MAX_MQTT_NAME_SIZE);

  //WiFiManager
  WiFiManager wifiManager;
  //wifiManager parameters
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_name);
  //wifiManager config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.setConfigPortalTimeout(20);
  wifiManager.startConfigPortal(ap_name);

  configuration.set_mqtt_server(custom_mqtt_server.getValue());
  configuration.set_mqtt_port(custom_mqtt_port.getValue());
  configuration.set_mqtt_name(custom_mqtt_name.getValue());
  configuration.save();

  configuration.get_mqtt_server(mqtt_server);
  configuration.get_mqtt_port(mqtt_port);
  configuration.get_mqtt_name(mqtt_name);

  LightSensor.begin();
  if (!bme680.begin()) {
    bme680.present = false;
    DEBUG_PRINT("no bme680 found");
  };

  if (mqtt_server[0] != '\0') {
    DEBUG_PRINTV("mqtt connect to server:\t");
    DEBUG_PRINT(mqtt_server);
    mqttClient.setServer(mqtt_server, 1883);
  } else {
    DEBUG_PRINT("no mqtt server set");
  }

  tasker.add(0, 10000, &readIAQ);
  //tasker.add(1, 500, &setLed);
  tasker.add(1, 200, &readLux);
};

void loop() {
  // put your main code here, to run repeatedly:
  tasker.run();
  delay(20);
  return;
};
