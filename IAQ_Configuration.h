#ifndef __IAQ_CONFIGURATION_H__
#define __IAQ_CONFIGURATION_H__

#include "IAQ_Globals.h"
#include "Arduino.h"
#include <ArduinoJson.h> 
#include <FS.h>

class Configuration
{
	public:
		bool changed = false;
		bool save();
		bool load();
		void get_mqtt_server(char buffer[]);
		Configuration& set_mqtt_server(const char* newValue);
		void get_mqtt_port(char buffer[]);
		Configuration& set_mqtt_port(const char* newValue);
    void get_mqtt_name(char buffer[]);
    Configuration& set_mqtt_name(const char* newValue);
		const float& led0_vLow() const;
		Configuration& led0_vLow(const float& newValue);
		const float& led0_vMedium() const;
		Configuration& led0_vMedium(const float& newValue);
		const float& led0_vHigh() const;
		Configuration& led0_vHigh(const float& newValue);
		const float& led0_cLow() const;
		Configuration& led0_cLow(const float& newValue);
		const float& led0_cMedium() const;
		Configuration& led0_cMedium(const float& newValue);
		const float& led0_cHigh() const;
		Configuration& led0_cHigh(const float& newValue);
	protected:
		// mqtt server
		char _mqtt_server[MAX_MQTT_SERVER_SIZE] = "";
		char _mqtt_port[MAX_MQTT_PORT_SIZE] = "1883";
    char _mqtt_name[MAX_MQTT_NAME_SIZE] = "";
		// char _mqtt_name
		// left led, temperature: values
		float _led0_vLow = 400;
		float _led0_vMedium = 800;
		float _led0_vHigh = 1200;
		// left led, temperature: color index
		float _led0_cLow = 510;
		float _led0_cMedium = 1020;
		float _led0_cHigh = 1530;
		// left led, co2: values
		float _led1_vLow = 400;
		float _led1_vMedium = 800;
		float _led1_vHigh = 1200;
		// left led, co2: color index
		float _led1_cLow = 510;
		float _led1_cMedium = 1020;
		float _led1_cHigh = 1530;
};

#endif
