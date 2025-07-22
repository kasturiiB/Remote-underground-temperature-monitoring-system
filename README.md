# Remote underground temperature monitoring system
## Overview
 A solar-powered, remote temperature monitoring system that can measure and log underground and near-surface environmental conditions in real-time is designed and built.
The Remote underground temperature monitoring system is designed for continuous, real-time environmental data collection in remote locations. It supports multiple sensors and timestamped logging, enabling precise analysis of underground and surface temperatures.
<br>
Built for durability and scalability, the system is solar-powered, making it ideal for unattended deployments. <br> <br> <br> 
## Why Choose the Remote Underground Temperature Monitoring System? <br>
This project facilitates accurate, timestamped temperature monitoring in challenging environments. Key features include:

 - Multi-Sensor Support: Collects data from DHT22 and DS18B20 sensors to provide comprehensive environmental insights.

 - Timestamped Logging: Utilizes RTC modules to ensure precise and synchronized data records.

 - Solar-Powered: Optimized for remote, off-grid deployments with reliable, sustainable energy.

 - Continuous Monitoring: Enables real-time data collection for ongoing analysis, control, and environmental management. <br> <br> <br>
## Features <br>
- Measures soil temperature at multiple depths using DS18B20 sensors.

- Measures ambient temperature using DHT22.

- Timestamps data using a DS3231 RTC module.

- Sends data to a server/cloud via GSM (SIM800L).

- Powered by solar panel + Li-ion battery with charge controller.

- Enclosed in a weatherproof, modular hardware structure for outdoor deployment. <br> <br> <br>
### 🔧 Hardware Components

| Component                | Description                           |
|--------------------------|----------------------------------------|
| **ESP32**                | Main controller                        |
| **DS18B20 (x2)**         | Waterproof temp sensors (underground)  |
| **DHT22**                | Ambient temp & humidity sensor         |
| **DS3231 RTC**           | Real-Time Clock for timestamps         |
| **SIM800L**              | GSM module for communication           |
| **Solar panel (5–6V)**   | Power source                           |
| **Li-ion Battery (18650)** | Backup power (2000–3000 mAh)         |
| **PVC Cylinder + Enclosure** | Custom housing for outdoor use     |

## Software Overview
Language: Arduino (C++)
<br>
Libraries Used:
- OneWire.h and DallasTemperature.h for DS18B20

- DHT.h for DHT22

- RTClib.h or DS3231.h for RTC

- SoftwareSerial or TinyGSM for GSM module <br> <br><br>
## 🔁 Data Logging & Transmission
- Sensor readings are taken every 5 minutes using the DS18B20 and DHT22 sensors.

- Each reading is timestamped using the DS3231 RTC module.

- The ESP32 stores the readings in memory temporarily.

- Every 1 hour, the ESP32 formats and sends the collected data to ThingSpeak via the GSM module using HTTP POST.


