#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <EEPROM.h>
#include "pins.h"

// defined in robot_linef.ino
extern bool on;
extern bool print_mode;

const int EEPROM_START_ADDR = 100; // Starting address in EEPROM
#define CALIBRATION_LENGTH 3000

#define ERROR_NO_PATH 102
#define ERROR_STOP 103
#define ERROR_SHARP_LEFT 104
#define ERROR_SHARP_RIGHT 105


int sensor_readings_raw[no_of_sensors];
bool sensor_readings[no_of_sensors];
int sensor_threshold[no_of_sensors] = {505, 503, 520, 367, 503}; //567, 567, 564, 424, 607,
int path_error = 0;//                                              535, 552, 534, 422, 519,   --|


void read_sensors(){
  uint8_t reading_x = 0;
  for (int i = 0; i < no_of_sensors; i++){
    sensor_readings_raw[i] = analogRead(sensor_pins[i]);
    sensor_readings[i] = (bool)(sensor_readings_raw[i] > sensor_threshold[i]);
    reading_x |= (sensor_readings[i] << i);
  }

  // calculating error
  if (reading_x == 0b00000100 || reading_x == 0b00001110){
    path_error = 0;
  } else if (reading_x == 0b00000110){
    path_error = 3;
  } else if (reading_x == 0b00000010){
    path_error = 6;
  } else if (reading_x == 0b00000011){
    path_error = 8;
  } else if (reading_x == 0b00000001){
    path_error = ERROR_SHARP_RIGHT;
  } else if (reading_x == 0b00001100){
    path_error = -3;
  } else if (reading_x == 0b00001000){
    path_error = -6;
  } else if (reading_x == 0b00011000){
    path_error = -8;
  } else if (reading_x == 0b00010000){
    path_error = ERROR_SHARP_LEFT;
  } else if (reading_x == 0b00011111){
    path_error = ERROR_STOP;
  } else if (reading_x == 0b00000000){
    path_error = ERROR_NO_PATH;
  }
}


void calibrate_sensors(){
  Serial.println("Calibration Process Started!");
  int tmp = 0;

  for (int i = 0; i < 5; i++){
    Serial.println("Sensor " + String(i+1) + "Calibration");
    for (int j = 0; j < i; j++){
      Serial.print(" O ");
    }
    Serial.print(" X ");
    for (int j = i+1; j < 5; j++){
      Serial.print(" O ");
    }
    Serial.println("Put the sesnor on white. Send anything to start.");
    while (!Serial.available()){
      //wait
      delay(1);
    }
    Serial.readString();
    Serial.println("Calibrating...");
    // take 1000 white mesurements
    long black_mean = 0;
    long white_mean = 0;
    for (int j = 0; j < CALIBRATION_LENGTH; j++){
      white_mean += analogRead(sensor_pins[i]);
      delay(2);
    }
    Serial.println("Put the sesnor on black. Send anything to start.");
    while (!Serial.available()){
      //wait
      delay(1);
    }
    Serial.readString();
    Serial.println("Calibrating...");
    for (int j = 0; j < CALIBRATION_LENGTH; j++){
      //measures[j] = analogRead(sensor_pins[i]);
      black_mean += analogRead(sensor_pins[i]);
      delay(2);
    }

    
    black_mean /= CALIBRATION_LENGTH;
    white_mean /= CALIBRATION_LENGTH;
    int mid = (white_mean + black_mean) / 2;

    Serial.print("Black Mean: ");
    Serial.println(black_mean);
    Serial.print("White Mean: ");
    Serial.println(white_mean);
    Serial.print("Middle:     ");
    Serial.println(mid);
    sensor_threshold[i] = mid;
    Serial.println("");
  }
  // we take 300 readings per sensor
  //int sensor_readings

}

void load_from_EEPROM(){
  // load threashold values int sensor_threshold[no_of_sensors] 
  for (int i = 0; i < no_of_sensors; ++i) {
    int address = EEPROM_START_ADDR + i * sizeof(int);
    EEPROM.get(address, sensor_threshold[i]);
    Serial.print(sensor_threshold[i]);
    Serial.print(", ");
  }
  Serial.println("  --|");
}

void save_to_EEPROMM(){
  // save the threashold values from int sensor_threshold[no_of_sensors] 
  for (int i = 0; i < no_of_sensors; ++i) {
    int address = EEPROM_START_ADDR + i * sizeof(int);
    EEPROM.put(address, sensor_threshold[i]);
    Serial.print(sensor_threshold[i]);
    Serial.print(", ");
  }
  Serial.println("  --|");
}
#endif