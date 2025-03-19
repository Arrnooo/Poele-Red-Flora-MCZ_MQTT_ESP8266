# Projet ESP8266 - Gestion de Poêle via MQTT

Ce projet permet de contrôler et de surveiller un poêle à granulés en utilisant un ESP8266 (NodeMCU). Il communique avec un broker MQTT pour envoyer et recevoir des commandes.
Présence d'un capteur VL53L0X pour connaitre le niveau de pellet dans la cuve.


## Fonctionnalités

- **Lecture des capteurs du poêle** (température ambiante, température des fumées, vitesse du ventilateur, etc.)
- **Contrôle de la puissance de chauffe et du ventilateur** via MQTT
- **Gestion des différents états du poêle** (arrêt, allumage, fonctionnement, nettoyage, etc.)
- **Indicateur LED de statut** avec différents modes de clignotement
- **Mise en veille profonde (Deep Sleep)** si la connexion MQTT est perdue
- **OTA (Over-The-Air Update)** pour mises à jour sans connexion physique

## Matériel requis

- **ESP8266** (NodeMCU, Wemos D1 Mini, etc.)
- **Module de communication série** (SoftwareSerial pour communiquer avec le poêle)
- **Broker MQTT** (ex: Mosquitto sur un Raspberry Pi ou un serveur)

![Screenshot 2025-03-17 at 11-54-51 EasyEDA(Standard) - Un outil de conception de circuits électroniques simple et puissant](https://github.com/user-attachments/assets/8e37bf87-8ef8-4a7c-86c6-ea9156d31235)
![Screenshot 2025-03-17 at 11-54-33 EasyEDA(Standard) - Un outil de conception de circuits électroniques simple et puissant](https://github.com/user-attachments/assets/070178de-76e1-475d-85b9-4710ec10a80c)
![Screenshot 2025-03-17 at 11-54-22 EasyEDA(Standard) - Un outil de conception de circuits électroniques simple et puissant](https://github.com/user-attachments/assets/68037ce9-d8f2-48f0-8779-4a3922dd3995)


## Structure du projet

### 1. **Fichiers principaux**

- `main.cpp` : Point d'entrée du programme
- `config.h` : Configuration générale
- `poele.cpp` / `poele.h` : Communication avec le poêle
- `wifi_mqtt.cpp` / `wifi_mqtt.h` : Gestion du WiFi et MQTT
- `button_led.cpp` / `button_led.h` : Gestion des boutons et LEDs
- `ota.cpp` / `ota.h` : Mises à jour OTA
- `sensor.cpp` / `sensor.h` : Gestion des capteurs
- `secrets.h` / `secrets.cpp` : Contient les informations sensibles (SSID, mot de passe WiFi, identifiants MQTT)

### 2. **Fichier `secrets.h`** (exclu de Git)

Pour stocker des informations sensibles comme le WiFi et MQTT, créez un fichier `secrets.h` :

```cpp
#ifndef SECRETS_H
#define SECRETS_H

// Informations WiFi
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PWD"

#endif
```

Puis, dans `config.cpp` :

```cpp
#include "config.h"

const char *mqtt_server = "192.168.***.***";
const int mqtt_port = 1883;
```

## Utilisation des commandes MQTT

### **Exemple de commande MQTT**

// Reception MQTT sous format 012345 // Jeedom envoi une suite de variable en fonction de la situation
// [0] Toujours 1, [1] On/Off (1;0), [2] puissance flame (1;2;3;4;5); [3] Puissance ventil (1;2;3;4.5), [4] Reset (0;1); [5] Action LED (0;1;2;3)
// Si un reset est demandé, il faut obligatoirement mettre la variable sur 0 après envoi par Jeedom

## Debugging et Logs

Utiliser la console série dans PlatformIO :
```sh
pio device monitor -b 115200
```
Ou vérifier les logs MQTT avec :
```sh
mosquitto_sub -h 192.168.1.100 -t "poele/#" -v
```
