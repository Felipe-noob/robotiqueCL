
#include <Arduino.h>

#define K 0.5
#define Kp 1
#define Ki 1
#define Kd 0.5

#define MAXPWM 255 // maximum duty cycle for the PWM is 255/MAXPWM


volatile int offset_prev = 0;
volatile int I = 0;

int pid(int offset, int DT) 
{
  const int P = Kp * offset;
  I += Ki * (offset + offset_prev) * DT / 2000;
  // antiwindup
  if (I > MAXPWM) {
    I = MAXPWM;
  } else if(I < -MAXPWM) {
    I = -MAXPWM;
  }
  
  // TODO: is this division too slow?
  const int D = 1000 * Kd * (offset - offset_prev) / DT;

  offset_prev = offset;

  Serial.print(offset);
  Serial.print(",");
  Serial.print(P);
  Serial.print(",");
  Serial.print(I);
  Serial.print(",");
  Serial.print(D);
  Serial.print(",");
  Serial.print(K * ( P + I + D));
  Serial.print("\n");

  return K * ( P + I + D);
}