#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

#define no_of_sensors 5

const int sensor_pins[] = {A0, A1, A2, A3, A4};
const int motor_left_fw = 6;
const int motor_left_bw = 5;
const int motor_right_fw = 4;
const int motor_right_bw = 3;
const int motor_left_pwm = 9;
const int motor_right_pwm = 10;



void setup_pins(){
  pinMode(motor_left_fw, OUTPUT);
  pinMode(motor_left_bw, OUTPUT);
  pinMode(motor_right_fw, OUTPUT);
  pinMode(motor_right_bw, OUTPUT);
  pinMode(motor_right_pwm, OUTPUT);
  pinMode(motor_left_pwm, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  /*
  No need to setup input modes for analog read
  for (int i = 0; i < no_of_sensors; i++){
    pinMode(sensor_pins[i], INPUT);
  }*/
}

#endif