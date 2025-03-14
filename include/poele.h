#ifndef POELE_H
#define POELE_H

#include <SoftwareSerial.h>

// Déclaration de l'objet pour la communication série avec le poêle
extern SoftwareSerial StoveSerial;

// Variables globales utilisées pour le poêle
extern uint8_t flamePower;
extern uint8_t ventPower;
extern uint8_t demandeBlinkLED;
extern const uint8_t delayPublish;

// Fonctions de gestion du poêle
void setup_poele();
void checkStoveReply();
void write(uint8_t loc, uint8_t param, uint8_t data);
void getState(uint8_t loc, uint8_t param);
void getStates();
void handle_poele();

#endif