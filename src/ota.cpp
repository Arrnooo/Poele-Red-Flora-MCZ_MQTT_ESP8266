#include "ota.h"
#include "ArduinoOTA.h"
#include "config.h"
#include <Arduino.h>

void setup_ota() {
  ArduinoOTA.setHostname("ESP8266_OTA");

  ArduinoOTA.onStart([]() {
    #ifdef DEBUG
    Serial.println("🔄 Démarrage OTA...");
    #endif
  });

  ArduinoOTA.onEnd([]() {
    #ifdef DEBUG
    Serial.println("\n✅ Mise à jour terminée !");
    #endif
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    #ifdef DEBUG
    Serial.printf("📶 Progression : %u%%\r", (progress / (total / 100)));
    #endif
  });

  ArduinoOTA.onError([](ota_error_t error) {
    #ifdef DEBUG
    Serial.printf("❌ Erreur OTA [%u] : ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Erreur d'authentification");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Erreur au démarrage");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Erreur de connexion");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Erreur de réception");
    else if (error == OTA_END_ERROR) Serial.println("Erreur de finalisation");
    #endif
  });

  ArduinoOTA.begin();
  #ifdef DEBUG
  Serial.println("OTA initialisé");
  #endif
}

void handle_ota() {
  ArduinoOTA.handle();
}