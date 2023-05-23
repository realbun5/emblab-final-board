#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Network.h"

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <SoftwareSerial.h>
#include <string>

Network *network;
unsigned long dataMillis = 0;

WiFiUDP ntpUp;
NTPClient timeClient(ntpUp, "pool.ntp.org");

EspSoftwareSerial::UART DataSerial;

String Data[4]={"0","0","0","0"};// humid, lightLevel, soilHumid, temp

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  DataSerial.begin(115200, EspSoftwareSerial::SWSERIAL_8N1, D7, D8, false, 100, 100);
  initNetwork();
}

void loop() {
  // put your main code here, to run repeatedly:
  Update_Data();
  if (Firebase.ready() && (millis() - dataMillis > 600000 || dataMillis == 0)){
    dataMillis = millis();
    timeClient.update();
    unsigned long now = timeClient.getEpochTime();
    network->firestoreDataUpdate(Data[0], Data[1], Data[2], Data[3], now);
  }
}

void initNetwork(){
  network = new Network();
  network->initWiFi();
  network->firebaseInit();
}

void Update_Data(){
  if(DataSerial.available() > 0){
    Data[0]="";
    Data[1]="";
    Data[2]="";
    Data[3]="";
    uint8_t state=0;
    while(DataSerial.available()){
      char c = DataSerial.read();
      if(c=='|') state++;
      else Data[state]+=c;
    }
    Serial.println(Data[0]);
    Serial.println(Data[1]);
    Serial.println(Data[2]);
    Serial.println(Data[3]);
  }
}
