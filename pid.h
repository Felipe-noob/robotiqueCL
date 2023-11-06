#pragma once
#define K 0.1
#define K_curve 0.4
#define Kp 0.4
#define Kp_curve 0.5
#define Ki 0.1
#define Ki_curve 0.1
#define Kd 0
#define Kd_curve 0.1
// K = 0.18
// Kd = 0.02
// speed 50
#define deadbandOffset 40

int pid(int offset, int DT, bool flagCurve);
