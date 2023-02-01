# HIS_Project
High integrity Sistems Project. Frankfurt University of Applied Sciences.

Co-authors: 
- Phuc Hoc Tran - 1235133 
- Jaime Sanchez Cotta - 1430488

This Markdown is written by Phuc Hoc Tran (1235133)

The aim of the project is to create an application which shows the actual temperature.
## Prerequisites: 
- A registered account on FiT IoT-Lab
- [RIOT](https://github.com/RIOT-OS/RIOT "RIOT") application 
- AwS Account (either free tier, or paid - if you are wealthy enough :-) )
- [Paho MQTT-SN Gateway](https://github.com/eclipse/paho.mqtt-sn.embedded-c/tree/master/MQTTSNGateway "Paho MQTT-SN Gateway") 
- To be updated...

- Bash/shell knowledge.
- C programming and Python programming. 


## Architecture: 
### 1. Overarching idea: 
---
SENSORS <--> BORDER ROUTER <--> MQTT-SN Gateway <--> EC2 Instance <--> AwS IoT Core (MQTT Broker) <--> AwS DynamoDB <--> Flask webpage
___

- In this project, the following applications from **RIOT** are ultilized: 
    * emcute_mqttsn: MQTT-SN client, which publishes the sensor data and subscribes to the MQTT-SN Gateway.
    * Border Router.

### 2. In details:

## Sensors: 
## EC2 Instance && AwS IoT Core Setup: 
## MQTT-SN Gateway Setup:
## DynamoDB
## Python Flask/Django Webpage: 