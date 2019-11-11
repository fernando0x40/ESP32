#include "LDR.h"

void LDR::set_Pin(int pin)
{
  this->input_Pin = pin;
}
void LDR::set_Max_Value(int analogMaxValue)
{
  this->max_Value = analogMaxValue;
}

LDR::LDR(int pin, int analogMaxValue)
{
  set_Pin(pin);
  set_Max_Value(analogMaxValue);
}

float LDR::get_Volt_3_3(short unsigned int (*readFunction) (unsigned char))
{
  return float(100-(readFunction(this->input_Pin)*3.3/this->max_Value));
}
float LDR::get_Volt_5(short unsigned int (*readFunction) (unsigned char))
{
  return float(100-(readFunction(this->input_Pin)*5/this->max_Value));
}
float LDR::get_Percent(short unsigned int (*readFunction) (unsigned char))
{
  return float(100-(readFunction(this->input_Pin)*100/this->max_Value));
}
int LDR::get_Max_Value()
{
  return this->max_Value;
}
int LDR::get_Input_Pin()
{
  return this->input_Pin;
}
