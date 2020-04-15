#ifndef __IAQ_GLOBALS_H__
#define __IAQ_GLOBALS_H__

#define DEBUG_SERIAL
#ifdef DEBUG_SERIAL
#define DEBUG_BEGIN Serial.begin(115200)
#define DEBUG_PRINTV(x) Serial.print(x)
#define DEBUG_PRINT(x) Serial.println(x)
#else
#define DEBUG_BEGIN
#define DEBUG_PRINTV(x)
#define DEBUG_PRINT(x)
#endif

// pin for MH-Z19B uart reading
#define MH_Z19_RX D7
#define MH_Z19_TX D8

#define MAX_MQTT_SERVER_SIZE 40
#define MAX_MQTT_PORT_SIZE 6
#define MAX_MQTT_NAME_SIZE 40

#endif
