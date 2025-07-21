#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHTPIN 4        // DHT22 data pin
#define ONE_WIRE_BUS 18 // DS18B20 shared data pin
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtc;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // RTC Init
  Wire.begin(21, 22);
  if (!rtc.begin()) {
    Serial.println("RTC not found");
    while (true);
  }

  dht.begin();
  sensors.begin();
}

void loop() {
  DateTime now = rtc.now();

  // --- Read DS18B20 temperatures ---
  sensors.requestTemperatures();
  float dsTemp1 = sensors.getTempCByIndex(0);
  float dsTemp2 = sensors.getTempCByIndex(1);

  // --- Read DHT22 temperature ---
  float dhtTemp = dht.readTemperature();

  Serial.print("Date: ");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.println(now.year());

  Serial.print("DHT22 Temp: ");
  Serial.print(dhtTemp);
  Serial.println(" °C");

  Serial.print("DS18B20 #1 Temp: ");
  Serial.print(dsTemp1);
  Serial.println(" °C");

  Serial.print("DS18B20 #2 Temp: ");
  Serial.print(dsTemp2);
  Serial.println(" °C");

  Serial.print("Time: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());

  Serial.println("------------------------");

  delay(2000);
}
