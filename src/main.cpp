#include <Arduino.h>

#include <Wire.h>
#define SDA D7  //(A vérifier) (INPUT_PULLUP)
#define SCL D8  //(INPUT_PULLUP)

#include <VL53L0X.h>
VL53L0X sensor;

#include <SoftwareSerial.h>
SoftwareSerial StoveSerial;
#define SERIAL_MODE SWSERIAL_8N2 // 8 data bits, parity none, 2 stop bits
#define ENABLE_RX D1
#define RX_PIN D5
#define TX_PIN D6

//Wifi
#include <ESP8266WiFi.h>
#define ssid "HibouHome"
#define password "0630232210"
WiFiClient espPoele;

//MQTT
#include <PubSubClient.h>
PubSubClient client(espPoele);
const char *mqtt_server = "192.168.0.42";
const int mqtt_port = 1883;
const uint8_t delayPublish = 200;

//GPIO
#define THERMPIN D2
#define BOUTON D3
#define RESETPIN D0

//GetState
const uint16_t delayRefresh = 60000;
uint32_t previousMillis;

// CheckDistance
const uint32_t delayCheckTank = 300000; // 5 min
const uint32_t delayCheckTankoff = 3600000; // 1h
uint32_t previousCheck;
uint16_t distance;

//Blink LED
const uint8_t NbchangementEtat = 10;
uint8_t changementEtat = 0;
uint8_t lastEtat = HIGH;
uint8_t demandeBlinkLED = 0;
const uint8_t blinkTime = 200;

//Bouton
#define antiRebond 10
uint32_t lastappui, now;
uint32_t lastChange = 0;
uint8_t lastEtatbouton = 1;
uint8_t Etatbouton;

//DeepSleep
uint8_t deepSleep = 0;
uint16_t delayDeepSleep = 60000;

//Topic
#define mqtt_topic "Poele"

#define pong_topic mqtt_topic "/pong"
#define state_topic mqtt_topic "/state"
#define onoff_topic mqtt_topic "/onoff"
#define ambtemp_topic mqtt_topic "/ambtemp"
#define fumetemp_topic mqtt_topic "/fumetemp"
#define flame_topic mqtt_topic "/flamepower"
#define fumesRPM_topic mqtt_topic "/fumesRPM"
#define ventPower_topic mqtt_topic "/ventPower"
#define cyclePellet_topic mqtt_topic "/cyclePellet"
#define pellet_topic mqtt_topic "/pellet"
#define in_topic mqtt_topic "/intopic"
#define distance_topic mqtt_topic "/distance"
#define conso_topic mqtt_topic "/conso"
#define puissance_topic mqtt_topic "/puissanceChauffe"
#define plein_topic mqtt_topic "/pleinfait"

// 0 - OFF, 1 - Starting, 2 - Pellet loading, 3 - Ignition, 4 - Work, 5 - Brazier cleaning, 6 - Final cleaning, 7 - Standby, 8 - Pellet missing alarm, 9 - Ignition failure alarm, 10 - Alarms (to be investigated)
 
// Checksum: Code+Address+Value on hexadecimal calculator

//Adresses
#define ReadRAM 0x00
#define ReadROM 0x20
#define WriteRAM 0x80
#define WriteROM 0xA0
#define stoveStateAddr 0x21
#define fumesRPMAddr 0x37
#define cyclePelletAddr 0x0C
#define pelletAddr 0x0D
#define ambTempAddr 0x01
#define fumesTempAddr 0x65
#define ventPowerAddr 0x7E  // ROM
#define flamePowerAddr 0x7F // ROM

const uint8_t grPelletMin = 47; //  debit vis sans fin poele en gr/min

uint8_t stoveState, fumesTemp, ventPower, flamePower, locCheksum;
uint16_t fumesRPM;
float ambTemp, cyclePellet, pellet, consoPellet, puissanceChauffe;
char stoveRxData[2]; // When the heater is sending data, it sends two bytes: a checksum and the value

void setup_wifi() // Setup WiFiManager and connect to WiFi
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    if (millis() > 15000)
    {
      Serial.println("Wifi HS Deep Sleep");
      ESP.deepSleepInstant(delayDeepSleep);
    }
  }
}

void reconnect()
{
  while (!client.connected())
  {
    if (client.connect("espPoele"))
    {
      delay(100);
      if (millis() > 15000)
      {
        Serial.println("MQTT HS Deep Sleep");
        ESP.deepSleepInstant(delayDeepSleep);
      }
    }
  }
}

void setup_mqtt()
{
  client.setServer(mqtt_server, mqtt_port);
  reconnect();
}

