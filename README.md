# Projet ESP8266 - Gestion de Poêle via MQTT

Ce projet permet de contrôler et de surveiller un poêle à granulés en utilisant un ESP8266 (NodeMCU). Il communique avec un broker MQTT pour envoyer et recevoir des commandes.

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

const char *mqtt_server = "192.168.0.42";
const int mqtt_port = 1883;
```

## Installation et Configuration

1. **Installer PlatformIO** dans VS Code
2. **Cloner ce projet** :
   ```sh
   git clone https://github.com/votre-repo.git
   cd votre-repo
   ```
3. **Configurer `secrets.h` et 'config.cpp'** (voir section précédente)
4. **Compiler et téléverser** sur l'ESP8266 avec PlatformIO
5. **Démarrer un broker MQTT** (Mosquitto, Home Assistant, etc.)
6. **Vérifier les logs série** pour débugger

## Utilisation des commandes MQTT

### **Topics MQTT disponibles ezntre autres**

| Topic            | Valeur | Description |
|-----------------|--------|-------------|
| `poele/state`   | Off/On | État du poêle |
| `poele/temp`    | 0-50°C | Température ambiante |
| `poele/fan`     | 0-5    | Vitesse du ventilateur |
| `poele/flame`   | 0-5    | Puissance de chauffe |

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
