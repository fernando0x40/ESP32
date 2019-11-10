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

  LDR(int pin, int analogMaxValue);
  ~LDR();

  float get_Volt_3_3(int (*readFunction) (int));
  float get_Volt_5(int (*readFunction) (int));
  float get_Percent(int (*readFunction) (int));
  int get_Max_Value();
  int get_Input_Pin();
};

#endif
