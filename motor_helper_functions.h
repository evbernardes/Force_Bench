#ifndef MOTOR_HELPER_H
#define MOTOR_HELPER_H

#include "config.h"
#include "console_helper_functions.h"
#include "sauter_helper_functions.h"

//
// Send pulses 
//
void make_steps(int dir, int number_of_steps, byte pin_direction, byte pin_step, int pulse_period, float pulse_dutycycle)
{
  //int number_of_steps = millimeters*steps_per_rev/millimeters_per_rev;
  digitalWrite(pin_direction, dir);
  for(int n = 0; n < number_of_steps; n++) {
//    if(n%50 == 0){
//      Serial.print("Step no ");
//      Serial.println(n);
//    }
    digitalWrite(pin_step, HIGH);
    delayMicroseconds(pulse_period*pulse_dutycycle);
    digitalWrite(pin_step, LOW);
    delay(pulse_period*(1-pulse_dutycycle));
  }
}

#endif
