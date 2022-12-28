#include "cmd_start.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sensordefinition.h"
#include "time.h"
#include <xtimer.h>
#include "gen_sensors_values.h"
#include "con.h"
#include "pub.h"
#include "discon.h"

// new shell command: start the station
// the function takes in input ip address and port of the gateway,
// and the id of the specified station
// every five seconds it generates new sensor values and publish them to 
// sensor/station + station id
int cmd_start(int argc, char **argv){
  if (argc < 4) {
      printf("usage: %s <address> <port> <id>\n", argv[0]);
      return 1;
  }
  // sensors struct
  t_sensors sensors;
  // name of the topic
  char topic[32];
  sprintf(topic,"sensor/station%d", atoi(argv[3]));
  
  // json that it will published
  char json[128];
  
  while(1){
    // it tries to connect to the gateway
    if (con(argv[1], atoi(argv[2]))) {
      continue;
    }
    
    // takes the current date and time
    char datetime[20];
    time_t current;
    time(&current);
    struct tm* t = localtime(&current);
    int c = strftime(datetime, sizeof(datetime), "%Y-%m-%d %T", t);
    if(c == 0) {
      printf("Error! Invalid format\n");
      return 0;
    } 

    // updates sensor values
    gen_sensors_values(&sensors);

    // fills the json document
    sprintf(json, "{\"id\": \"%d\", \"datetime\": \"%s\", \"temperature\": "
                  "\"%d\", \"humidity\": \"%d\", \"windDirection\": \"%d\", "
                  "\"windIntensity\": \"%d\", \"rainHeight\": \"%d\"}",
                  atoi(argv[3]), datetime, sensors.temperature, sensors.humidity, 
                  sensors.windDirection, sensors.windIntensity, sensors.rainHeight);
      
    // publish to the topic
    pub(topic, json, 0);

    // it disconnects from the gateway
    discon();

    // it sleeps for five seconds
    xtimer_sleep(5);
  }

  return 0;
}