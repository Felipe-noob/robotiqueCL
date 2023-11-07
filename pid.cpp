#include <Arduino.h>
#include "pid.h"

#define MAXPWM 255 // maximum duty cycle for the PWM is 255/MAXPWM

volatile int offset_prev = 0;
volatile int I = 0;

int pid(int offset, int DT, float K, float Kp, float Ki, float Kd) 
{
  const int P = Kp * offset;
  /*
  if (abs(offset) <= deadbandOffset) {
    offset = 0;
  }
  */
  I += Ki * (offset + offset_prev) * DT / 2000;
  // antiwindup
  if (I > MAXPWM) {
    I = MAXPWM;
  } else if(I < -MAXPWM) {
    I = -MAXPWM;
  }
  
  const int D = 1000 * Kd * (offset - offset_prev) / DT;

  offset_prev = offset;
  return K * ( P + I + D );
}
