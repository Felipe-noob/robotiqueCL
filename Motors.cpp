#include "Motors.h"
#include <Arduino.h>
void InitMotors()
{
  // Left Motor
  pinMode(BI1m1, OUTPUT);
  pinMode(BI2m1, OUTPUT);
  pinMode(PWMm1, OUTPUT);

  pinMode(BI1m2, OUTPUT);
  pinMode(BI2m2, OUTPUT);
  pinMode(PWMm2, OUTPUT);
}

// fonction permettant de gerer l'alimentation moteur (sens et amplitude)
void setLeftMotorAVoltage(int valeur)
{
  if (valeur < 0)
  {
    digitalWrite(BI1m1, 1);
    digitalWrite(BI2m1, 0);
  }
  else
  {
    digitalWrite(BI1m1, 0);
    digitalWrite(BI2m1, 1);
  }
  analogWrite(PWMm1, constrain(abs(valeur), 0, MAXPWM));
}
// Function for right motor
void setRightMotorAVoltage(int valeur)
{
  if (valeur < 0)
  {
    digitalWrite(BI1m2, 1);
    digitalWrite(BI2m2, 0);
  }
  else
  {
    digitalWrite(BI1m2, 0);
    digitalWrite(BI2m2, 1);
  }
  analogWrite(PWMm2, constrain(abs(valeur), 0, MAXPWM));
}
