#include "button_led.h"
#include "config.h"
#include "mqtt_topics.h"   // Pour sacvide_topic
#include "wifi_mqtt.h"     // Pour accéder à 'client'
#include "OneButtonTiny.h"
#include <Arduino.h>

OneButtonTiny button(BOUTON, true);

static void myClickFunction() {
  client.publish(sacvide_topic, "Sac vide");
  // Vous pouvez ajouter ici d'autres actions si nécessaire
}

void setup_button() {
  button.attachClick(myClickFunction);
  #ifdef DEBUG
  Serial.println("Bouton initialisé");
  #endif
}

void handle_button() {
  button.tick();
}