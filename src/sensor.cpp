#include "sensor.h"
#include "config.h"
#include "wifi_mqtt.h"        // Pour le client MQTT
#include "mqtt_topics.h"      // Pour les noms des topics MQTT
#include <Wire.h>
#include <VL53L0X.h>
#include <Arduino.h>

VL53L0X sensor;
uint16_t distance;

void setup_sensor() {
  Wire.begin(PIN_SDA, PIN_SCL);
  sensor.setTimeout(500);
  while (!sensor.init()) {
    #ifdef DEBUG
    Serial.println("Échec d'initialisation du capteur de distance");
    #endif
    delay(500);
  }
  sensor.setMeasurementTimingBudget(200000);
  #ifdef DEBUG
  Serial.println("Capteur de distance initialisé");
  #endif
}

void handle_sensor() {
  distance = sensor.readRangeSingleMillimeters() - 20;
  if (distance < 10)
    distance = 0;
  if (distance > 800) {
    #ifdef DEBUG
    Serial.println("Erreur de mesure de distance");
    #endif
  }

  if (distance <= 800) {
    client.publish(distance_topic, String(distance).c_str(), 0);
    #ifdef DEBUG
    Serial.println(distance);
    #endif
  }

}