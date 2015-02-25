/*!
LTC2946: 12-Bit Wide Range Power, Charge and Energy Monitor


The LTC®2946 is a rail-to-rail system monitor that measures
current, voltage, power, charge and energy. It features an
operating range of 2.7V to 100V and includes a shunt regulator
for supplies above 100V. The current measurement common mode
range of 0V to 100V is independent of the input supply. 
A 12-bit ADC measures load current, input voltage and an 
auxiliary external voltage. Load current and internally 
calculated power are integrated over an external clock or
crystal or internal oscillator time base for charge and energy.
An accurate time base allows the LTC2946 to provide measurement
accuracy of better than ±0.6% for charge and ±1% for power and
energy. Minimum and maximum values are stored and an overrange
alert with programmable thresholds minimizes the need for software
polling. Data is reported via a standard I2C interface.
Shutdown mode reduces power consumption to 15uA. 
*/


#include <Arduino.h>
#include <stdint.h>
#include "LTC2946.h"
#include <Wire.h>


void LTC2946::init(byte A0, byte A1)
{
	if (A0 == L && A1 == H) I2C_ADDRESS = 0x6C;
	else if (A0 == F && A1 == L) I2C_ADDRESS = 0x6B;
	else if (A0 == H && A1 == F) I2C_ADDRESS = 0x6D;
	else if (A0 == H && A1 == H) I2C_ADDRESS = 0x69;
	else if (A0 == F && A1 == F) I2C_ADDRESS = 0x6A;
	else if (A0 == L && A1 == F) I2C_ADDRESS = 0x6E;
	else if (A0 == H && A1 == L) I2C_ADDRESS = 0x67;
	else if (A0 == F && A1 == H) I2C_ADDRESS = 0x68;
	else if (A0 == L && A1 == L) I2C_ADDRESS = 0x6F;
}

void LTC2946::init(uint8_t i2c_addr)
{
  I2C_ADDRESS = i2c_addr;
}

// Write an 8-bit code to the LTC2946.
int8_t LTC2946::LTC2946_write(uint8_t adc_command, uint8_t code)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int8_t ack;
  
//  ack = i2c_write_byte_data(i2c_address, adc_command, code);

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(adc_command);
  
  Wire.write(code);
  ack = Wire.endTransmission(false);
  
  return ack;
 
}

// Write a 16-bit code to the LTC2946.
int8_t LTC2946::LTC2946_write_16_bits(uint8_t adc_command, uint16_t code)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int8_t ack;
  
  union
  {
    uint8_t b[2];
    uint16_t w;
  } data;
  
  data.w = code;

//  ack = i2c_write_word_data(i2c_address, adc_command, code);

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(adc_command);
  
  Wire.write(data.b[1]);
  Wire.write(data.b[0]);
  ack = Wire.endTransmission(false);

  return(ack);
}

// Write a 24-bit code to the LTC2946.
int8_t LTC2946::LTC2946_write_24_bits(uint8_t adc_command, uint32_t code)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int8_t ack;

  union 
{
  int32_t MY_int32;    //!< 32-bit signed integer to be converted to four bytes
  uint32_t MY_uint32;  //!< 32-bit unsigned integer to be converted to four bytes
  uint8_t MY_byte[4];  //!< 4 bytes (unsigned 8-bit integers) to be converted to a 32-bit signed or unsigned integer
} data;


  data.MY_int32 = code;
    
//  ack = i2c_write_block_data(i2c_address, adc_command, (uint8_t) 3, data.LT_byte);

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(adc_command);
  
  Wire.write(data.MY_byte[2]);
  Wire.write(data.MY_byte[1]);
  Wire.write(data.MY_byte[0]);
  ack = Wire.endTransmission(false);

  return(ack);
}

