// PIN utilisées pour l'alimentation moteur
#define PWMm1 12
#define BI1m1 35
#define BI2m1 34

#define PWMm2 8
#define BI1m2 36
#define BI2m2 37

#define MAXPWM 255 // maximum duty cycle for the PWM is 255/MAXPWM

void InitMotors();

void setRightMotorAVoltage(int value);


void setLeftMotorAVoltage(int value);
