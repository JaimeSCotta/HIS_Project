# HIS_Project
High integrity Sistems Project. Frankfurt University of Applied Sciences.

Co-authors: 
- Phuc Hoc Tran - 1235133 
- Jaime Sanchez Cotta - 1430488

This Markdown is written by Phuc Hoc Tran (1235133)

The aim of the project is to create an application which sends an MQTT-SN message with artificial sensor's measurement (client is deployed on FiT/IoT-Lab) as its content to AwS cloud (in this project, we used AwS IoT Core) and visualize the measurement.
## Prerequisites: 
- A registered account on FiT IoT-Lab
- [RIOT](https://github.com/RIOT-OS/RIOT "RIOT") application 
- AwS Account (either free tier, or paid - if you are wealthy enough :-) )
- [Paho MQTT-SN Gateway](https://github.com/eclipse/paho.mqtt-sn.embedded-c/tree/master/MQTTSNGateway "Paho MQTT-SN Gateway") 
- To be updated...

- Bash/shell knowledge.
- C programming and Python programming. 

---
## Architecture: 
### 1. Overarching idea: 
---
SENSORS <--> BORDER ROUTER <--> MQTT-SN Gateway <--> EC2 Instance <--> AwS IoT Core (MQTT Broker) <--> AwS DynamoDB <--> Flask webpage
___

- In this project, the following applications from **RIOT** are ultilized: 
    * emcute_mqttsn: MQTT-SN client, which publishes the sensor data and subscribes to the MQTT-SN Gateway.
    * Border Router.
### 2. In details:
~~some detail explanation here~~
___
## Sensors: 
Based on the requirement, we decided to create an emulated sensor with values follow a Zig Zag pattern, as follow: 
```c
/* function to create Zig Zag pattern */
int posRead =0;
int vMax = 20;
int arrayAux[41];

int zigZag_val(int x){
    if(x>=0&&x<vMax){
    return x;
  }
  else if(x>=vMax&&x<2*vMax){
    return 2*vMax-x;
  }
  else if(x>=2*vMax){
    return zigZag_val(x-2*vMax);
  }
  else if(x<0){
    return zigZag_val(x+2*vMax);
  }
  else return 0;
}

```
And then generate the value: 
```c
/* generate Zig Zag pattern */
void gen_sensors_values(t_sensors* sensors, int position){
  int x;
  int i = 0;
  
  x=-2*vMax;
  while(x<=2*vMax){
    arrayAux[i] = zigZag_val(x);
    x=x+2;
    i++;
  }
  sensors->temperature = arrayAux[position];
  sensors->humidity = arrayAux[position]+1;
  sensors->windDirection = arrayAux[position]-1;
  sensors->windIntensity = arrayAux[position]+2;
  sensors->rainHeight = arrayAux[position]-2;
}
```
___
## EC2 Instance & AwS IoT Core Setup:
Multiple online sources provided ways to set up the EC2 instance and AwS IoT Core. One among which is a great [article]("https://aws.amazon.com/blogs/iot/how-to-bridge-mosquitto-mqtt-broker-to-aws-iot/" "How to Bridge Mosquitto MQTT Broker to AWS IoT") from AwS themselves to setup Brokers on both EC2 and AwS IoT Core. 

Fortunately, although our accounts are not allowed to use IAM servce, we managed to bypass that by simply creating an EC2 instance as the tutorial instruced, and manually create an AwS IoT Core's Thing and attach policy to it. Then, follow the steps on the article above to setup the brokers.

Afterwards, simply bridging the two brokers by pasting the AwS IoT Core's endpoint (can be found via AwS IoT Core's Website/Setting/Device data endpoint, usually in the form *xxx-ats.Iot.REGION.amazonaws.com*) onto __address__ line in the bridge.conf: 
```conf
# ============================================================
# Bridge to AWS IOT
# ============================================================

connection awsiot

#<Paste your AWS IoT Core ATS endpoint in the form of xxxxxxxxxxxxxxx-ats.iot.<region>.amazonaws.com:8883

address xxxxxxxxxxxxxxx-ats.iot.<region>.amazonaws.com:8883

# Specifying which topics are bridged and in what fashion
topic awsiot_to_localgateway in 1
topic localgateway_to_awsiot out 1
topic both_directions both 1

# Setting protocol version explicitly
bridge_protocol_version mqttv311
bridge_insecure false

# Bridge connection name and MQTT client Id, enabling the connection automatically when the broker starts.
cleansession true
clientid bridgeawsiot
start_type automatic
# 2 additional lines 
listener 1884 0.0.0.0
allow_anonymous true
#end 2 additional lines
notifications false
log_type all

# ============================================================
# Certificate based SSL/TLS support
# ============================================================

#Path to the rootCA
bridge_cafile /etc/mosquitto/certs/rootCA.pem

# Path to the PEM encoded client certificate
bridge_certfile /etc/mosquitto/certs/cert.crt

# Path to the PEM encoded client private key
bridge_keyfile /etc/mosquitto/certs/private.key

#END of bridge.conf
```
**IMPORTANT:** Note that, the bridge.conf is specified on EC2 instance, and those 2 additional lines is for allowing anonymous connection at port 1884. We will use this port for the Gateway deployed on FiT/IoT-Lab to connect to the instance. 
___
## MQTT-SN Gateway Setup:
### 1. Initial setup:
Based on the experience of the co-author, setting up the latest Paho was quite cumbersome and somewhat difficult, since the creator of Paho Mosquitto package changed the name convention in the latest release, and all the online sources used the older version (hence, the old name convention). Therefore, we decided to opt for the old version for this project. Simply enter these commands in the CLI:  
```
wget --progress=dot:giga --no-check-certificate -O paho.mqtt-sn.embedded-c.zip https://github.com/eclipse/paho.mqtt-sn.embedded-c/archive/f2dcda358f21e264de57b47b00ab6165bab4da18.zip
unzip paho.mqtt-sn.embedded-c.zip
rm paho.mqtt-sn.embedded-c.zip 
mv paho.mqtt-sn.embedded-c-f2dcda358f21e264de57b47b00ab6165bab4da18 paho.mqtt-sn.embedded-c
cd paho.mqtt-sn.embedded-c/MQTTSNGateway 
```
To run the Gateway, one must build the application first

```
./build.sh udp6
cd bin
./MQTTSNGateway
```

### 2. Custom configuration to deploy on FiT/IoT-Lab: 

___
## DynamoDB

___
## Python Flask/Django Webpage: 