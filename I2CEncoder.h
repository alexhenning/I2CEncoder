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

/**** CONVERSION RATES ****/
//// 269 Motor:
// Encoder Revolutions to Output Rotations
// 1 (Output Rotation) / 30.056 (Encoder Revolution)
#define MOTOR_269_ROTATIONS 0.03327122704

// Time-delta to Minutes
// 64 microseconds  (per revolution)
// 64 microseconds * 1s/10^6microseconds * 1 minute/60s
#define MOTOR_269_TIME_DELTA 0.00000106666

//// 393 Motor configured for torque:
// Encoder Revolutions to Output Rotations
// 1 (Output Rotation) / 39.2 (Encoder Revolution)
#define MOTOR_393_TORQUE_ROTATIONS 0.02551020408	

// 393 Motor configured for speed: Ticks to Rotations
// Encoder Revolutions to Output Rotations
// 1 (Output Rotation) / 24.5 (Encoder Revolution)
#define MOTOR_393_SPEED_ROTATIONS 0.04081632653

// 393 Motor configured for Turbo speed: Ticks to Rotations
// Encoder Revolutions to Output Rotations
// 1 (Output Rotation) / 16.3 (Encoder Revolution)
#define MOTOR_393_TURBO_ROTATIONS 0.06134969325

// Time-delta to Minutes
// 128 microseconds (per revolution)
// 128 microseconds * 1s/10^6microseconds * 1 minute/60s
#define MOTOR_393_TIME_DELTA 0.00000213333

// The default number of ticks per encoder revolution
#define TICKS 8

/**
 * The I2CEncoder class represents the VEX encoders.
 */
class I2CEncoder
{
 private:
  static unsigned char nextAddress;
  static I2CEncoder* lastEncoder;
  bool is_reversed;
  float rotation_factor;
  float time_delta;
  int ticks;
  int address;
  void accessRegister(unsigned char reg);
 public:
  I2CEncoder();
  void init(double rotation_factor, double time_delta);
  void init(double rotation_factor, double time_delta, int ticks);
  void setReversed(bool is_reversed);
  double getSpeed();
  unsigned int getVelocityBits();
  double getPosition();
  long getRawPosition();
  void zero();
  void unTerminate();
  void terminate();
  int getAddress();
};
#endif

