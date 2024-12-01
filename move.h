#include "Arduino.h"
#ifndef MOVE_H
#define MOVE_H


#include "pins.h"
#include "sensors.h"

uint8_t move_delay = 20;
int default_pwm_value_left = 210;
int default_pwm_value_right = 180;

void stop_vehicle(){
  digitalWrite(motor_left_fw, LOW);
  digitalWrite(motor_left_bw, LOW);
  digitalWrite(motor_right_fw, LOW);
  digitalWrite(motor_right_bw, LOW);
}

void move_step_forward(){
  digitalWrite(motor_left_fw, HIGH);
  digitalWrite(motor_left_bw, LOW);
  digitalWrite(motor_right_fw, HIGH);
  digitalWrite(motor_right_bw, LOW);
  delay(move_delay);
  stop_vehicle();
}

void move_step_backward(){
  digitalWrite(motor_left_fw, LOW);
  digitalWrite(motor_left_bw, HIGH);
  digitalWrite(motor_right_fw, LOW);
  digitalWrite(motor_right_bw, HIGH);
  delay(move_delay);
  stop_vehicle();
}

void software_pwm_move_forward(uint8_t left, uint8_t right, long time) {
  // pre calculations
  analogWrite(motor_left_pwm, left);
  analogWrite(motor_right_pwm, right);
  digitalWrite(motor_left_fw, HIGH);
  digitalWrite(motor_right_fw, HIGH);
  delay(time);
}

void move_step_left(){
  analogWrite(motor_left_pwm, default_pwm_value_left);
  analogWrite(motor_right_pwm, default_pwm_value_right);

  digitalWrite(motor_left_fw, LOW);
  digitalWrite(motor_left_bw, HIGH);
  digitalWrite(motor_right_fw, HIGH);
  digitalWrite(motor_right_bw, LOW);
  delay(move_delay);
  stop_vehicle();
}

void move_step_right(){
  analogWrite(motor_left_pwm, default_pwm_value_left);
  analogWrite(motor_right_pwm, default_pwm_value_right);
  
  digitalWrite(motor_left_fw, HIGH);
  digitalWrite(motor_left_bw, LOW);
  digitalWrite(motor_right_fw, LOW);
  digitalWrite(motor_right_bw, HIGH);
  delay(move_delay);
  stop_vehicle();
}

/**
When robot is in the extreme left on the track, get it back to middle
*/
void move_step_right_to_track(){
  analogWrite(motor_left_pwm, default_pwm_value_left);
  analogWrite(motor_right_pwm, default_pwm_value_right);

  digitalWrite(motor_left_fw, LOW);
  digitalWrite(motor_left_bw, HIGH);
  digitalWrite(motor_right_fw, HIGH);
  digitalWrite(motor_right_bw, LOW);

  read_sensors();
  delay(30);

  /*while (true || (path_error > 0 && path_error != ERROR_STOP)){
    read_sensors();
    delay(30);
    break;
  }*/
  stop_vehicle();
}

/**
When the robot is in extreme right on the track, get it back to middle
*/
void move_step_left_to_track(){
  analogWrite(motor_left_pwm, default_pwm_value_left);
  analogWrite(motor_right_pwm, default_pwm_value_right);
  
  digitalWrite(motor_left_fw, HIGH);
  digitalWrite(motor_left_bw, LOW);
  digitalWrite(motor_right_fw, LOW);
  digitalWrite(motor_right_bw, HIGH);

  read_sensors();
  delay(30);

  /*while (true || (path_error > 0 && path_error != ERROR_STOP)){
    read_sensors();
    delay(30);
    break;
  }*/
  stop_vehicle();
}


/**
When detect a cross path, or stop sign, validate weather a stop sign or not
*/
void validate_stop_sign(){
  stop_vehicle(); // stop the vehicle first
  delay(100);

  // move step forward and check
  move_step_forward();
  delay(100);
  read_sensors();
  if (path_error == ERROR_STOP || path_error == ERROR_NO_PATH) {
    // came to an end
    // stop_vehicle();
    // wait for reset
    // NOTE: Inf loop
    while (1){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(300);
      digitalWrite(LED_BUILTIN, LOW);
      delay(300);
    }
  }
  return;

  move_step_backward();
  move_step_backward();
  delay(100);

  read_sensors();

  if (path_error == ERROR_STOP || path_error == ERROR_NO_PATH) {
    // came to an end
    // stop_vehicle();
    // wait for reset
    // NOTE: Inf loop
    while (1){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(300);
      digitalWrite(LED_BUILTIN, LOW);
      delay(300);
    }
  }
  move_step_forward();
}

#endif