#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>   // Pour avoir uint8_t, uint16_t, etc.

// Informations WiFi
#define WIFI_SSID "HibouHome"
#define WIFI_PASSWORD "0630232210"

// Informations MQTT
const char *mqtt_server = "192.168.0.42";
const int mqtt_port = 1883;

// Délais et paramètres divers
const uint8_t delayPublish = 200;
const uint16_t delayRefresh = 60000;
const uint16_t delayDeepSleep = 60000;

// Définition des pins
#define ENABLE_RX D8
#define RX_PIN D5
#define TX_PIN D6
#define THERMPIN D7
#define BOUTON D3
#define RESETPIN D0
#define LED_BUILTIN 2  // Exemple pour ESP8266, vérifie selon ta carte

// Pour le capteur VL53L0X
#define PIN_SDA D1
#define PIN_SCL D2

// Paramètres du poêle
const uint8_t grPelletMin = 50;  // débit en grammes/min

#endif