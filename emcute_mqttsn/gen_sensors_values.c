#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "time.h"

#include "sensordefinition.h"
#include "gen_sensors_values.h"
// function that computes random values in the specified range
int rand_val(int min, int max){
  srand(time(NULL));
  return (rand() % (int)((max-min+1) * 100)) / 100 + min;
}

// function that generate sensor values
void gen_sensors_values(t_sensors* sensors){
  sensors->temperature = rand_val(-50,50);
  sensors->humidity = rand_val(0, 100);
  sensors->windDirection = rand_val(0, 360);
  sensors->windIntensity = rand_val(0, 100);
  sensors->rainHeight = rand_val(0, 50);
}