void checkStoveReply()
{
  digitalWrite(ENABLE_RX, HIGH);
  delay(80);
  uint8_t rxCount = 0;
  stoveRxData[0] = 0x00;
  stoveRxData[1] = 0x00;
  while (StoveSerial.available()) // It has to be exactly 2 bytes, otherwise it's an error
  {
    stoveRxData[rxCount] = StoveSerial.read();
    rxCount++;
  }
  digitalWrite(ENABLE_RX, LOW);
  if (rxCount == 2)
  {
    uint8_t val = stoveRxData[1];
    uint8_t checksum = stoveRxData[0];
    uint8_t param = checksum - val - locCheksum;
    Serial.printf("Param=%01x value=%01x ", param, val);
    switch (param)
    {
    case stoveStateAddr:
      stoveState = val;
      switch (stoveState)
      {
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
      Serial.printf("Stove %s\n", stoveState ? "ON" : "OFF");
      break;
    case ambTempAddr:
      ambTemp = (float)val / 2;
      client.publish(ambtemp_topic, String(ambTemp).c_str(), 0);
      Serial.print("T. amb. ");
      Serial.println(ambTemp);
      break;
    case fumesTempAddr:
      fumesTemp = val;
      client.publish(fumetemp_topic, String(fumesTemp).c_str(), 0);
      Serial.printf("T. fumes %d\n", fumesTemp);
      break;
    case flamePowerAddr:
      flamePower = val;
      client.publish(flame_topic, String(flamePower).c_str(), 0);
      Serial.printf("Fire %d\n", flamePower);
      break;
    case fumesRPMAddr:
      fumesRPM = val * 10 + 250;
      if (fumesRPM <= 250)
      {
        fumesRPM = 0;
      }
      client.publish(fumesRPM_topic, String(fumesRPM).c_str(), 0);
      Serial.printf("fumesRPM %d\n", fumesRPM);
      break;
    case ventPowerAddr:
      ventPower = val;
      client.publish(ventPower_topic, String(ventPower).c_str(), 0);
      Serial.printf("ventPower %d\n", ventPower);
      break;
    case cyclePelletAddr:
      cyclePellet = (float)val / 10;
      client.publish(cyclePellet_topic, String(cyclePellet).c_str(), 0);
      Serial.print("cyclePellet ");
      Serial.println(cyclePellet);
      break;
    case pelletAddr:
      pellet = (float)val / 10;
      client.publish(pellet_topic, String(pellet).c_str(), 0);
      Serial.print("pellet ");
      Serial.println(pellet);
      consoPellet = pellet / cyclePellet * grPelletMin; // Conso en gr/min
      client.publish(conso_topic, String(consoPellet).c_str(), 0);
      Serial.print("consoPellet ");
      Serial.println(consoPellet);
      puissanceChauffe = consoPellet * 60 * 5; // Puissance en W/h
      client.publish(puissance_topic, String(puissanceChauffe).c_str(), 0);
      Serial.print("puissanceChauffe ");
      Serial.println(puissanceChauffe);
      break;
    }
  }
}

void write(uint8_t loc, uint8_t param, uint8_t data)
{
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

void getState(uint8_t loc, uint8_t param)
{
  locCheksum = loc;
  StoveSerial.write(loc);
  delay(1);
  StoveSerial.write(param);
  checkStoveReply();
  locCheksum = 0;
}

void getStates() // Calls all the get…() functions
{
  getState(ReadRAM, stoveStateAddr);
  getState(ReadRAM, fumesRPMAddr);
  getState(ReadRAM, cyclePelletAddr);
  getState(ReadRAM, pelletAddr);
  getState(ReadRAM, ambTempAddr);
  getState(ReadRAM, fumesTempAddr);
  getState(ReadROM, ventPowerAddr);
  getState(ReadROM, flamePowerAddr);
}

void callback(char *topic, byte *payload, unsigned int length) // Envoi de X chiffres,1er Toujours 1, 2ème On/Off (1;0);3ème  puissance (1;2;3;4;5); 4ème Pventil (1;2;3;4.5); 5ème Reset (1;0)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (uint8_t i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if ((char)payload[1] == '1') // Remplacer [X] par la position du caractère dans le payload et "1" par le message envoyé par jeedom
  {
    digitalWrite(THERMPIN, LOW);
    if ((char)payload[2] != '9')
    {
      byte puissanceCall;
      if ((char)payload[2] == '1')
      {
        puissanceCall = 1;
      }
      else if ((char)payload[2] == '2')
      {
        puissanceCall = 2;
      }
      else if ((char)payload[2] == '3')
      {
        puissanceCall = 3;
      }
      else if ((char)payload[2] == '4')
      {
        puissanceCall = 4;
      }
      else if ((char)payload[2] == '5')
      {
        puissanceCall = 5;
      }
      else
      {
        puissanceCall = flamePower;
      }
      if (puissanceCall != flamePower)
      {
        write(WriteROM, flamePowerAddr, puissanceCall);
        getState(ReadROM, flamePowerAddr);
      }
    }
    if ((char)payload[3] != '9')
    {
      byte ventPowerCall;
      if ((char)payload[3] == '1')
      {
        ventPowerCall = 1;
      }
      else if ((char)payload[3] == '2')
      {
        ventPowerCall = 2;
      }
      else if ((char)payload[3] == '3')
      {
        ventPowerCall = 3;
      }
      else if ((char)payload[3] == '4')
      {
        ventPowerCall = 4;
      }
      else if ((char)payload[3] == '5')
      {
        ventPowerCall = 5;
      }
      else
      {
        ventPowerCall = ventPower;
      }
      if (ventPowerCall != ventPower)
      {
        write(WriteROM, ventPowerAddr, ventPowerCall); // Ecrire puissance flame}
        getState(ReadROM, ventPowerAddr);
      }
    }
  }
  else if ((char)payload[1] == '0')
  {
    digitalWrite(THERMPIN, HIGH);
  }
  if ((char)payload[4] == '1')
  {
    digitalWrite(RESETPIN, LOW);
  }
  if ((char)payload[5] == '1')
  {
    demandeBlinkLED = 1;  
  }
  else if ((char)payload[5] == '2')
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if ((char)payload[5] == '0')
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void setup()
{
  pinMode(ENABLE_RX, OUTPUT);
  digitalWrite(ENABLE_RX, LOW); // The led of the optocoupler is off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(RESETPIN, OUTPUT);
  digitalWrite(RESETPIN, HIGH);
  pinMode(THERMPIN, OUTPUT);
  digitalWrite(THERMPIN, HIGH);
  pinMode(BOUTON, INPUT);
  Wire.begin(SDA, SCL);
  sensor.setTimeout(500);
  Serial.begin(115200);
  StoveSerial.begin(1200, SERIAL_MODE, RX_PIN, TX_PIN, false, 256);
  setup_wifi();
  setup_mqtt();
  client.setCallback(callback);
  client.subscribe(in_topic);
}

void loop()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    if (millis() > 15000)
    {
      Serial.println("Wifi HS Deep Sleep");
      ESP.deepSleepInstant(delayDeepSleep);
    }
  }
  
  if (!client.connected())
  {
    reconnect();
    client.subscribe(in_topic);
  }
  client.loop();

  uint32_t currentMillis = millis();
  if (currentMillis - previousMillis >= delayRefresh)
  {
    previousMillis = currentMillis;
    client.publish(pong_topic, "Connected");
    getStates();
  }

  if(pellet > 0)
  {
    if (currentMillis - previousCheck >= delayCheckTank)
    {
      distance = sensor.readRangeSingleMillimeters();
      client.publish(distance_topic, String(distance).c_str(), 0);
    }
  }
  else
  {
    if (currentMillis - previousCheck >= delayCheckTankoff)
    {
      distance = sensor.readRangeSingleMillimeters();
      client.publish(distance_topic, String(distance).c_str(), 0);
    }
  }
  
  now = millis();
  Etatbouton = digitalRead(BOUTON);
  if(Etatbouton != lastEtatbouton && now - lastappui >= antiRebond)
  {
    lastappui = now;
    lastEtatbouton= Etatbouton;
    if(Etatbouton == 0)
    {
      client.publish(plein_topic, "Plein fait");
      demandeBlinkLED = 1;
    }
  }

  if (demandeBlinkLED == 1)
  {
    if (changementEtat >= NbchangementEtat)
    {
      demandeBlinkLED = 0;
      changementEtat = 0;
      digitalWrite(LED_BUILTIN, HIGH);
      lastEtat = LOW;
      lastChange = millis();
    }
    else if (millis() - lastChange >= blinkTime)
    {
      lastChange = millis();
      changementEtat++;
      digitalWrite(LED_BUILTIN, !lastEtat);
      lastEtat = !lastEtat;
    }
  }

  if (deepSleep == 1) // Does not work without hardaware modification (a cable must be connected between RST and D0)
  {
    Serial.println("Deep Sleep");
    ESP.deepSleepInstant(delayDeepSleep);
  }
}
