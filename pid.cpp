#define K 1
#define Kp 1
#define Ki 0
#define Kd 0

volatile int offset_prev = 0;
volatile int I = 0;

int pid(int offset, int DT) 
{
  const int P = Kp * offset;
  I += Ki * (offset + offset_prev) * DT /2;
  // TODO: is this division too slow?
  const int D = Kd * (offset - offset_prev) / DT;

  offset_prev = offset;
  return K * ( P + I + D);
}