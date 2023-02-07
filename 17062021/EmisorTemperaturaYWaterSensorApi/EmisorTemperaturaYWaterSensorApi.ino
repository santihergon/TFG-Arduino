// Incluyo las librerias
#include <SPI.h>
#include <LoRa.h>
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11

// Inicializamos el sensor DHT.
DHT dht(DHTPIN, DHTTYPE);

const int ldr = 33;// LDR (Sensor de luz o Fotoresistor)

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

int counter = 0;
String LoRaMessage = "";
int idEmisor = 10;

int tipoMedidaSensor4 = 0;
int tipoMedidaSensor3 = 3;
int tipoMedidaSensor2 = 2;
int tipoMedidaSensor1 = 1;

int datoSensor4 = 0;    // variable para almacenar valor leido de
int datoSensor3 = 0;    // variable para almacenar valor leido de Agua
int datoSensor2 = 0;    // variable para almacenar valor leido de Luz
int datoSensor1 = 0;    // variable para almacenar valor leido de Temperatura

const int LED = 12;  // Declaro Pin 12 de ESP32 donde está conectado el LED
// set pin numbers
const int buttonPin = 35;  // Declaro Pin 35 de ESP32 donde está conectado el LED

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);


  while (!Serial);

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  pinMode(LED, OUTPUT);

  dht.begin();

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(866E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  delay(2000); //Es un sensor lento, por lo que hay que darle tiempo. Funciona cada 1s
  float h = dht.readHumidity(); //Leo la humedad y la guardo en la variable h
  float t = dht.readTemperature(); //Leo la temperatura y la guardo en la variable t. Los valores están en Celsius de forma predeterminada pero se puden pasar a Fahrenheit

  if (isnan(h) || isnan(t)) { // Si devuelve nan aviso de que ha fallado la lectura de datos
    Serial.println(F("Fallo en la lectura de los datos del sensor DHT!"));
    return;
  }

// Imprimo los datos de humedad y temperatura en el Monitor Serie para comprobar que funciona correctamente
  Serial.print(F("Humedad: "));
  Serial.print(h);
  Serial.print(F("% Temperatura: "));
  Serial.print(t);
  Serial.println(F("°C "));

  Serial.print("Sending packet: ");
  Serial.println(counter);

  // Leo el valor del pin35 y lo guardo en la varibale datoSensor3
  datoSensor3 = analogRead(buttonPin);
  Serial.print("idEmisor: ");
  Serial.println(idEmisor);

  Serial.print("datoSensor3: ");
  Serial.println(datoSensor3);

  datoSensor1 = t;
  Serial.print("datoSensor1: ");
  Serial.println(datoSensor1);

  datoSensor2 = analogRead(ldr);

  Serial.print("datoSensor2: ");
  Serial.println(datoSensor2);

//Guardo todas los datos de los sensores en la variable LoRaMessage para poder enviarlo por LoRa
  LoRaMessage = String(idEmisor) + "/" + String(datoSensor1) + "&" + String(tipoMedidaSensor1) + "#" + String(datoSensor2) + "@" + String(tipoMedidaSensor2) + "$" + String(datoSensor3) + "^" + String(tipoMedidaSensor3);


  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();

  //Recibimos el mensaje del receptor y lo imprimimos en el Monitor Serie
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String datosLoraRecibidos = LoRa.readString();
      Serial.print(datosLoraRecibidos);
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  counter++;
  delay(10000);
}
