#include "button_led.h"
#include "config.h"
#include "mqtt_topics.h"   // Pour sacvide_topic
#include "wifi_mqtt.h"     // Pour accéder à 'client'
#include "OneButtonTiny.h"
#include <Arduino.h>

// Blink LED
#define NbchangementEtat 10
#define blinkTime 150
#define blinkTimeSlow 500
uint32_t lastChange = 0;
uint8_t changementEtat = 0;
uint8_t lastEtat = HIGH;
uint8_t demandeBlinkLED = 0;

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

void setup_led() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void handle_button() {
  button.tick();
}

void checkDemandeLED(){
  switch (demandeBlinkLED) {
    case 0:
      digitalWrite(LED_BUILTIN, HIGH);
      lastEtat = HIGH;
      break;
    case 1:
      blinkLED();
      break;
    case 3:
      blinkLedSlow();
      break;
    case 2:
      digitalWrite(LED_BUILTIN, LOW);
      lastEtat = LOW;
      break;
  }
}

void blinkLedSlow()
{
  // Vérifiez si 500 ms se sont écoulés
  if (millis() - lastChange >= blinkTimeSlow)
  {
    lastChange = millis();            // Mettez à jour le temps de dernière modification
    lastEtat = !lastEtat;                // Changez l'état de la LED
    digitalWrite(LED_BUILTIN, lastEtat); // Appliquez le nouvel état
  }
}

void blinkLED()
{

  if (changementEtat >= NbchangementEtat)
  {
    lastChange = millis();
    demandeBlinkLED = 0;
    changementEtat = 0;
    digitalWrite(LED_BUILTIN, HIGH); 
    lastEtat = HIGH;                 
  }
  else if (millis() - lastChange >= blinkTime)
  {
    lastChange = millis();
    changementEtat++;
    digitalWrite(LED_BUILTIN, !lastEtat);
    lastEtat = !lastEtat;
  }
}