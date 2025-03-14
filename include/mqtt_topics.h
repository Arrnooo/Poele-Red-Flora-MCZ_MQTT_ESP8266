#ifndef MQTT_TOPICS_H
#define MQTT_TOPICS_H

// Préfixe commun
#define mqtt_topic "Poele"

// Définition des topics
#define pong_topic         mqtt_topic "/pong"
#define state_topic        mqtt_topic "/state"
#define onoff_topic        mqtt_topic "/onoff"
#define ambtemp_topic      mqtt_topic "/ambtemp"
#define fumetemp_topic     mqtt_topic "/fumetemp"
#define flame_topic        mqtt_topic "/flamepower"
#define fumesRPM_topic     mqtt_topic "/fumesRPM"
#define ventPower_topic    mqtt_topic "/ventPower"
#define cyclePellet_topic  mqtt_topic "/cyclePellet"
#define pellet_topic       mqtt_topic "/pellet"
#define in_topic           mqtt_topic "/intopic"
#define distance_topic     mqtt_topic "/distance"
#define conso_topic        mqtt_topic "/conso"
#define puissance_topic    mqtt_topic "/puissanceChauffe"
#define sacvide_topic      mqtt_topic "/sacvide"

#endif