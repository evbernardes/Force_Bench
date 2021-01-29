#include "config.h"
#include "console_helper_functions.h"
#include "sauter_helper_functions.h"
#include "motor_helper_functions.h"

float x;
float force;
bool sign;

void setup() {
  // start serial connection to computer
  Serial.begin(9600);
  
  // start serial connection to Sauter
  Serial1.begin(9600);

  // set connection to Gecko driver
  pinMode(pin_direction, OUTPUT);
  pinMode(pin_step, OUTPUT);
  delay(100);
}

void loop() {
  x = 0.0;
  force = 0.0;
  // flush data from console
  while(Serial.available() ) {Serial.read();}
  
  int option = 0;
  while(option < 1 or option > 6){
    Serial.println("----------------------------------------------------");
    Serial.println("Force bench cycle test. Choose option:");
    Serial.println("1) open.");
    Serial.println("2) open, then close.");
    Serial.println("3) close.");
    Serial.println("4) close, then open");
    Serial.println("5) manual");
    Serial.println("6) config");
    //Serial.println("7) change other options");
    option = get_int();
  }
  
  while(Serial.available() ) {Serial.read();} // read everything coming from sauter
  while(Serial1.available() ) {Serial1.read();} // read everything coming from sauter

  //
  // Configurations menu option selected
  //
  if (option == 6){
    change_config();

  //
  // Manual mode selected
  // Manually enter desired position 
  //
  } else if(option == 5){
    Serial.println("----------------------------------------------------");
    Serial.println("Manual mode selected");
    Serial.println("Measure height and enter it: ");
    x = get_float();
    force = get_sauter_force(n_force_measures,BUFFER_SIZE);
    print_data(x, force);
    float target;
    bool stop = false;
    bool init = false;
    int direction, steps;
    while(~stop){
      Serial.println("Desired position (0 to stop, \'negative\' to go to initial position): ");
      target = get_float();
      if (target == 0)
        break;
      else if (target < 0){
        steps = steps_initial;
        target = -target;
        init = true;
      } else {
        steps = steps_test;
        init = false;
      }
      
      direction = target < x? HIGH : LOW;
      while(abs(x - target) > 2*millimeters_per_step*steps){
        make_steps(direction,steps,pin_direction,pin_step,pulse_period, pulse_dutycycle);
        x = x - 2*(direction-0.5)*millimeters_per_step*steps;
        //Serial.print("dist: "); Serial.print(abs(x - target)); Serial.print(" ");
        if(init)
          force = get_sauter_force(1,BUFFER_SIZE);
        else
          force = get_sauter_force(n_force_measures,BUFFER_SIZE);
        print_data(x, force);
        if(abs(force) >= FORCE_MAX) {
          stop = true;
          Serial.println("Force over limits, stopping test...");
          break;
        }
      }
    }

  //
  // Auto mode selected
  // Test all the way until force limit is achieved
  //
  } else {

    int direction_odd = (option == 1 || option == 2)? HIGH : LOW;
    int direction_even = HIGH - direction_odd;

    int direction = (option == 1 || option == 2)? HIGH : LOW;
    bool stop = false;
    //print_dir(direction);
    
    // Stage 1
    char proceed = 'a';
    while(proceed != 'y' && proceed != 'n'){
      Serial.println("Go to initial position (y/n)?");
      proceed = get_char();
      //Serial.println(proceed);
    }

    if(proceed == 'y'){
      Serial.println("Going to initial position...");
      while(abs(force) < FORCE_LIM){
        make_steps(direction,steps_initial,pin_direction,pin_step,pulse_period, pulse_dutycycle);
        force = get_sauter_force(1,BUFFER_SIZE);
      }
      sign = force > 0;
    }
    
    // Stage 2:
    int i = 0;
    if(!stop){
      Serial.println("Starting test phase 1...");
      direction = switch_dir(direction);
      //print_dir(direction);
      Serial.println("Measure height and enter it: ");
      x = get_float();
      while(abs(force) < FORCE_LIM || i*millimeters_per_step < X_MIN){
        if(abs(force) >= FORCE_MAX) {
          stop = true;
          Serial.println("Force over limits, stopping test...");
          break;
        }
        
        force = get_sauter_force(n_force_measures,BUFFER_SIZE);
        //x = height_initial + 2*(direction_odd-0.5)*i*millimeters_per_step;
        x = x - 2*(direction-0.5)*millimeters_per_step*steps_test;
        sign = force > 0;    
        print_data(x, force);
        make_steps(direction,steps_test,pin_direction,pin_step,pulse_period, pulse_dutycycle);
        i += 1;
      }
    }
  
    // Stage 3:
    if(!stop && (option == 2 || option == 4)){
      Serial.println("Starting test phase 2...");
      direction = switch_dir(direction);
      //print_dir(direction);

      if(wait_between_stages) wait_for_enter();
      int steps = i;
      for(i = 0; i < steps; i++){
        force = get_sauter_force(n_force_measures,BUFFER_SIZE);
        x = x - 2*(direction-0.5)*millimeters_per_step*steps_test;
        sign = force > 0;
        print_data(x, force);
        make_steps(direction,steps_test,pin_direction,pin_step,pulse_period, pulse_dutycycle);
        if(abs(force) >= FORCE_MAX) {
          stop = true;
          Serial.println("Force over limits, stopping test...");
          break;
        }
      }
      // Print last one
      print_data(x, force);
    }
  
    // Stage 4: Go to low force position
    if(!stop){
      Serial.println("Stopping...");
      direction = switch_dir(direction);
      //print_dir(direction);
      while(abs(force) > FORCE_ZERO){
        force = get_sauter_force(n_force_measures,BUFFER_SIZE);
        make_steps(direction,steps_final,pin_direction,pin_step,pulse_period, pulse_dutycycle);
        if(abs(force) >= FORCE_MAX){
          break;
        }
      }
    }
  }

}

int switch_dir(int value){
  if (value == HIGH){
    return LOW;
  } else if (value == LOW){
    return HIGH;
  }
}
