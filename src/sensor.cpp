#include "sensor.h"
#include "config.h"
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
  if (distance < 10) distance = 0;
  if (distance > 800) {
    #ifdef DEBUG
    Serial.println("Erreur de mesure de distance");
    #endif
  }
}