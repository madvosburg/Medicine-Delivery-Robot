

typedef struct{
    float sound_dist;  // PING sensor
    int IRval;    // IR sensor
} Scan_t;


void init_scan();

int IRconversion(double irInput);

//holdFlag = 1 for hold at position
void execute_scan(int angle, Scan_t* scan, int holdFlag);


