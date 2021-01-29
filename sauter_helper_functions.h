#ifndef SAUTER_HELPER_H
#define SAUTER_HELPER_H

#include "config.h"
#include "console_helper_functions.h"
#include "motor_helper_functions.h"

int BUFFER_SIZE = 64;

// Function to read command from serial port
float get_sauter_one_measure(int BUFFER_SIZE) {
  int i = 0, j = 0;
  char data[BUFFER_SIZE];

  while(i != 7){
    //Serial1.write(0x39);
    
    i = 0;
    for (i = 0; i < BUFFER_SIZE; i++) {
      data[i] = '\0';
    }
    
    i = 0;
    j = 0;
    while(Serial1.available() < 7){
      if(j == 0) {
        Serial1.write(0x39);
        j++;
      } else if (j == 2000) {
        //Serial.println("Sauter probably disconnected");
        j = 0;
      } else j++;
      
    }
    
    while(Serial1.available()) {
      data[i++] = Serial1.read();
      if (i == BUFFER_SIZE) {
        break;
      }
    }
  }

  // first digit is sign
  if(data[0] == '0'){
    data[0] = '-';
  } else {
    data[0] = '+';
  }
  
  return atof(data);
}


float get_sauter_force(int n, int BUFFER_SIZE){
  if (n==1){
    return get_sauter_one_measure(BUFFER_SIZE);
    
  } else {
    float mean = 0;
    for(int i = 0; i < n; i++){
      mean = mean + get_sauter_one_measure(BUFFER_SIZE)/n;
    }
    return mean;
  }
}

//void test_force_sign(float x, bool wait){
//  if (sign != (force > 0)){
//    Serial.print("Sign changed at x: ");
//    Serial.print(x);
//    if (wait)
//      wait_for_enter();
//  }
//}

#endif
