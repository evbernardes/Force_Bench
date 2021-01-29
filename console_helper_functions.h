#ifndef CONSOLE_HELPER_H
#define CONSOLE_HELPER_H

#include "config.h"
#include "sauter_helper_functions.h"
#include "motor_helper_functions.h"


void wait_for_enter(){
  while(!Serial.available() ){} // wait for key
  while(Serial.available() ){Serial.read();} // read everything when something is written on the console
}


int get_char(){
  while(Serial.available() == 0) {}
  char char_incoming, data[BUFFER_SIZE];
  for (int i = 0; i < BUFFER_SIZE; i++) {
    data[i] = '\0';
  }
  int i = 0;
  while(Serial.available() > 0) {
    char_incoming = Serial.read();
    if (i == BUFFER_SIZE || char_incoming == '\r' || char_incoming == '\n') {
      break;
    }
    data[i] = char_incoming;
    i++;
    delay(3);
  }
  return data[0];
}

int get_int(){
  while(Serial.available() == 0) {}
  char char_incoming, data[BUFFER_SIZE];
  for (int i = 0; i < BUFFER_SIZE; i++) {
    data[i] = '\0';
  }
  int i = 0;
  while(Serial.available() > 0) {
    char_incoming = Serial.read();
    if (i == BUFFER_SIZE || char_incoming == '\r' || char_incoming == '\n') {
      break;
    }
    data[i] = char_incoming;
    i++;
    delay(3);
  }
  return atoi(data);
}

//
float get_float(){
  while(Serial.available() == 0) {}
  char char_incoming, data[BUFFER_SIZE];
  for (int i = 0; i < BUFFER_SIZE; i++) {
    data[i] = '\0';
  }
  int i = 0;
  while(Serial.available() > 0) {
    char_incoming = Serial.read();
    if (i == BUFFER_SIZE || char_incoming == '\r' || char_incoming == '\n') {
      break;
    }
    data[i] = char_incoming;
    i++;
    delay(3);
  }
  return atof(data);
}

void print_data(float x, float force){
    Serial.print("x: ");
    Serial.print(x, 5);
    Serial.print("\tF: ");
    Serial.println(force, 3);
}

void print_dir(int value){
  if (value == HIGH){
    Serial.println("HIGH");
  } else if (value == LOW){
    Serial.println("LOW");
  }
}

#endif
