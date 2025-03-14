#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>  // Pour uint8_t, uint16_t, etc.

// ===== Paramètres WiFi =====
#define WIFI_SSID       "HibouHome"
#define WIFI_PASSWORD   "0630232210"

// ===== Paramètres MQTT =====
extern const char *mqtt_server;
extern const int mqtt_port;

// ===== Délais =====
const uint8_t delayPublish = 200;
const uint16_t delayRefresh = 60000;
const uint16_t delayDeepSleep = 60000;

// ===== Définition des pins =====
#define ENABLE_RX       D8
#define RX_PIN          D5
#define TX_PIN          D6
#define THERMPIN        D7
#define BOUTON          D3
#define RESETPIN        D0
#define LED_BUILTIN     2    // Vérifiez selon votre carte ESP8266

// ===== Pour le capteur VL53L0X =====
#define PIN_SDA         D1
#define PIN_SCL         D2

// ===== Paramètres du poêle =====
#define ReadRAM         0x00
#define ReadROM         0x20
#define WriteRAM        0x80
#define WriteROM        0xA0
#define stoveStateAddr  0x21
#define fumesRPMAddr    0x37
#define cyclePelletAddr 0x0C
#define pelletAddr      0x0D
#define ambTempAddr     0x01
#define fumesTempAddr   0x65
#define ventPowerAddr   0x7E  // ROM
#define flamePowerAddr  0x7F  // ROM
const uint8_t grPelletMin = 50;  // débit en grammes/min

// ===== Définition du mode série logiciel =====
#define SERIAL_MODE SWSERIAL_8N2

#endif