int8_t LTC2946::LTC2946_write_32_bits(uint8_t adc_command, uint32_t code)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int8_t ack;

  union 
{
  int32_t MY_int32;    //!< 32-bit signed integer to be converted to four bytes
  uint32_t MY_uint32;  //!< 32-bit unsigned integer to be converted to four bytes
  uint8_t MY_byte[4];  //!< 4 bytes (unsigned 8-bit integers) to be converted to a 32-bit signed or unsigned integer
} data;


  data.MY_int32 = code;

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(adc_command);
  
  Wire.write(data.MY_byte[3]);
  Wire.write(data.MY_byte[2]);
  Wire.write(data.MY_byte[1]);
  Wire.write(data.MY_byte[0]);
  ack = Wire.endTransmission(false);
    
 // ack = i2c_write_block_data(i2c_address, adc_command, (uint8_t) 4, data.LT_byte);

  return(ack);
}

// Reads an 8-bit adc_code from LTC2946
int8_t LTC2946::LTC2946_read(uint8_t adc_command, uint8_t *adc_code)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int8_t ack;
  
//  ack = i2c_read_byte_data(i2c_address, adc_command, adc_code);

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(adc_command);
  
  ack = Wire.endTransmission(false);
  
  Wire.requestFrom(I2C_ADDRESS, (uint8_t)1);
  
  *adc_code = Wire.read();
  
  return ack;
}

// Reads a 12-bit adc_code from LTC2946
int8_t LTC2946::LTC2946_read_12_bits(uint8_t adc_command, uint16_t *adc_code)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  // Use union type defined in Linduino.h to combine two uint8_t's (8-bit unsigned integers) into one uint16_t (unsigned 16-bit integer)
  // Then, shift by 4 bits and return in *adc_code
  int8_t ack;
  
  union
  {
    uint8_t b[2];
    uint16_t w;
  } data;
  
  
//  ack = i2c_read_word_data(i2c_address, adc_command, adc_code);

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(adc_command);
  
  ack = Wire.endTransmission(false);
  
  Wire.requestFrom(I2C_ADDRESS, (uint8_t)2);
  
  data.b[1] = Wire.read();
  data.b[0] = Wire.read();
  
  *adc_code = data.w;

  *adc_code >>= 4;
  return ack;
}

// Reads a 16-bit adc_code from LTC2946
int8_t LTC2946::LTC2946_read_16_bits(uint8_t adc_command, uint16_t *adc_code)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int8_t ack;
  
  union
  {
    uint8_t b[2];
    uint16_t w;
  } data;
  
 // ack = i2c_read_word_data(i2c_address, adc_command, adc_code);
 
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(adc_command);
  
  ack = Wire.endTransmission(false);
  
  Wire.requestFrom(I2C_ADDRESS, (uint8_t)2);
  
  data.b[1] = Wire.read();
  data.b[0] = Wire.read();
  
  *adc_code = data.w;
  
  return ack;
}

// Reads a 24-bit adc_code from LTC2946
int8_t LTC2946::LTC2946_read_24_bits(uint8_t adc_command, uint32_t *adc_code)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int8_t ack;
  
union 
{
  int32_t MY_int32;    //!< 32-bit signed integer to be converted to four bytes
  uint32_t MY_uint32;  //!< 32-bit unsigned integer to be converted to four bytes
  uint8_t MY_byte[4];  //!< 4 bytes (unsigned 8-bit integers) to be converted to a 32-bit signed or unsigned integer
} data;
  
 // ack = i2c_read_block_data(i2c_address, adc_command, (uint8_t)3, data.LT_byte);
  
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(adc_command);
  
  ack = Wire.endTransmission(false);
  
  Wire.requestFrom(I2C_ADDRESS, (uint8_t)3);
  
  data.MY_byte[2] = Wire.read();
  data.MY_byte[1] = Wire.read();
  data.MY_byte[0] = Wire.read();
  
  *adc_code = 0x0FFFFFF & data.MY_int32;
  return(ack);
}

// Reads a 32-bit adc_code from LTC2946
int8_t LTC2946::LTC2946_read_32_bits(uint8_t adc_command, uint32_t *adc_code)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int8_t ack;
  
