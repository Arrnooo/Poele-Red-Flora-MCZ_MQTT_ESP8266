#include "poele.h"
#include "mqtt_topics.h"   // Pour les noms des topics (voir ci-dessous)
#include "config.h"
#include "wifi_mqtt.h"     // Pour accéder à 'client'
#include <Arduino.h>

SoftwareSerial StoveSerial(RX_PIN, TX_PIN);  // Utilise les pins définis dans config.h

uint8_t flamePower = 0;
uint8_t ventPower = 0;
uint8_t demandeBlinkLED = 0;
const uint8_t delayPublish = delayPublish;  // Utilise la constante définie dans config.h

uint8_t stoveState, fumesTemp, locCheksum;
uint16_t fumesRPM;
float ambTemp, cyclePellet, pellet, consoPellet, puissanceChauffe;
char stoveRxData[2];

void setup_poele() {
  StoveSerial.begin(1200, SERIAL_MODE, RX_PIN, TX_PIN, false, 256);
  pinMode(ENABLE_RX, OUTPUT);
  digitalWrite(ENABLE_RX, LOW);
  #ifdef DEBUG
  Serial.println("Poêle initialisé");
  #endif
}

void checkStoveReply() {
  digitalWrite(ENABLE_RX, HIGH);
  delay(80);
  uint8_t rxCount = 0;
  stoveRxData[0] = 0x00;
  stoveRxData[1] = 0x00;
  while (StoveSerial.available()) {
    stoveRxData[rxCount] = StoveSerial.read();
    rxCount++;
  }
  digitalWrite(ENABLE_RX, LOW);
  if (rxCount == 2) {
    uint8_t val = stoveRxData[1];
    uint8_t checksum = stoveRxData[0];
    uint8_t param = checksum - val - locCheksum;
    #ifdef DEBUG
    Serial.printf("Param=%01x value=%01x ", param, val);
    #endif
    switch (param) {
      case stoveStateAddr:
        stoveState = val;
        switch (stoveState) {
          case 0:
            client.publish(state_topic, "Off", true);
            delay(delayPublish);
            client.publish(onoff_topic, "OFF", true);
            break;
          case 1:
            client.publish(state_topic, "Starting", true);
            delay(delayPublish);
            client.publish(onoff_topic, "ON", true);
            break;
          case 2:
            client.publish(state_topic, "Pellet loading", true);
            delay(delayPublish);
            client.publish(onoff_topic, "ON", true);
            break;
          case 3:
            client.publish(state_topic, "Ignition", true);
            delay(delayPublish);
            client.publish(onoff_topic, "ON", true);
            break;
          case 4:
            client.publish(state_topic, "Working", true);
            delay(delayPublish);
            client.publish(onoff_topic, "ON", true);
            break;
          case 5:
            client.publish(state_topic, "Brazier cleaning", true);
            break;
          case 6:
            client.publish(state_topic, "Final cleaning", true);
            delay(delayPublish);
            client.publish(onoff_topic, "OFF", true);
            break;
          case 7:
            client.publish(state_topic, "Standby", true);
            delay(delayPublish);
            client.publish(onoff_topic, "OFF", true);
            break;
          case 8:
            client.publish(state_topic, "Pellet missing", true);
            break;
          case 9:
            client.publish(state_topic, "Ignition failure", true);
            delay(delayPublish);
            client.publish(onoff_topic, "OFF", true);
            break;
          case 10:
            client.publish(state_topic, "Alarm", true);
            break;
        }
        #ifdef DEBUG
        Serial.printf("Stove %s\n", stoveState ? "ON" : "OFF");
        #endif
        break;
      case ambTempAddr:
        ambTemp = (float)val / 2;
        client.publish(ambtemp_topic, String(ambTemp).c_str(), 0);
        #ifdef DEBUG
        Serial.print("T. amb. ");
        Serial.println(ambTemp);
        #endif
        break;
      case fumesTempAddr:
        fumesTemp = val;
        client.publish(fumetemp_topic, String(fumesTemp).c_str(), 0);
        #ifdef DEBUG
        Serial.printf("T. fumes %d\n", fumesTemp);
        #endif
        break;
      case flamePowerAddr:
        flamePower = val;
        client.publish(flame_topic, String(flamePower).c_str(), 0);
        #ifdef DEBUG
        Serial.printf("Fire %d\n", flamePower);
        #endif
        break;
      case fumesRPMAddr:
        fumesRPM = val * 10 + 250;
        if (fumesRPM <= 250) { fumesRPM = 0; }
        client.publish(fumesRPM_topic, String(fumesRPM).c_str(), 0);
        #ifdef DEBUG
        Serial.printf("fumesRPM %d\n", fumesRPM);
        #endif
        break;
      case ventPowerAddr:
        ventPower = val;
        client.publish(ventPower_topic, String(ventPower).c_str(), 0);
        #ifdef DEBUG
        Serial.printf("ventPower %d\n", ventPower);
        #endif
        break;
      case cyclePelletAddr:
        cyclePellet = (float)val / 10;
        client.publish(cyclePellet_topic, String(cyclePellet).c_str(), 0);
        #ifdef DEBUG
        Serial.print("cyclePellet ");
        Serial.println(cyclePellet);
        #endif
        break;
      case pelletAddr:
        pellet = (float)val / 10;
        client.publish(pellet_topic, String(pellet).c_str(), 0);
        #ifdef DEBUG
        Serial.print("pellet ");
        Serial.println(pellet);
        #endif
        consoPellet = pellet / cyclePellet * grPelletMin;
        client.publish(conso_topic, String(consoPellet).c_str(), 0);
        #ifdef DEBUG
        Serial.print("consoPellet ");
        Serial.println(consoPellet);
        #endif
        puissanceChauffe = consoPellet * 60 * 5;
        client.publish(puissance_topic, String(puissanceChauffe).c_str(), 0);
        #ifdef DEBUG
        Serial.print("puissanceChauffe ");
        Serial.println(puissanceChauffe);
        #endif
        break;
    }
  }
}

void write(uint8_t loc, uint8_t param, uint8_t data) {
  uint8_t checksum = loc + param + data;
  StoveSerial.write(loc);
  delay(1);
  StoveSerial.write(param);
  delay(1);
  StoveSerial.write(data);
  delay(1);
  StoveSerial.write(checksum);
  delay(200);
}

void getState(uint8_t loc, uint8_t param) {
  locCheksum = loc;
  StoveSerial.write(loc);
  delay(1);
  StoveSerial.write(param);
  checkStoveReply();
  locCheksum = 0;
}

void getStates() {
  getState(ReadRAM, stoveStateAddr);
  getState(ReadRAM, fumesRPMAddr);
  getState(ReadRAM, cyclePelletAddr);
  getState(ReadRAM, pelletAddr);
  getState(ReadRAM, ambTempAddr);
  getState(ReadRAM, fumesTempAddr);
  getState(ReadROM, ventPowerAddr);
  getState(ReadROM, flamePowerAddr);
}

void handle_poele() {
  getStates();
  checkStoveReply();
}