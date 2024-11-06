#include "random.h"

void lcg_init(LCG *lcg, int seed){
    lcg->state = seed % lcg->m; //seed mod MAX
}

int lcg_random(LCG *lcg){
    //next=(a × seed + c ) mod m
    lcg->state = (lcg->a * lcg->state + lcg->c) % lcg->m; 
    return lcg->state;
};