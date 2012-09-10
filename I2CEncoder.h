/*

 */

#ifndef I2CEncoder_h
#define I2CEncoder_h

// The default address for an encoder that's just been turrned on.
#define I2CENCODER_DEFAULT_ADDRESS 0x30

// Registers for important data
#define I2CENCODER_ADDRESS_REGISTER 0x4D
#define I2CENCODER_POSITION_REGISTER 0x40
#define I2CENCODER_VELOCITY_REGISTER 0x44
#define I2CENCODER_ZERO_REGISTER 0x4A
#define I2CENCODER_UNTERMINATE_REGISTER 0x4B
#define I2CENCODER_TERMINATE_REGISTER 0x4C

// The address to assign the first encoder. The addresses after the
// first is this + (n - 1)
#define I2CENCODER_STARTING_ADDRESS 0x10

/**
 * The I2CEncoder class represents the VEX encoders.
 */
class I2CEncoder
{
 private:
  static unsigned char nextAddress;
  static I2CEncoder* lastEncoder;
  bool is_reversed;
  unsigned char address;
  void accessRegister(unsigned char reg);
 public:
  I2CEncoder();
  void init();
  void setReversed(bool is_reversed);
  bool getDirection();
  double getSpeed();
  unsigned int getVelocityBits();
  long getPosition();
  void zero();
  void unTerminate();
  void terminate();
  unsigned char getAddress();
};
#endif

