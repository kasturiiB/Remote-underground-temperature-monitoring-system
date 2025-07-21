#include <DHT.h>

#define DHTPIN1 4
#define DHTPIN2 5
#define DHTTYPE DHT22
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht1.begin();
  dht2.begin();
}
void loop() {
  float t1 = dht1.readTemperature();
  float t2 = dht2.readTemperature();
  Serial.print("T1: ");
  Serial.print(t1);
  Serial.print(" °C | T2: ");
  Serial.print(t2);
  Serial.println(" °C");
  delay(2000);
}
