#include "pins.h"
#include "move.h"
#include "sensors.h"
bool on = true;
bool print_mode = true;


// PID Constants
float Kp = 25;
float Ki = 0;
float Kd = 5;

float previous_pi = 0;
float pid_value = 0;

float P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;

void setup() {
  // put your setup code here, to run once:
  setup_pins();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_sensors();
  
  if (print_mode){
    for (int i = 0; i < no_of_sensors; i++){
      Serial.print(sensor_readings[i]);
      Serial.print(", ");
    }
    Serial.println(path_error);
  }

  if (Serial.available()){
    String s = Serial.readString();
    if (s == "mf"){
      move_step_forward();
    }else if (s == "ml"){
      move_step_left();
    }else if (s == "ping"){
      Serial.println("HIII");
    }else if (s == "mr"){
      move_step_right();
    } else if (s == "set_th"){
      // set threashold values
    } else if (s == "off"){
      // set threashold values
      on = false;
    } else if (s == "on"){
      // set threashold values
      on = true;
    } else if (s == "cal"){
      // set threashold values
      calibrate_sensors();
    } else if (s == "load_t"){
      load_from_EEPROM();
      Serial.println("Loaded!");
      delay(10000);
    } else if (s == "save_t"){
      save_to_EEPROMM();
      Serial.println("Saved!");
      delay(10000);
    }
  }
  if (!on ) {
    Serial.println(", ");
    return;
  }

  if (path_error == ERROR_SHARP_LEFT){
    // Serial.println("SL");
    // We are in the left side of the track and need to move right to get into the track
    move_step_right_to_track();
    
  } else if (path_error == ERROR_SHARP_RIGHT){
    //Serial.println("SR");
    // We are in the right side of the track and need to move left to get into the track
    move_step_left_to_track();
    
  } else if (path_error == ERROR_STOP){  
    // Stop signal detected.
    stop_vehicle();
    // validate_stop_sign();

  } else {
    calculate_pid();
    PID_value /= 5;
    // Serial.print("  --->  ");
    // Serial.print(PID_value);

    if (PID_value > 30){
    // We are in the right side of the track and need to move left to get into the track
      move_step_left_to_track();
      
    }else if(PID_value < -30){
    // We are in the left side of the track and need to move right to get into the track
      move_step_right_to_track();
      
    }else{
      //PID_value *= 1.5;
      software_pwm_move_forward(default_pwm_value_left + PID_value, default_pwm_value_right - PID_value, 30);
    }
  }
}


/**
PD Controller function
**/
void calculate_pid(){
  P = path_error * Kp;
  D = Kd * (path_error - previous_error);
  PID_value = P + D;
  previous_error = path_error;
}