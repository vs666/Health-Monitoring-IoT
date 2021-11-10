#include <Wire.h>

#include <WiFiClientSecure.h>
#include "Protocentral_MAX30205.h"
#include "WebServer.h"
#include "WiFi.h"
#include "mbedtls/aes.h"
#include "mbedtls/md.h"
#include <JSONVar.h>
#include <Arduino_JSON.h>
#include <JSON.h>

MAX30205 tempSensor;

const int GSR = 34;
const char* ssid = "harshaupper";
const char* ssid_2 = "harshaupper";
const char* pswd = "Harsak@2002";

// oneM2M : CSE params
const char* CSE_IP      = "esw-onem2m.iiit.ac.in";
//String CSE_IP      = "192.168.1.egin(9600);233";
int   CSE_HTTP_PORT = 443;
String CSE_NAME    = "~/in-cse/in-name";
String CSE_M2M_ORIGIN  = "3ULsBQex0w:0z8Uh6re@6";



String DESC_CNT_NAME = "DESCRIPTOR";
String DATA_CNT_NAME = "DATA";
String CMND_CNT_NAME = "COMMAND";
int TY_AE  = 2;   
int TY_CNT = 3; 
int TY_CI  = 4;
int TY_GRP = 9;
int TY_SUB = 23;

// HTTP constants
int LOCAL_PORT = 9999;
char* HTTP_CREATED = "HTTP/1.1 201 Created";
char* HTTP_OK    = "HTTP/1.1 200 OK\r\n";
int REQUEST_TIME_OUT = 5000; //ms

