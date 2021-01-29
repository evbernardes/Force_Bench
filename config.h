#ifndef CONFIG_H
#define CONFIG_H

#include "console_helper_functions.h"
#include "sauter_helper_functions.h"
#include "motor_helper_functions.h"

// Sauter config data
byte pin_direction = 12;
byte pin_step = 13;

// GeckoDrive microstep data
float angle_per_step = 0.18; // 1.8 divided by 10, microstepping
int steps_per_rev = (int) 360.0/angle_per_step;
float millimeters_per_rev = 54.0;
float millimeters_per_step = millimeters_per_rev/steps_per_rev;

// Force limits
float FORCE_MAX_ABSOLUTE = 20.;
float FORCE_MAX = 15.0;
float FORCE_LIM = 8.0;
float FORCE_ZERO = 1.0;
float X_MIN = 5.0;

// Other options
int n_force_measures = 1;
int steps_initial = 3;
int steps_test = 1;
int steps_final = 3;
int pulse_period = 10; // milliseconds
float pulse_dutycycle = 0.20;
bool wait_between_stages = false;

void change_config(){
  
  char strBuff[2000];
  int option = 0;
  
  while(true){
    Serial.println("----------------------------------------------------");
    Serial.println("Options:");
    Serial.print("1) Force max (for security) = "); Serial.println(FORCE_MAX);
    Serial.print("2) Force limit (define when to start and stop tests) = "); Serial.println(FORCE_LIM);
    Serial.print("3) Force stop (close to zero, to know when to stop to rest) = "); Serial.println(FORCE_ZERO);
    Serial.print("4) Length min (minimal displacement before stopping if force = Force limit) = "); Serial.println(X_MIN);
    Serial.print("5) Number of measures (force measure will be a mean of multiple measures) = "); Serial.println(n_force_measures);
    Serial.print("6) Initial steps (number of steps per loop when finding the start of test) = "); Serial.println(steps_initial);
    Serial.print("7) Test steps (number of steps per loop while doing tests) = "); Serial.println(steps_test);
    Serial.print("8) Final steps (number of steps per loop when finding the rest position at the end) = "); Serial.println(steps_final);
    Serial.print("9) Stepper motor pulse period (in milliseconds) = "); Serial.println(pulse_period);
    Serial.print("10) Stepper motor pulse dutycycle (from 0 to 100) = "); Serial.println((int)100*pulse_dutycycle);
    Serial.print("11) Wait between stages? (0 for false, 1 for true) = "); Serial.println(wait_between_stages? 1 : 0);
    Serial.println("12) Quit to main menu");
    Serial.println("----------------------------------------------------");
    
    Serial.print("Choose option: ");
    option = get_int();
    Serial.println(option);
    if (option == 12) break;
    
    else if (option < 0 or option > 13)
      Serial.println("Invalid option");
    
    else {
      Serial.print("Enter new value: ");
      float value = get_float();
      Serial.println(value);

      switch (option) {
        case 1:
        if (value > 0 and value < FORCE_MAX_ABSOLUTE)
          FORCE_MAX = value;
        else
          {Serial.print("Invalid value, must be between 0 and "); Serial.println(FORCE_MAX_ABSOLUTE);}
        break;
          
        case 2:
        if (value > 0 and value < FORCE_MAX_ABSOLUTE)
          FORCE_LIM = value;
        else
          {Serial.print("Invalid value, must be between 0 and "); Serial.println(FORCE_MAX_ABSOLUTE);}
        break;
          
        case 3:
        if (value > 0 and value < FORCE_MAX_ABSOLUTE)
          FORCE_ZERO = value;
        else
          {Serial.print("Invalid value, must be between 0 and "); Serial.println(FORCE_MAX_ABSOLUTE);}
        break;
          
        case 4:
        X_MIN = value;
        break;
  
        case 5:
        n_force_measures = (int) value;
        break;
  
        case 6:
        steps_initial = (int) value;
        break;
  
        case 7:
        steps_test = (int) value;
        break;
  
        case 8:
        steps_final = (int) value;
        break;
  
        case 9:
        pulse_period = value;
        break;
        
        case 10:
        if (value > 0 and value < 100)
          pulse_dutycycle = value/100;
        else
          Serial.println("Invalid value");
        break;
  
        case 11:
        if (value == 0)
          wait_between_stages = false;
        else if (value == 1)
          wait_between_stages = true;
        else
          Serial.println("Invalid value");
        break;
  
        default:
        Serial.println("Invalid option");
      }
    }
  }
}

#endif /* CONFIG_H */
