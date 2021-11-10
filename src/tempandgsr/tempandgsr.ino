#include <Wire.h>

#include "Protocentral_MAX30205.h"
MAX30205 tempSensor;
const int GSR = 34;

int sensorValue = 0;

int gsr_average = 0;
void setup() {
    Serial.begin(9600);
    Wire.begin();
    analogReadResolution(GSR);  //12 bits
    analogSetPinAttenuation(GSR, ADC_11db);
    //scan for temperature in every 30 sec untill a sensor is found. Scan for both addresses 0x48 and 0x49
    while (!tempSensor.scanAvailableSensors()) {
        Serial.println("Couldn't find the temperature sensor, please connect the sensor.");
        delay(3000);
    }
    tempSensor.begin();  // set continuos mode, active mode
    Serial.println("temperature sensor began");
}

void loop() {
    float temp = tempSensor.getTemperature();  // read temperature for every 100ms
                                               //  Serial.print(temp ,2);
                                               //  Serial.print("'c  " );
    temp = (temp * 1.8) + 32;
    Serial.print("Temperature = ");
    Serial.print(temp, 2);
    Serial.print("'f , ");

    long sum = 0;
    for (int i = 0; i < 10; i++)  //Average the 10 measurements to remove the glitch
    {
        sensorValue = analogRead(GSR);
        sum += sensorValue;
        delay(5);
    }
    gsr_average = sum / 10;
    Serial.print("gsr_average = ");
    Serial.print(gsr_average);
    int human_resistance = ((4095.0 + 2.0 * gsr_average) * 10000.0) / (1000.0 - gsr_average);
    Serial.print(" , human_resistance = ");
    Serial.println(human_resistance / 100000.0);
    delay(1000);
}
