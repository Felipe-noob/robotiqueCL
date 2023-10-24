#define K 0.18
#define Kp 0.4
#define Ki 0.1
#define Kd 0.02
// K = 0.18
// Kd = 0.02
// speed 50
#define deadbandOffset 40

int pid(int offset, int DT, bool flagCurve);