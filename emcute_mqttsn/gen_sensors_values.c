#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "time.h"

#include "sensordefinition.h"
#include "gen_sensors_values.h"
#include<math.h>

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



  
// function that generate sensor values
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
/*
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
}*/
