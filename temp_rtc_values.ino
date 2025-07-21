#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>
#define DHTPIN1 4
#define DHTPIN2 5
#define DHTTYPE DHT22
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  Wire.begin(21,22);
  if(!rtc.begin()){
    Serial.println("rtc not found");
    while(true);
  }
  
  dht1.begin();
  dht2.begin();
}
void loop() {
  DateTime now=rtc.now();
  Serial.print("Date:");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.println(now.year());

  float t1 = dht1.readTemperature();
  float t2 = dht2.readTemperature();
  Serial.print("T1: ");
  Serial.print(t1);
  Serial.print(" °C,   T2:");
  Serial.print(t2);
  Serial.println(" °C");


  Serial.print("Time:");
  Serial.print(now.hour());
  Serial.print("-");
  Serial.print(now.minute());
  Serial.print("-");
  Serial.println(now.second());
  Serial.println("------------------------");
  
  

  delay(2000);
}
