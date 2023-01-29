#include "cmd_start.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sensordefinition.h"
#include "time.h"
#include <xtimer.h>
#include "sensors_read.h"
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

  // name of the topic
  char topic[32];
  sprintf(topic,"sensor/station%d", atoi(argv[3]));
  // sensors struct
  t_sensors sensors;
  // json that it will published
  char json[512];

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
    sensors_read(&sensors);

    // fills the json document
    sprintf(json, "{\"id\": \"%d\", \"Datetime\": \"%s\", \"Temperature\": "
                  "\"%i.%uºC\", \"Pressure\": \"%uhPa\", \"Light\": \"%2iLUX\", \"Gyroscopic [dps]\": \"x: %1i y: %1i z: %1i\", "
                  "\"Acelerometer\": \"x: %1i y: %1i z: %1i\", \"Magnetometer\": \"x: %1i y: %1i z: %1i\"}",
                  atoi(argv[3]), datetime, (sensors.temperature/100),(sensors.temperature%100),
		  sensors.pressure, sensors.light, sensors.gyroscopic_x, sensors.gyroscopic_y, sensors.gyroscopic_z,
                  sensors.acecelerometer_x, sensors.acecelerometer_y,sensors.acecelerometer_z,
                  sensors.magnetometer_x, sensors.magnetometer_y, sensors.magnetometer_z);

    // publish to the topic
    pub(topic, json, 0);

    // it disconnects from the gateway
    discon();

    // it sleeps for five seconds
    xtimer_sleep(5);
  }

  return 0;
}
