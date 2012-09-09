
#include "Wire.h"
#include "I2CEncoder.h"

// INITIALIZE
unsigned char I2CEncoder::nextAddress = I2CENCODER_STARTING_ADDRESS;
I2CEncoder* I2CEncoder::lastEncoder = NULL;

// CONSTRUCTOR
I2CEncoder::I2CEncoder() {
  // TODO: Handle last encoder nicely:  lastEncoder = this;
  address = nextAddress;
  nextAddress++;
  is_reversed = false;
}

// PUBLIC METHODS

/*
 * Initialize the encoder to it's address.
 */
void I2CEncoder::init() {
  // Unterminates previous encoder so that messages flow to this one.
  if (lastEncoder) {
    lastEncoder->unTerminate();
  }
  lastEncoder = this;

  // Assign it's address
  Wire.beginTransmission(I2CENCODER_DEFAULT_ADDRESS);
  Wire.write(I2CENCODER_ADDRESS_REGISTER);
  Wire.write(address << 1);
  Wire.endTransmission();

  // Zero it on initialization
  zero();
}

/**
 * Sets whether or not the encoder is setup "backwards".
 */
void I2CEncoder::setReversed(bool is_reversed) {
  this->is_reversed = is_reversed;
}

bool I2CEncoder::getDirection() {
  accessRegister(0x3E);
  Wire.requestFrom(address, 1);
  return (bool) (Wire.read() >> 7);
  // TODO: Implement. Should handle reversed
}

/**
 * Returns the speed of rotation of the encoder
 */
double I2CEncoder::getSpeed() {
  // TODO: Check sanity of the values
  double vb = double(getVelocityBits());
  double st = vb * 0.000064; // Seconds/Rev
  double speed = (60.0/st)/30.056; // RPM
  return speed < 0.49 ? 0 : speed;
}

/**
 * Returns the unsigned velocity bits. This is the time-delta between
 * ticks in multiples of 64 microseconds/tick.
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
 * Returns the position in encoder ticks since power on.
 */
long I2CEncoder::getPosition() {
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

void I2CEncoder::zero() {
  accessRegister(I2CENCODER_ZERO_REGISTER);
}

void I2CEncoder::unTerminate() {
  accessRegister(I2CENCODER_UNTERMINATE_REGISTER);
}

void I2CEncoder::terminate() {
  accessRegister(I2CENCODER_TERMINATE_REGISTER);
}

unsigned char I2CEncoder::getAddress() {
  return address;
}

// Private Functions
void I2CEncoder::accessRegister(unsigned char reg) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
}
