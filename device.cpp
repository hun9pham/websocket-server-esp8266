#include "device.h"

int temp =0;
int humidity =0;

int getTemp(){
  temp = random(1,70);
  temp = temp % 35;
  return temp;
}
int getHumidity(){
  humidity = random(1,70);
  humidity = humidity % 25;
  return humidity;
}
