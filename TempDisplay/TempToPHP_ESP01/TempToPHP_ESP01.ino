/* This requires three files to be loaded on the server.
    File 1: dht11.php -- Used to do the actual inputs.
    File 2: db.php -- Used to store database login info.
    File 3: TempData.php -- The page that you view the data on
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//#include <DHT.h> //  DHT.h library
#include "DHTesp.h"
  DHTesp dht;
  
// Local Network Info
#ifndef STASSID
#define STASSID "Skynet2"
#define STAPSK  "Je$u$_S@ve$"
#define STASSID2 "Skynet"
#define STAPSK2 "ilovemywife"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;
const char *ssid2 = STASSID2;
const char *password2 = STAPSK2;

// This is where we'll take the temp and humidity readings from
#define DHTPIN 2        //Not D0 Pin it is D3 Pin, Here 0 means GPIO 0.
#define DHTTYPE  DHTesp::DHT11

// Variables
String Room = "Kitchen";
float humidity;
float temperature;
int updateSeconds = 60 / 5;
int counterTicks = 0;
int counterWifi = 0;

int networkConnect = 0;

//External server that houses the php pages (Can be ipAddress as well)
char server[] = "k-farley.com";

// Web client object
WiFiClient client;

void setup(void) {
  Serial.begin(115200);
  // New sensor object
  dht.setup(DHTPIN, DHTTYPE);
  connectWifi();
}

void connectWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    if (counterWifi == 0) {
      WiFi.mode(WIFI_STA);
      Serial.print("Connecting to:");
      if (networkConnect % 2 == 0) {
        WiFi.begin(ssid, password);
        Serial.println(ssid);
      } else {
        WiFi.begin(ssid2, password2);
        Serial.println(ssid2);
      }
    }
    counterWifi++;
    delay(500);
    Serial.print(counterWifi);
    Serial.print(".");

    if (counterWifi > 30)
    {
      Serial.println(".");
      Serial.println("Wifi not connecting. Pausing 30 Seconds");
      delay(30000);
      counterWifi = 0;
      networkConnect += 1;
      WiFi.mode(WIFI_OFF);
    }
  }
  counterWifi = 0;

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
{
  if (isnan(humidity) || isnan(temperature) || humidity > 100) {
    Serial.print("Bad Data: ");
    Serial.print(temperature);
    Serial.print("F, ");
    Serial.print(humidity);
    Serial.println("%");
    return;
  }
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.print("GET /dht11.php?humidity=");
    client.print("GET /dht11.php?humidity=");
    Serial.print(humidity);
    client.print(humidity);
    client.print("&Room=");
    Serial.print("&Room=");
    Serial.print(Room);
    client.print(Room);
    client.print("&temperature=");
    Serial.print("&temperature=");
    client.print(temperature);
    Serial.print(temperature);
    client.print("&ipAddress=");
    Serial.print("&ipAddress=");
    client.print(WiFi.localIP());
    Serial.print(WiFi.localIP());
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    Serial.print(" ");
    client.println("HTTP/1.1");
    Serial.println("HTTP/1.1");
    Serial.println("Host:k-farley.com");
    client.println("Host:k-farley.com");
    Serial.println("Connection: close");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
    connectWifi();
  }
}


void loop(void) {

  counterTicks++;
  if (counterTicks >= updateSeconds) {
    Serial.write("Updating...");
    Sending_To_phpmyadmindatabase();
    counterTicks = 0;
  }
  humidity = dht.getHumidity();
  temperature = dht.toFahrenheit(dht.getTemperature());

  Serial.print(temperature);
  Serial.print("F, ");
  Serial.print(humidity);
  Serial.println("%");
  delay(5000);

}