WebServer server(80);
IPAddress local_ip(192, 168, 0, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

int sensorValue = 0;
int gsr_average = 0;

String doPOST(String url, int ty, String rep) {

  String postRequest = String() + "POST " + url + " HTTP/1.1\r\n" +
                       "Host: " + CSE_IP + ":" + CSE_HTTP_PORT + "\r\n" +
                       "X-M2M-Origin: " + CSE_M2M_ORIGIN + "\r\n" +
                       "Content-Type: application/json;ty=" + ty + "\r\n" +
                       "Content-Length: " + rep.length() + "\r\n"
                       "Connection: close\r\n\n" +
                       rep;

  // Connect to the CSE address
  Serial.println(postRequest);
  Serial.println("connecting to push" );

  // Get a client
  WiFiClientSecure Secureclient;
  Secureclient.setInsecure();
   delay(1000);
  if (!Secureclient.connect(CSE_IP, CSE_HTTP_PORT)) {
    Serial.println("Connection failed !");
    return "error";
  }

  // if connection succeeds, we show the request to be send
#ifdef DEBUG
  Serial.println(postRequest);
#endif

  // Send the HTTP POST request
  Secureclient.print(postRequest);
  
  // Manage a timeout
  unsigned long startTime = millis();
  while (Secureclient.available() == 0) {
    if (millis() - startTime > REQUEST_TIME_OUT) {
      Serial.println("Client Timeout");
      Secureclient.stop();
      return "error";
    }
  }

  // If success, Read the HTTP response
  String result = "";
  if (Secureclient.available()) {
    result = Secureclient.readStringUntil('\r');
   Serial.println(result);
  }
  while (Secureclient.available()) {
    String line = Secureclient.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection...");
  return result;
}

// Method for creating an ContentInstance(CI) resource on the remote CSE under a specific CNT (this is done by sending a POST request)
// param : ae --> the targted AE name (should be unique under the remote CSE)
// param : cnt  --> the targeted CNT name (should be unique under this AE)
// param : ciContent --> the CI content (not the name, we don't give a name for ContentInstances)
String createCI(String ae, String cnt, String ciContent) {
  String ciRepresentation =
    "{\"m2m:cin\": {"
    "\"con\":\"" + ciContent + "\""
    "}}";
  return doPOST("/" + CSE_NAME + "/" + ae + "/" + cnt, TY_CI, ciRepresentation);
}


String hash(String inp) {
    char payload[inp.length()];
    inp.toCharArray(payload, inp.length()+1);
    byte shaResult[32];
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

    const size_t payloadLength = strlen(payload);

    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
    mbedtls_md_starts(&ctx);
    mbedtls_md_update(&ctx, (const unsigned char*)payload, payloadLength);
    mbedtls_md_finish(&ctx, shaResult);
    mbedtls_md_free(&ctx);

    String ret = "";
    for (int i = 0; i < sizeof(shaResult); i++) {
        char str[3];
        sprintf(str, "%02x", (int)shaResult[i]);
        ret += str;
    }
    
    return ret;

}

String getCipher(String inp) {
  String key = "password";
  int x = 0;
  int y = 0;
  String ph = "";
  String ret = "";
  while(x < inp.length()){
    if(y == 0){
      y = 256;
      ph=ph+key;
      ph = hash(ph);
    }
    char elt = (inp[x]^ph[256 - y]);
            char str[3];
        sprintf(str, "%02x", (int)elt);
        ret += str;

    x++;
    y--;
  }
  return ret;

}

String encryptData(float temp, long gsr, float h_resist,int sys,int dia,int hr,float spo2) {

    String di = "{\"temp\":\"" + String(temp) + "\",\"gsr\":\"" + String(gsr) + "\",\"hr\":\"" + String(h_resist) + "\",\"bpsys\":\"" + String(sys) + "\",\"bpdia\":\"" + String(dia) + "\",\"hrate\":\"" + String(hr) + "\",\"spo2\":\"" + String(spo2) + "\"}";

    return getCipher(di);

}

String uploadData(float temp, long gsr, float h_resist,int sys,int dia,int hr,float spo2) {
    String msg = encryptData(temp, gsr, h_resist,sys,dia, hr,spo2);
    Serial.println(msg);
    //msg="Trinadh";
    String hashed_msg="waste";
    hashed_msg=hash(msg);
    
    String di = "{'message':'" + msg + "','hash':'" + hashed_msg + "'}";
    Serial.println(di);

    createCI("Team-28", "Node-1/Data", di);
    return di;
}

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
    String information = "<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC\" crossorigin=\"anonymous\"><title>Health-Monitoring</title></head><body><nav class=\"navbar navbar-light bg-light\"><div class=\"container-fluid\"><a class=\"navbar-brand\" href=\"#\"><img src=\"https://drive.google.com/file/d/1lg3nc4N_UqkM0M9FkM11XHxMeThnp_Fy/view?usp=sharing\" alt=\"\" width=\"50\" height=\"50\" class=\"d-inline-block align-text-top\"> Health Monitoring Device</a></div></nav>";
    information +="<div style=\"margin-left: 15%; margin-right: 15%;\"><h3>Temperature :";
    information += String(getTemp(),1);
    information += "&deg;F</h3><div class=\"progress\"><div class=\"progress-bar\" role=\"progressbar\" style=\"width: 25%\" aria-valuenow=\"";
    information += String(getTemp(),1);
    information += "\" aria-valuemin=\"0\" aria-valuemax=\"100\"></div></div></div><div style=\"margin-left: 15%; margin-right: 15%; margin-top: 3vh;\"><h3>GSR :";
    information += String(getGsrAverage(),0);
    information += "</h3></div><script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM\" crossorigin=\"anonymous\"></script></body></html>";
    /**
   * Enter the body of html page here
   */
    information += "</body>\n</html>\n";

    server.send(200, "text/html", information);
}

float getTemp() {
    return tempSensor.getTemperature();
}

long getGsrAverage() {
    long sum = 0;
    for (int i = 0; i < 10; i++)  //Average the 10 measurements to remove the glitch
    {
        sensorValue = analogRead(GSR);
        sum += sensorValue;
        delay(5);
    }
    gsr_average = sum / 10;
    return gsr_average;
}


void setup() {
    Serial.begin(115200);

    /**
   * WIFI MODULE CODE 
   */
    connect_wifi();
    /**
   * Not sure what is the use of making 2 wifi APs. 
   */
//    WiFi.softAP(ssid_2, pswd);
//    WiFi.softAPConfig(local_ip, gateway, subnet);
    server.on("/", handleRoot);
    server.on("/information", handleInformation);
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
    Wire.begin();
    
    analogReadResolution(GSR);  //12 bits
    analogSetPinAttenuation(GSR, ADC_11db);
    //scan for temperature in every 30 sec untill a sensor is found. Scan for both addresses 0x48 and 0x49
    while (!tempSensor.scanAvailableSensors()) {
        Serial.println("Couldn't find the temperature sensor, please connect the sensor.");
        delay(3000);
    }
    tempSensor.begin();  // set continuos mode, active mode
    Serial.println("temperature sensor active");
    
    
//     int r=0;
//  while ((!Secureclient.connect(CSE_IP, CSE_HTTP_PORT))&& (r<30)) {
//    Serial.println("--");
//    delay(100);
//    r++;
//  }
//  if(r==30) {
//    Serial.println("Connection failed");
//  }
//  else {
//    Serial.println("Connected to web");
//  }
//  createCI("Team-28", "Node-1/Data", "Trinadh the waste");
}

void loop() {
    float temp = getTemp();  // read temperature for every 100ms
    temp = (temp * 1.8) + 32;
    Serial.print("Temperature = ");
    Serial.print(temp, 2);
    
    Serial.print("°f , ");
    gsr_average = getGsrAverage();
    Serial.print("gsr_average = ");
    Serial.print(gsr_average);
    int human_resistance = ((4095.0 + 2.0 * gsr_average) * 10000.0) / (1000.0 - gsr_average);
    Serial.print(" , human_resistance = ");
    Serial.println(human_resistance / 100000.0);
    uploadData(temp,gsr_average, (human_resistance / 100000.0) ,121,78,78,96.00);
    delay(2000);
}
