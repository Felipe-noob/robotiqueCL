// PIN utilisées pour l'alimentation moteur
#define PWM 12
#define BI1 35
#define BI2 34

#define MAXPWM 190 // maximum duty cycle for the PWM is 255/MAXPWM

void InitMotors();

void setMotorAVoltage(int value);
