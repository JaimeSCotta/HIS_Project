
// struct that contains sensors
typedef struct sensors{
  int light;
  int16_t temperature;
  uint16_t pressure;
  short acecelerometer_x;
  short acecelerometer_y;
  short acecelerometer_z;
  short magnetometer_x;
  short magnetometer_y;
  short magnetometer_z;
  short gyroscopic_x;
  short gyroscopic_y;
  short gyroscopic_z;
}t_sensors;
