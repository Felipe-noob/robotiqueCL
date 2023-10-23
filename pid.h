#define K 0.18
#define Kp 0.5
#define Ki 0.1
#define Kd 0.02

#define deadbandOffset 40

int pid(int offset, int DT);