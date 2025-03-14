#include <Arduino.h>
#include "wifi_mqtt.h"
#include "poele.h"
#include "sensor.h"
#include "button_led.h"
#include "ota.h"
#include "config.h"

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
  handle_poele();
  handle_sensor();
  handle_button();
  handle_ota();

  // Exécution périodique toutes les 'delayRefresh' millisecondes
  static uint32_t previousMillis = 0;
  if (millis() - previousMillis >= delayRefresh) {
    previousMillis = millis();
    client.publish(pong_topic, "Connected");
    getStates();
    handle_sensor();  // Met à jour la variable 'distance'
    if (distance <= 800) {
      client.publish(distance_topic, String(distance).c_str(), 0);
      #ifdef DEBUG
      Serial.println(distance);
      #endif
    }
  }
}