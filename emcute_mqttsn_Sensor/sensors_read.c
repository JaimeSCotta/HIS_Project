#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#include "lpsxxx.h"
#include "lpsxxx_params.h"
#include "lsm303dlhc.h"
#include "lsm303dlhc_params.h"
#include "isl29020.h"
#include "isl29020_params.h"
#include "l3g4200d.h"
#include "l3g4200d_params.h"

#include "sensordefinition.h"
#include "sensors_read.h"


/* Declare the devices we are using */
static lpsxxx_t lpsxxx;
static lsm303dlhc_t lsm303dlhc;
static isl29020_t isl29020;
static l3g4200d_t l3g4200d;


static char stack[THREAD_STACKSIZE_MAIN];

static void *thread_handler(void *arg){
  (void)arg;
  
  while(1){
    lsm303dlhc_3d_data_t mag_value;
    lsm303dlhc_3d_data_t acc_value;

    lsm303dlhc_read_acc(&lsm303dlhc, &acc_value);
    printf("Accelerometer x: %i y: %i z: %i\n", 
	    acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);
 
    lsm303dlhc_read_mag(&lsm303dlhc, &mag_value);
    printf("Magnetometer x: %i y: %i z: %i\n -------------------------------------\n", 
	    mag_value.x_axis, mag_value.y_axis, mag_value.z_axis);

    sensors->magnetometer_x = mag_value.x_axi;
    sensors->magnetometer_y = mag_value.y_axis;
    sensors->magnetometer_z = mag_value.z_axis;

    sensors->acecelerometer_x = acc_value.x_axis;
    sensors->acecelerometer_y = acc_value.y_axis;
    sensors->acecelerometer_z = acc_value.z_axis;

    xtimer_sleep(2);
  }

  return 0;
}



void sensors_read (t_sensors* sensors)){

/* Initialize the lps331ap semsor */
lpsxxx_init(&lpsxxx, &lpsxxx_params[0]);

/* Initialize the lsm303dlhc sensor */
lsm303dlhc_init(&lsm303dlhc, &lsm303dlhc_params[0]);

/* Initialize the isl29020 sensor */
isl29020_init(&isl29020, &isl29020_params[0]);

/* Initialize the l3g4200d sensor */
l3g4200d_init(&l3g4200d, &l3g4200d_params[0]);


thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
		0, thread_handler, NULL, "lsm303dlhc");

/* Print the values of the sensors every 2 seconds */
while (1) {
  uint16_t pres = 0;
  int16_t temp = 0;
  l3g4200d_data_t acc_data;
  
  lpsxxx_read_temp(&lpsxxx, &temp);
  lpsxxx_read_pres(&lpsxxx, &pres);
  l3g4200d_read(&l3g4200d, &acc_data);

  printf("Pressure: %uhPa, Temperature: %i.%uºC\n Light: %5i LUX\n Gyro data [dps] -X: %6i Y: %6i Z: %6i \n -------------------------------------\n",
	  pres, (temp / 100), (temp % 100), isl29020_read(&isl29020), acc_data.acc_x, acc_data.acc_y, acc_data.acc_z);
  sensors->pressure = pres;
  sensors->temperature = temp;
  sensors->light = isl29020_read(&isl29020);
  sensors->gyroscopic_x = acc_data.acc_x;
  sensors->gyroscopic_y = acc_data.acc_y;
  sensors->gyroscopic_z = acc_data.acc_z;

  xtimer_sleep(2);
}

}
