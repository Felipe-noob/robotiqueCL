#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <MeAuriga.h>
#include "MeRGBLineFollower.h"
#include "MeEnhanceEncoderOnBoard.h"

MeEnhanceEncoderOnBoard EnhanceEncoder_1(SLOT1);
MeEnhanceEncoderOnBoard EnhanceEncoder_2(SLOT2);
MeRGBLineFollower RGBLineFollower(PORT_9, ADDRESS2);

int16_t turnoffset = 0;
uint8_t sensorstate;
int16_t set_speed = 0;
uint8_t study_types = 0;

void isr_process_enhance_encoder1(void)
{
    if(digitalRead(EnhanceEncoder_1.getPortB()) == 0){
        EnhanceEncoder_1.pulsePosMinus();
    }else{
        EnhanceEncoder_1.pulsePosPlus();
    }
}
void isr_process_enhance_encoder2(void)
{
    if(digitalRead(EnhanceEncoder_2.getPortB()) == 0){
        EnhanceEncoder_2.pulsePosMinus();
    }else{
        EnhanceEncoder_2.pulsePosPlus();
    }
}

void setup()
{
  //Set Pwm 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  attachInterrupt(EnhanceEncoder_1.getIntNum(), isr_process_enhance_encoder1, RISING);
  attachInterrupt(EnhanceEncoder_2.getIntNum(), isr_process_enhance_encoder2, RISING);

  RGBLineFollower.begin();
  RGBLineFollower.setKp(0.3);
}

void loop()
{
  EnhanceEncoder_1.loop();
  EnhanceEncoder_2.loop();
  RGBLineFollower.loop();

  turnoffset = RGBLineFollower.getPositionOffset();
  
  set_speed = 80;
  EnhanceEncoder_1.setTarPWM(set_speed + turnoffset);
  EnhanceEncoder_2.setTarPWM(-(set_speed - turnoffset));	
}

