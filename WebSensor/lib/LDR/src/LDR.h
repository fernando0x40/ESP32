#ifndef LDR_H
#define LDR_H

class LDR
{
private:

  int max_Value;
  int input_Pin;

  void set_Pin(int pin);
  void set_Max_Value(int analogMaxValue);

public:

  LDR();
  LDR(int pin, int analogMaxValue);
  ~LDR();

  float get_Volt_3_3(short unsigned int (*readFunction) (unsigned char));
  float get_Volt_5(short unsigned int (*readFunction) (unsigned char));
  float get_Percent(short unsigned int (*readFunction) (unsigned char));
  int get_Max_Value();
  int get_Input_Pin();
};

#endif