union 
{
  int32_t MY_int32;    //!< 32-bit signed integer to be converted to four bytes
  uint32_t MY_uint32;  //!< 32-bit unsigned integer to be converted to four bytes
  uint8_t MY_byte[4];  //!< 4 bytes (unsigned 8-bit integers) to be converted to a 32-bit signed or unsigned integer
} data;  

 // ack = i2c_read_block_data(i2c_address, adc_command, (uint8_t) 4, data.LT_byte);
  
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(adc_command);
  
  ack = Wire.endTransmission(false);
  
  Wire.requestFrom(I2C_ADDRESS, (uint8_t)4);
  
  data.MY_byte[3] = Wire.read();
  data.MY_byte[2] = Wire.read();
  data.MY_byte[1] = Wire.read();
  data.MY_byte[0] = Wire.read();
  
  *adc_code = 0xFFFFFFFF & data.MY_int32;
  return(ack);
}

// Calculate the LTC2946 VIN voltage
float LTC2946::LTC2946_VIN_code_to_voltage(uint16_t adc_code, float LTC2946_VIN_lsb)
// Returns the VIN Voltage in Volts
{
  float voltage;
  voltage = (float)adc_code*LTC2946_VIN_lsb;    //! 1) Calculate voltage from code and lsb
  return(voltage);
}

// Calculate the LTC2946 ADIN voltage
float LTC2946::LTC2946_ADIN_code_to_voltage(uint16_t adc_code, float LTC2946_ADIN_lsb)
// Returns the ADIN Voltage in Volts
{
  float adc_voltage;
  adc_voltage = (float)adc_code*LTC2946_ADIN_lsb;   //! 1) Calculate voltage from code and ADIN lsb
  return(adc_voltage);
}

// Calculate the LTC2946 current with a sense resistor
float LTC2946::LTC2946_code_to_current(uint16_t adc_code, float resistor, float LTC2946_DELTA_SENSE_lsb)
// Returns the LTC2946 current in Amps
{
  float voltage, current;
  voltage = (float)adc_code*LTC2946_DELTA_SENSE_lsb;    //! 1) Calculate voltage from ADC code and delta sense lsb
  current = voltage/resistor;                           //! 2) Calculate current, I = V/R
  return(current);
}

// Calculate the LTC2946 power
float LTC2946::LTC2946_code_to_power(int32_t adc_code, float resistor, float LTC2946_Power_lsb)
// Returns The LTC2946 power in Watts
{
  float power;
  power = (float)adc_code*LTC2946_Power_lsb/resistor;  //! 1) Calculate Power using Power lsb and resistor
     
  return(power);
}


// Calculate the LTC2946 energy
float LTC2946::LTC2946_code_to_energy(int32_t adc_code,float resistor, float LTC2946_Power_lsb, float LTC2946_TIME_lsb)
// Returns the LTC2946 energy in Joules
{
  float energy_lsb, energy;
  energy_lsb=(float)(LTC2946_Power_lsb/resistor)*65536*LTC2946_TIME_lsb;   //! 1) Calculate Energy lsb from Power lsb and Time lsb
  energy = adc_code*energy_lsb;                               //! 2) Calculate Energy using Energy lsb and adc code 
  return(energy);
}

// Calculate the LTC2946 Coulombs
float LTC2946::LTC2946_code_to_coulombs(int32_t adc_code, float resistor, float LTC2946_DELTA_SENSE_lsb, float LTC2946_TIME_lsb)
// Returns the LTC2946 Coulombs
{
  float coulomb_lsb, coulombs;
  coulomb_lsb=(float)(LTC2946_DELTA_SENSE_lsb/resistor)*16*LTC2946_TIME_lsb;   //! 1) Calculate Coulomb lsb Current lsb and Time lsb
  coulombs = adc_code*coulomb_lsb;                                             //! 2) Calculate Coulombs using Coulomb lsb and adc code 
  return(coulombs);
}

//Calculate the LTC2946 Time in Seconds
float LTC2946::LTC2946_code_to_time(float time_code, float LTC2946_TIME_lsb)
{
  float seconds;
  seconds = LTC2946_TIME_lsb * time_code;
  return seconds;
}

