#include <SPI.h>
#include <LoRa.h>

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

String id;
String accion;
String cantidad;

const int LED = 12;  // Declaro Pin 12 de ESP32 donde está conectado el LED
// set pin numbers
const int buttonPin = 35;  // the number of the pushbutton pin

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  pinMode(LED, OUTPUT);

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
  
//  digitalWrite(LED, HIGH);
//  delay(1000);
//  digitalWrite(LED, LOW);
//  delay(1000);

  Serial.print("Sending packet: ");
  Serial.println(counter);

  // read the state of the pushbutton value
  //  datoSensor3 = digitalRead(buttonPin);
  datoSensor3 = analogRead(buttonPin);
  Serial.print("idEmisor: ");
  Serial.println(idEmisor);

  Serial.print("Sending datoSensor3: ");
  Serial.println(datoSensor3);


  // LoRaMessage = String(id) + "/" + String(temperatura) + "&" + String(luz) + "#" + String(humedad);

  LoRaMessage = String(idEmisor) + "/" + String(datoSensor3) + "&" + String(tipoMedidaSensor3);


  //Send LoRa packet to receiver
  LoRa.beginPacket();

  LoRa.print(LoRaMessage);

  LoRa.endPacket();

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData);

      int pos1 = LoRaData.indexOf('/');
      int pos2 = LoRaData.indexOf('&');
      // int pos3 = LoRaData.indexOf('#');

      id = LoRaData.substring(0, pos1);
      accion = LoRaData.substring(pos1 + 1, pos2);
      // datoSensor3 = LoRaData.substring(pos1 + 1, LoRaData.length());
      cantidad = LoRaData.substring(pos2 + 1, LoRaData.length());
      
      // AQUI LO QUE HAGO ES RECOGER ACCCION Y SI ES 100 ENCIENDO EL LED. 
      //EL PROBLEMA ESTÁ EN QUE EL EMISOR NO ESTA RECIBIENDO BIEN EL MENSAJE DEL RECEPTOR 
      //O QUE EL EMISOR NO ESTA IMPRIMIENDO BIEN EL MENSAJA DEL RECEPTOR

 if (accion == "100"){
  //Enciende la luz
    digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
  }
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  counter++;
  delay(10000);
 // delay(2000);
}
