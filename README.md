/!\ Fonctionnel mais quelques bugs sur l'allumage de la LED
Plutôt tester la branche "Restructurée"

# Projet Poêle

Ce projet contrôle un poêle à granulés via un microcontrôleur ESP8266. Il utilise MQTT pour la communication et plusieurs capteurs pour surveiller et contrôler le poêle.

## Structure du Projet

- `main.cpp` : Fichier principal contenant la logique du programme.
- `config.h` : Fichier de configuration pour les paramètres WiFi et MQTT.

## Fonctions Principales

### `setup_wifi()`

Configure et connecte le microcontrôleur au réseau WiFi en utilisant la bibliothèque ESP_WiFiManager.

### `reconnect()`

Tente de reconnecter le client MQTT si la connexion est perdue.

### `setup_mqtt()`

Configure le client MQTT et définit les fonctions de rappel pour les messages entrants.

### `checkStoveReply()`

Vérifie et traite les réponses du poêle.

### `write(uint8_t loc, uint8_t param, uint8_t data)`

Écrit une valeur dans une adresse spécifique du poêle.

### `getState(uint8_t loc, uint8_t param)`

Lit l'état d'un paramètre spécifique du poêle.

### `callback(char *topic, byte *payload, unsigned int length)`

Fonction de rappel pour traiter les messages MQTT entrants. Elle contrôle divers aspects du poêle en fonction des messages reçus.

### `blinkLedSlow()`

Fait clignoter la LED intégrée lentement.

### `blinkLED()`

Fait clignoter la LED intégrée rapidement.

### `myClickFunction()`

Fonction appelée lors de l'appui sur un bouton. Elle publie un message MQTT indiquant que le sac de granulés est vide.

### `checkDistance()`

Vérifie la distance mesurée par un capteur de distance et renvoie la valeur.

### `setup()`

Fonction d'initialisation appelée au démarrage du programme. Elle configure les broches, initialise les capteurs, et établit les connexions WiFi et MQTT.

### `loop()`

Boucle principale du programme. Elle gère les tâches récurrentes telles que la vérification des messages MQTT et le contrôle de la LED.

## Configuration

Renommez `exempleconfig.h` en `config.h` et modifiez les valeurs pour correspondre à votre configuration réseau.

```h
//Renomer en config.h

#define ssid "SSID"
#define password "PWD"
const char *mqtt_server = "192.168.xxx.xxx";
const int mqtt_port = 1883;
