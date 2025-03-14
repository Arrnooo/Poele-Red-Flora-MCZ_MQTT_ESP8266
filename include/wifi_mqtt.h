#ifndef WIFI_MQTT_H
#define WIFI_MQTT_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Fonctions de gestion WiFi/MQTT
void setup_wifi();
void reconnect();
void setup_mqtt();
void check_wifi_connection();
void handle_mqtt();
void callback(char *topic, byte *payload, unsigned int length);

// Objets externes pour être accessibles dans d'autres modules
extern WiFiClient espPoele;
extern PubSubClient client;

#endif