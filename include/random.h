#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    int m; // μέγιστος αριθμός (modulus)
    int a; // πολλαπλασιαστής
    int c; // σταθερά πρόσθεσης
    int state; // τρέχουσα κατάσταση
} LCG;

void lcg_init(LCG *lcg, int seed);
int lcg_random(LCG *lcg);