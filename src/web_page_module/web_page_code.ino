#include "WiFi.h"
#include "DHTesp.h"
#include "ThingSpeak.h"
#include "esp_adc_cal.h"
#include "WebServer.h"


#define DEBUG 1
int MODE = 0;

DHTesp dht;

const char* ssid = "TP-Link_6B5E";
const char* ssid_2 = "Kronos";
const char* pswd = "---ENTER-PASSWORD-HERE---";

WiFiClient client;

WebServer server(80);
IPAddress local_ip(192, 168, 0, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void connect_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pswd);
  Serial.print("Connecting to wifi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);

  // connection established
}

void handleRoot() {
  Serial.println("DISPLAYING ROOT PAGE");
  server.send(200, "text/html", "<h1>Hello World!!</h1>");
}

void handleInformation() {
  String information="<!DOCTYPE html><html>\n";
  information +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  information +="<title>Website title</title>\n";
  information +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  information +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  information +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  information +=".button-on {background-color: #3498db;}\n";
  information +=".button-on:active {background-color: #2980b9;}\n";
  information +=".button-off {background-color: #34495e;}\n";
  information +=".button-off:active {background-color: #2c3e50;}\n";
  information +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  information +="</style>\n";
  information +="</head>\n";
  information +="<body>\n";
  /**
   * Enter the body of html page here
   */
  information += "</body>\n</html>\n";

  server.send(200,"text/html",information); 
}

void setup() {
  Serial.begin(115200);

  connect_wifi();
  /**
   * Not sure what is the use of making 2 wifi APs. 
   */
  WiFi.softAP(ssid_2, pswd);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  server.on("/", handleRoot);
  server.on("/information",handleInformation);
  /**
   * APIs :
   *  
   * 1. showVitals
   * 2. showRecord/History/Graph
   * 3. showSettings (??)
   * 4. showAnalytics
   * 
   */
  server.begin();
}

void loop() {
}
