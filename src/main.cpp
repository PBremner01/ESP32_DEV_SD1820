#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <WiFi.h>
#include <WebServer.h>
#include "index.html"

#define ssid "HomeSAP"
#define password "!234567$"

IPAddress local_ip(192,168,101,2);
IPAddress gateway(192,168,101,2);
IPAddress subnet(255,255,255,0);
WebServer server(80);

/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
  Based on the Dallas Temperature Library example
*********/

// GPIO where the DS18B20 is connected to
#define ONE_WIRE_BUS 4 

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
float fTemp = 0.0;

String SendHTML(float fTemp){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +=" <meta http-equiv=\"refresh\" content=\"5\">\n";
  ptr +="<title>Pool SAP</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Pool Standalone WiFi</h1>\n";
  ptr +="<p>Temperature: ";
  ptr += fTemp;
  ptr +=" F </p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}


void handle_OnConnect(){
	Serial.println("handle_OnConnect.");
	server.send(200, "text/html", SendHTML(fTemp));
}

void handle_tempUpdate(){

	server.send(200, "text/html", SendHTML(fTemp));

}

void handle_NotFound() {
	server.send(404, "text/plain", "Not Found");
}


void setup() {
  // // Start the Serial Monitor
  // Serial.begin(115200);
  // // Start the DS18B20 sensor
  // sensors.begin();
    //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial);
  // Start up the library DallasTemperature 
  sensors.begin(); 


	WiFi.softAP(ssid, password);
	WiFi.softAPConfig(local_ip, gateway, subnet);
	delay(1000);

	server.on("/", handle_OnConnect);
	// server.on("/tempUpdate", handle_tempUpdate(fTemp));
	server.onNotFound(handle_NotFound);
	server.begin();
	Serial.println("HTTP Server Started.");   
  server.handleClient();
}

void loop() {
// Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  Serial.print(" - Fahrenheit temperature: ");
  fTemp = sensors.getTempFByIndex(0);
  Serial.println(fTemp);
  delay(1000);

  server.handleClient();

  handle_tempUpdate();
}