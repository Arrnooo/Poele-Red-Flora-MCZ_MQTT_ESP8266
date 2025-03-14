#ifndef POELE_H
#define POELE_H

#include <SoftwareSerial.h>

// Déclaration de l'objet pour la communication série avec le poêle
extern SoftwareSerial StoveSerial;

// Variables globales pour le poêle
extern uint8_t flamePower;
extern uint8_t ventPower;
extern uint8_t demandeBlinkLED;
extern uint8_t stoveState;
extern uint8_t fumesTemp;
extern uint16_t fumesRPM;
extern float ambTemp, cyclePellet, pellet, consoPellet, puissanceChauffe;
extern char stoveRxData[2];
extern uint8_t locCheksum;

// Fonctions de gestion du poêle
void setup_poele();
void checkStoveReply();
void write(uint8_t loc, uint8_t param, uint8_t data);
void getState(uint8_t loc, uint8_t param);
void getStates();
void handle_poele();

#endif