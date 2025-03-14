#include "wifi_mqtt.h"
#include "config.h"
#include "mqtt_topics.h"  // Pour utiliser les topics MQTT
#include "poele.h"

WiFiClient espPoele;
PubSubClient client(espPoele);

void setup_wifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  #ifdef DEBUG
  Serial.print("Connexion au WiFi...");
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  #ifdef DEBUG
  Serial.println(" Connecté au WiFi");
  #endif
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("espPoele")) {
      delay(100);
      #ifdef DEBUG
      Serial.println("Connecté au MQTT");
      #endif
      client.subscribe(in_topic);
    }
  }
}

void setup_mqtt() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}

void check_wifi_connection() {
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }
}

void handle_mqtt() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void callback(char *topic, byte *payload, unsigned int length) {
  #ifdef DEBUG
  Serial.print("Message reçu [");
  Serial.print(topic);
  Serial.print("] ");
  for (uint8_t i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif
  
  // Traitement du message MQTT (adapté à vos besoins)
  if ((char)payload[1] == '1') {
    digitalWrite(THERMPIN, HIGH);
    if ((char)payload[2] != '9') {
      byte puissanceCall;
      if ((char)payload[2] == '1') {
        puissanceCall = 1;
      } else if ((char)payload[2] == '2') {
        puissanceCall = 2;
      } else if ((char)payload[2] == '3') {
        puissanceCall = 3;
      } else if ((char)payload[2] == '4') {
        puissanceCall = 4;
      } else if ((char)payload[2] == '5') {
        puissanceCall = 5;
      } else {
        puissanceCall = flamePower;
      }
      if (puissanceCall != flamePower) {
        write(WriteROM, flamePowerAddr, puissanceCall);
        getState(ReadROM, flamePowerAddr);
      }
    }
    if ((char)payload[3] != '9') {
      byte ventPowerCall;
      if ((char)payload[3] == '1') {
        ventPowerCall = 1;
      } else if ((char)payload[3] == '2') {
        ventPowerCall = 2;
      } else if ((char)payload[3] == '3') {
        ventPowerCall = 3;
      } else if ((char)payload[3] == '4') {
        ventPowerCall = 4;
      } else if ((char)payload[3] == '5') {
        ventPowerCall = 5;
      } else {
        ventPowerCall = ventPower;
      }
      if (ventPowerCall != ventPower) {
        write(WriteROM, ventPowerAddr, ventPowerCall);
        getState(ReadROM, ventPowerAddr);
      }
    }
  }
  else if ((char)payload[1] == '0') {
    digitalWrite(THERMPIN, LOW);
  }
  
  if ((char)payload[4] == '1') {
    digitalWrite(RESETPIN, LOW);
  }
  
  if ((char)payload[5] == '1') {
    demandeBlinkLED = 1;
  }
  else if ((char)payload[5] == '3') {
    demandeBlinkLED = 3;
  }
  else if ((char)payload[5] == '2') {
    demandeBlinkLED = 2;
  }
  else if ((char)payload[5] == '0') {
    demandeBlinkLED = 0;
  }
}