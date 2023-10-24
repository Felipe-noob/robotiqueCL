
#include <Arduino.h>
#include "pid.h"

#define MAXPWM 255 // maximum duty cycle for the PWM is 255/MAXPWM

volatile int offset_prev = 0;
volatile int I = 0;

int pid(int offset, int DT, bool flagCurve) 
{
  // const int deadbandOffset = 40;
  const float K_real = flagCurve ? K_curve : K;
  const float Kp_real = flagCurve ? Kp_curve : Kp;
  const float Ki_real = flagCurve ? Ki_curve : Ki;
  const float Kd_real = flagCurve ? Kd_curve : Kd;

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
  // Serial.print(offset);
  // Serial.print(",");
  // Serial.print(P);
  // Serial.print(",");
  // Serial.println(I);

  offset_prev = offset;
  return K_real * ( P + I + D);
}
