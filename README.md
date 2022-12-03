# HIS_Project
High integrity Sistems Project. Frankfurt University of Applied Sciences.
Realized by Phuc Tran and Jaime Sanchez Cotta.
The aim of the project is to create an application which shows the actual temperature.
## Architecture: 
 - MQTT-SN on RIOT application. 
 - MQTT on AwS IoT Core. 
 - Temperature sensor data are collected on the RIOT application side. 
 - Mosquitto package is used on the server to translate MQTT-SN to MQTT (Keep in mind that we also have to set up MQTT-Broker as well). 
 - Border router (Code example can be found on FIT IoT-Lab Documentation) will be set up on A8-M3 node. 

## Used in the project:
- Temperature sensor provided by AWS IoT Core (? --> @Phuc: Is this true?) 
- Two A8-M3 cores provided by FIT IoT-LAB.
