#include "Encoders.h"

volatile int position1=0;

int getPosition1()
{
  noInterrupts();
  int temp = position1;
  interrupts();
  return temp;
}

void InitEncoders()
{ 
  pinMode(PORT1_NE2,INPUT_PULLUP);
  pinMode(PORT1_NE1,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PORT1_NE2),ISR_encoder1,RISING);
}


// fonction appelée lors des interruptions 'front montant'
void ISR_encoder1(){
  if(digitalRead(PORT1_NE1)) // detection du sens de rotation
    position1++;
    else
    position1--;
}
