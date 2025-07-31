//final working code, it takes temp readings every 20 sec for 1 minute and send all 3 readings together to thingSpeak in JSON format. It also indicates battery level. 
//in ThingSpeak the data is stored and can be stored for over a year, simultaneously plotting each reading on a separate graph.
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Wire.h>
#include "RTClib.h"

#define DS1_pin 18   // 1s DS18B20 pin
#define DS2_pin 14   // 2nd DS18B20 pin
#define DHT_PIN 23        // DHT22 pin
#define DHT_TYPE DHT22
#define fullBatt 12
#define lowBatt 13

HardwareSerial sim800(2); // SIM800L on UART2 (RX = 16, TX = 17)
OneWire oneWire1(DS1_pin);
OneWire oneWire2(DS2_pin);
DallasTemperature sensor1(&oneWire1);
DallasTemperature sensor2(&oneWire2);
DHT dht(DHT_PIN, DHT_TYPE);
RTC_DS3231 rtc;

const char* APN = "airtelgprs.com"; // "*"" means pointer. it points to a const character or strinng
const char* API_KEY = "UW3X9ZE4NVMF0EF3";
const char* CHANNEL_ID = "3010755";

// Data Storage 
struct TempData {   //TempData is an obj that has the following structure
  String timestamp;
  float t1, t2, t3;
  int BatteryLevel;
};
TempData readings[3]; //readings is an array of 4elements each having structure as TempData
int readingIndex = 0;

//func declarations, they tell the compiler that these funcs exist and will be used  
void collectTemperature(); //void so takes no i/p parameters but collectes temp data
void sendAT(String cmd);// returns nothing but takes string as i/p i.e AT cmds
bool waitForResponse(String target, int timeout);//gives true if target string(could be anything, eg: OK) is received within 'timeout' time
String buildJSON();//returns a string (JSON obj), takes no i/p
void sendToThingSpeak(); 
String getBatteryStatus(); //returns a string

// === Setup ===
void setup() {
  Serial.begin(115200);
  sim800.begin(9600, SERIAL_8N1, 16, 17); //8 data bits, 1stop bit
  sensor1.begin();
  sensor2.begin();
  dht.begin();
  rtc.begin();
  pinMode(fullBatt, INPUT);
  pinMode(lowBatt, INPUT);
  delay(1000);
  Serial.println("Setup complete");
}

// === Main Loop ===
int lastSecondChecked = -1;

void loop() {
  DateTime now = rtc.now();
  int currentSecond = now.second();
  int currentMinute = now.minute();

  // send only after 5mins i.e. multiple of 5's (0, 20, 40)
  if ((currentSecond % 20 == 0) && (currentSecond != lastSecondChecked) && readingIndex < 12) {
    collectTemperature();
    readingIndex++;
    lastSecondChecked = currentSecond;
  }
  if (readingIndex >= 3) {
    Serial.println("Sending batch to ThingSpeak...");
    sendToThingSpeak();
    readingIndex = 0;
  }

  delay(1000); 
}


// Collect temperature and timestamp 
void collectTemperature() {
  sensor1.requestTemperatures();
  float t1 = sensor1.getTempCByIndex(0);
  sensor2.requestTemperatures();
  float t2 = sensor2.getTempCByIndex(0);
  float t3 = dht.readTemperature();

  if (isnan(t3)) {
    Serial.println("DHT read failed!");
    t3 = -999.0;
  }

  int batteryCode;
  if (digitalRead(fullBatt) == HIGH) {
    batteryCode = 3;
  } else if (digitalRead(lowBatt) == HIGH) {
    batteryCode = 1;
  } else {
    batteryCode = 2;
  }

  DateTime now = rtc.now();
  char ts[25];
  sprintf(ts, "%04d-%02d-%02dT%02d:%02d:%02dZ",
          now.year(), now.month(), now.day(),
          now.hour(), now.minute(), now.second());

  readings[readingIndex].timestamp = String(ts);
  readings[readingIndex].t1 = t1;
  readings[readingIndex].t2 = t2;
  readings[readingIndex].t3 = t3;
  readings[readingIndex].batteryCode = batteryCode;

  Serial.println("Stored Reading " + String(readingIndex) +
                 ": DS1=" + String(t1) +
                 "°C, DS2=" + String(t2) +
                 "°C, DHT22=" + String(t3) +
                 "°C, BatteryCode=" + String(batteryCode) +
                 " at " + String(ts));
}


// === Send to ThingSpeak via SIM800L ===
void sendToThingSpeak() {
  String payload = buildJSON();

  sendAT("AT");
  sendAT("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  sendAT("AT+SAPBR=3,1,\"APN\",\"" + String(APN) + "\"");
  sendAT("AT+SAPBR=1,1");
  delay(3000);
  sendAT("AT+SAPBR=2,1");

  sendAT("AT+HTTPINIT");
  sendAT("AT+HTTPPARA=\"CID\",1");
  sendAT("AT+HTTPPARA=\"URL\",\"http://api.thingspeak.com/channels/" + String(CHANNEL_ID) + "/bulk_update.json\"");
  sendAT("AT+HTTPPARA=\"CONTENT\",\"application/json\"");

  sim800.println("AT+HTTPDATA=" + String(payload.length()) + ",10000");
  delay(100);
  if (waitForResponse("DOWNLOAD", 5000)) {
    sim800.print(payload);
    delay(3000);
    sendAT("AT+HTTPACTION=1");
    delay(6000);
  }

  sendAT("AT+HTTPTERM");
  //sendAT("AT+SAPBR=0,1");

  delay(20000);
// === Build JSON for bulk upload ===
String buildJSON() {
  String json = "{\"write_api_key\":\"" + String(API_KEY) + "\",\"updates\":[";
  for (int i = 0; i < 3; i++) {
    json += "{\"created_at\":\"" + readings[i].timestamp + "\",";
    json += "\"field1\":" + String(readings[i].t1, 2) + ",";
    json += "\"field2\":" + String(readings[i].t2, 2) + ",";
    json += "\"field3\":" + String(readings[i].t3, 2) + ",";
    json += "\"field4\":" + String(readings[i].batteryCode) + "}";
    if (i < 2) json += ",";
  }
  json += "]}";
  return json;
}


// Send AT and print response 
void sendAT(String cmd) {
  sim800.println(cmd);
  delay(500);
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
}

// Wait for a keyword in SIM800L response 
bool waitForResponse(String target, int timeout) {
  unsigned long start = millis();
  String response = "";
  while (millis() - start < timeout) {
    while (sim800.available()) {
      char c = sim800.read(); //reads one character from sim800's reply and adds it to response strinng
      response += c; //one ch at a time because sim800 sends data byte by byte and not full reply 
      if (response.indexOf(target) != -1) return true;
    }
  }
  return false;
}