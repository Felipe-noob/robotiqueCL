// PIN utilisées pour l'encodeur incrémental
#define PORT1_NE1 31
#define PORT1_NE2 18

void InitEncoders(); // setup encoder position detection

int getPosition1(); // get the actual position of the incremental encoder in pulse.
