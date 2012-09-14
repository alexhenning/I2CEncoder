
#include "Wire.h"
#include "Arduino.h"

#include "I2CEncoder.h"

// INITIALIZE
unsigned char I2CEncoder::nextAddress = I2CENCODER_STARTING_ADDRESS;
I2CEncoder* I2CEncoder::lastEncoder = NULL;

// CONSTRUCTOR
/**
 * Create the encoder with it's automatically assigned address.
 */
I2CEncoder::I2CEncoder() {
  address = nextAddress;
  nextAddress++;
  is_reversed = false;
  rotation_factor = 0;
  time_delta = 0;
  ticks = 0;
}

// PUBLIC METHODS

/*
 * Initialize the encoder to it's address.
 */
void I2CEncoder::init(double rotation_factor, double time_delta, int ticks) {
  // Unterminates previous encoder so that messages flow to this one.
  if (lastEncoder) {
    lastEncoder->unTerminate();
  }
  lastEncoder = this;
  
  this->rotation_factor = rotation_factor;
  this->time_delta = time_delta;
  this->ticks = ticks;

  // Assign it's address
  Wire.beginTransmission(I2CENCODER_DEFAULT_ADDRESS);
  Wire.write(I2CENCODER_ADDRESS_REGISTER);
  Wire.write(address << 1); // Shift to an 8-bit address for the encoder.
  Wire.endTransmission();

  // Zero it on initialization
  zero();
}
void I2CEncoder::init(double rotation_factor, double time_delta) {
  init(rotation_factor, time_delta, TICKS);
}

/**
 * Sets whether or not the encoder is setup "backwards" or flipped.
 */
void I2CEncoder::setReversed(bool is_reversed) {
  this->is_reversed = is_reversed;
}

/**
 * Returns the speed of the encoder rotation per minute for the output
 * shaft of the motor. (Assumes 269)
 */
double I2CEncoder::getSpeed() {
  // TODO: Check sanity of the values
  unsigned int vb = getVelocityBits();
  if (vb == 0xFFFF) return 0;
  return rotation_factor / (double(vb) * time_delta);
}

/**
 * Returns the unsigned velocity bits. This is the time-delta between
 * ticks in multiples of 64 microseconds/tick. Stopped is 0xFFFF or 4
 * seconds. (Assumes 269)
 */
unsigned int I2CEncoder::getVelocityBits() {
  accessRegister(I2CENCODER_VELOCITY_REGISTER);
  Wire.requestFrom(address, 2);

  unsigned int speed = 0;
  speed |= Wire.read() << 8;
  speed |= Wire.read();
  return speed;
}


/**
 * Returns the position in rotations since power on or last reset.
 */
double I2CEncoder::getPosition() {
  return rotation_factor / ((double) ticks) * ((double) getRawPosition());
}

/**
 * Returns the position in encoder ticks since power on or last reset.
 */
long I2CEncoder::getRawPosition() {
  // TODO: Deal with the two extra bytes
  accessRegister(I2CENCODER_POSITION_REGISTER);
  Wire.requestFrom(address, 4);

  long position = 0;
  position |= Wire.read() << 8;
  position |= Wire.read();
  position |= Wire.read() << 24;
  position |= Wire.read() << 16;

  return is_reversed ? -position : position;
}

/**
 * Zero the position of this encoder.
 */
void I2CEncoder::zero() {
  accessRegister(I2CENCODER_ZERO_REGISTER);
}

/**
 * UnTerminate this encoder. This allows access to all I2C devices
 * after this encoder.
 */
void I2CEncoder::unTerminate() {
  accessRegister(I2CENCODER_UNTERMINATE_REGISTER);
}

/**
 * Terminate this encoder. This prevents access to all I2C devices
 * after this encoder.
 */
void I2CEncoder::terminate() {
  accessRegister(I2CENCODER_TERMINATE_REGISTER);
}

/**
 * Gets the I2C Address of this encoder for manual access.
 */
int I2CEncoder::getAddress() {
  return address;
}

// Private Functions
void I2CEncoder::accessRegister(unsigned char reg) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
}
