#include <stdlib.h>
#include <time.h>
#include "sensor.h"

float get_temperature() {
    srand((unsigned int)time(NULL));
    return ((float)rand()/(float)(RAND_MAX)) * 30;
}