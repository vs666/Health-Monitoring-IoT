#include <Wire.h>

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
String CSE_IP      = "192.168.0.107";
//String CSE_IP      = "192.168.1.egin(9600);233";
int   CSE_HTTP_PORT = 8080;
String CSE_NAME    = "in-name";
String CSE_M2M_ORIGIN  = "admin:admin";


WiFiClient client;

WebServer server(80);
IPAddress local_ip(192, 168, 0, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

int sensorValue = 0;
int gsr_average = 0;
char* hash(String inp) {
    char* payload;
    inp.toCharArray(payload, inp.length());
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

    Serial.print("Hash: ");
    String ret = "0x";
    for (int i = 0; i < sizeof(shaResult); i++) {
        char str[3];
        sprintf(str, "%02x", (int)shaResult[i]);
        ret += str;
    }
//    Serial.println(ret);
//    return ret;
    char* outt;
    ret.toCharArray(outt, ret.length());
    return outt;
}

String getCipher(String inp) {
    char* input;
    inp.toCharArray(input, inp.length());
    mbedtls_aes_context aes;

    char* key_unhashed = "abcdefghijklmnop";
    char* key = hash(key_unhashed);
    unsigned char output[16];
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_enc(&aes, (const unsigned char*)key, strlen(key) * 8);
    mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, (const unsigned char*)input, output);
    mbedtls_aes_free(&aes);

    String ret = "";
    for (int i = 0; i < 16; i++) {
        char str[3];

        sprintf(str, "%02x", (int)output[i]);
        ret += str;
    }
    return ret;
}

String getData(float temp, long gsr, float h_resist) {
    String di = "{\"temperature\":\"" + String(temp, 0) + "\",\"GSR\":" + String(gsr, 0) + "\",\"human_resistance\":" + String(h_resist, 0) + "\"}";
    return getCipher(di);
}

String getUploadData(float temp, long gsr, float h_resist) {
    String msg = getData(temp, gsr, h_resist);
    String di = "{\"message\":" + msg + ",\"hash\":\"" + hash(msg) + "\"}";
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
    WiFi.softAP(ssid_2, pswd);
    WiFi.softAPConfig(local_ip, gateway, subnet);
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
    delay(1000);
}
