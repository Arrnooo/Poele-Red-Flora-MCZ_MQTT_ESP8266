#include <Arduino.h>
#include "wifi_mqtt.h"
#include "poele.h"
#include "sensor.h"
#include "button_led.h"
#include "ota.h"
#include "config.h"
#include "mqtt_topics.h"  // Pour utiliser pong_topic et distance_topic

void setup() {
  #ifdef DEBUG
  Serial.begin(115200);
  #endif

  setup_wifi();
  setup_mqtt();
  setup_poele();
  setup_sensor();
  setup_button();
  setup_ota();
}

void loop() {
  check_wifi_connection();
  handle_mqtt();
  handle_button();
  handle_ota();

  // Exécution périodique toutes les delayRefresh millisecondes
  static uint32_t previousMillis = 0;
  if (millis() - previousMillis >= delayRefresh) {
    previousMillis = millis();
    client.publish(pong_topic, "Connected");
    getStates();  // Met à jour les états du poêle
    handle_sensor();  // Met à jour 'distance'
    if (distance <= 800) {
      client.publish(distance_topic, String(distance).c_str(), 0);
      #ifdef DEBUG
      Serial.println(distance);
      #endif
    }
  }
}