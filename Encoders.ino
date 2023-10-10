#include "Encoders.h"

volatile int position1 = 0;
volatile int position2 = 0;

int getPosition1()
{
  noInterrupts();
  int temp = position1;
  interrupts();
  return temp;
}

int getPosition2()
{
  noInterrupts();
  int temp = position2;
  interrupts();
  return temp;
}

void InitEncoders()
{
  // Left motor
  pinMode(PORT1_NE2, INPUT_PULLUP);
  pinMode(PORT1_NE1, INPUT_PULLUP);

  // Right motor
  pinMode(PORT2_NE2, INPUT_PULLUP);
  pinMode(PORT2_NE1, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PORT1_NE2), ISR_encoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(PORT2_NE2), ISR_encoder2, RISING);
}

// fonction appelée lors des interruptions 'front montant'
void ISR_encoder1()
{
  if (digitalRead(PORT1_NE1)) // detection du sens de rotation
    position1++;
  else
    position1--;
}

// fonction appelée lors des interruptions 'front montant'
void ISR_encoder2()
{
  if (digitalRead(PORT2_NE1)) // detection du sens de rotation
    position2++;
  else
    position2--;
}