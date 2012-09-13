/**
 * This code measures the average speed in feet per second of a
 * robot. It assumes that the robot uses tank drive (differential
 * steering) and drives 8 inches (2/3 ft) per rotation of the ouput
 * shaft of the 269 motor driving it.
 */

#include <Wire.h>
#include <I2CEncoder.h>

// Create an I2CEncoder for measuring the speed
I2CEncoder leftEncoder;
I2CEncoder rightEncoder;

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  
  // Initialize the encoders for a 269 motors that are moving 2/3 of a
  // foot per motor output shaft rotation.
  leftEncoder.init((2.0/3.0)*MOTOR_269_ROTATIONS, MOTOR_269_TIME_DELTA);
  rightEncoder.init((2.0/3.0)*MOTOR_269_ROTATIONS, MOTOR_269_TIME_DELTA);
  rightEncoder.setReversed(true);
}

void loop() {
  // Calculate the average speed of the robot in feet per second and print it.
  double speed = ((leftEncoder.getSpeed() + rightEncoder.getSpeed()) / 2.0) / 60.0;
  Serial.print(speed);
  Serial.println(" feet per second.");
}
