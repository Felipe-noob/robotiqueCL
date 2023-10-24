
#include <Arduino.h>
#include "pid.h"
// #define K 0.5
// #define Kp 1
// #define Ki 1
// #define Kd 0.5

#define MAXPWM 255 // maximum duty cycle for the PWM is 255/MAXPWM


volatile int offset_prev = 0;
volatile int I = 0;

int pid(int offset, int DT, bool flagCurve) 
{
  // const int deadbandOffset = 40;
  const int K_real = flagCurve ? 0.5 : K;
  const int Kp_real = flagCurve ? 0.5 : Kp;
  const int Ki_real = flagCurve ? 0.05 : Ki;
  const int Kd_real = flagCurve ? 0 : Kd;

  const int P = Kp_real * offset;

  if (abs(offset) <= deadbandOffset) {
    offset = 0;
  }
  I += Ki_real * (offset + offset_prev) * DT / 2000;
  // antiwindup
  if (I > MAXPWM) {
    I = MAXPWM;
  } else if(I < -MAXPWM) {
    I = -MAXPWM;
  }
  
  // TODO: is this division too slow?
  const int D = 1000 * Kd_real * (offset - offset_prev) / DT;

  offset_prev = offset;

  return K_real * ( P + I + D);
}
