
#include <LoRa.h>
#include <WiFi.h>
#include "WiFi.h"

/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

#include <SPI.h>


//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

const char* wifi_ssid = "MiWifi";
const char* wifi_password = "942543459";
unsigned long previousMillis = 0;
unsigned long interval = 30000;
const char* host = "192.168.1.111";
int numero = 0;
int counter = 0;
String loRaMessage;
// String LoRaData;

String datoSensor3;
String datoSensor2;
String datoSensor1;
String datoSensor4;

String idEmisor;

String tipoMedidaSensor3;
String tipoMedidaSensor1;
String tipoMedidaSensor2;
String tipoMedidaSensor4;

void setup() {

  conectarWifi();//Llamamos a la función conectarWifi()

  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

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

  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    // WiFi.reconnect();
    WiFi.begin(wifi_ssid, wifi_password);
    previousMillis = currentMillis;
  }

  // try to parse packet
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    Serial.print("PacketSize: ");
    Serial.println(packetSize);
    // received a packet
    Serial.print("Received packet '");

    // String datosLoraRecibidos = "estonorecibionada";

    // read packet
    while (LoRa.available()) {
     String datosLoraRecibidos = LoRa.readString();
      Serial.print(datosLoraRecibidos);

      // Get readingID, temperature and soil moisture
      int pos1 = datosLoraRecibidos.indexOf('/');
      int pos2 = datosLoraRecibidos.indexOf('&');
      int pos3 = datosLoraRecibidos.indexOf('#');
      int pos4 = datosLoraRecibidos.indexOf('@');
      int pos5 = datosLoraRecibidos.indexOf('$');
      int pos6 = datosLoraRecibidos.indexOf('^');

      //    temperature = datosLoraRecibidos.substring(pos1 +1, pos2);
      //    humidity = datosLoraRecibidos.substring(pos2+1, pos3);
      //    pressure = datosLoraRecibidos.substring(pos3+1, datosLoraRecibidos.length());



      // LoRaData = datosLoraRecibidos.substring(0, pos1);
      idEmisor = datosLoraRecibidos.substring(0, pos1);
      datoSensor1 = datosLoraRecibidos.substring(pos1 + 1, pos2);
      tipoMedidaSensor1 = datosLoraRecibidos.substring(pos2 + 1, pos3);
      datoSensor2 = datosLoraRecibidos.substring(pos3 + 1, pos4);
      tipoMedidaSensor2 = datosLoraRecibidos.substring(pos4 + 1, pos5);
      datoSensor3 = datosLoraRecibidos.substring(pos5 + 1, pos6);
      tipoMedidaSensor3 = datosLoraRecibidos.substring(pos6 + 1, datosLoraRecibidos.length());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    Serial.print("Sending packet: ");
    Serial.println(counter);

    //Send LoRa packet to receiver
    LoRa.beginPacket();
    LoRa.print("hello ");
    LoRa.print(counter);
    LoRa.endPacket();

    counter++;

    delay(2000);

    // String parametros = "&LoRaData=" + LoRaData + "&datoSensor3=" + datoSensor3;
    String parametros = "&idEmisor=" + idEmisor + "&datoSensor1=" + datoSensor1 + "&tipoMedidaSensor1=" + tipoMedidaSensor1 + + "&datoSensor2=" + datoSensor2 + "&tipoMedidaSensor2=" + tipoMedidaSensor2 + "&datoSensor3=" + datoSensor3 + "&tipoMedidaSensor3=" + tipoMedidaSensor3 + "&datoSensor4=" + datoSensor4 + "&tipoMedidaSensor4=" + tipoMedidaSensor4;

    enviarPOST(parametros);
  }
}

void enviarPOST(String parametros)
{
  String url = "/LaGranjaPruebaPHP/Servidor/prueba_arduino.php";
  Serial.print("Conectando a ");
  Serial.println("127.0.0.1");

  WiFiClient client;
  const int httpPort = 8080; //8080
  // strhost.toCharArray(host, 49);
  if (!client.connect(host, httpPort))
  {
    Serial.println("conexión fallida");
    return;
  }

  Serial.print("Requesting URL: ");
  Serial.print(url);

  //  client.print(String("POST ") + url + " HTTP/1.1" + "\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  client.println("POST " + url + " HTTP/1.1");
  client.println("Host: 192.168.1.111");     // + host);
  client.println("Cache-Control: no-cache");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(parametros.length());
  client.println();
  client.println(parametros);
  // Ejemplo línea de parámetros:
  // LoRaData=123&idDispositivo=23&idMedida=34&nomnbre=Antonio

  unsigned long timeout = millis();
  while (client.available() == 0)
  {
    if (millis() - timeout > 5000)
    {
      Serial.println(">>>Client Timeout !");
      client.stop();
      return;
    }
  }
  while (client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("Cerrando conexion");
}
//Estas son las funciones que se utilizan
void conectarWifi()
{
  Serial.begin(115200);
  Serial.println();
  while (!Serial);
  Serial.println("Prueba de POST contra el servidor desde Arduino");
  Serial.print("Conectando a ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
  }

  Serial.println();
  Serial.println("Conectado al WiFi");
  Serial.println("Direccion IP:  ");
  Serial.println(WiFi.localIP());
}
