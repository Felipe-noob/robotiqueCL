#include "Motors.h"

#include "MeRGBLineFollower.h"

void InitMotors()
{
 pinMode(BI1,OUTPUT);
 pinMode(BI2,OUTPUT);
 pinMode(PWM,OUTPUT);

}


// fonction permettant de gerer l'alimentation moteur (sens et amplitude)
void setMotorAVoltage(int valeur)
{
  if(valeur<0)
  {
    digitalWrite(BI1,1);
    digitalWrite(BI2,0);
  }
  else
   {
    digitalWrite(BI1,0);
    digitalWrite(BI2,1);
  }
  analogWrite(PWM,constrain(abs(valeur),0,MAXPWM));
}
