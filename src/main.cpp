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
  setup_led();
}

void loop() {
  check_wifi_connection();
  handle_mqtt();
  handle_button();
  handle_ota();
  checkDemandeLED();

  // Exécution périodique toutes les delayRefresh millisecondes
  static uint32_t previousMillis = 0;
  if (millis() - previousMillis >= delayRefresh) {
    previousMillis = millis();
    client.publish(pong_topic, "Connected");
    handle_poele();  // Met à jour les états du poêle
    handle_sensor();  // Met à jour 'distance'    
  }
}