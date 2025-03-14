#include <Arduino.h>
#include "button_led.h"
#include "config.h"
#include "wifi_mqtt.h"  // Pour utiliser 'client' dans la fonction de clic
#include "OneButtonTiny.h"

OneButtonTiny button(BOUTON, true);

static void myClickFunction() {
  client.publish(sacvide_topic, "Sac vide");
  // Optionnel : définir demandeBlinkLED si besoin
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