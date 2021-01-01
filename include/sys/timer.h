#include <stdint.h>

extern volatile uint64_t system_time;

void SetIntervalTimer(int frequency);

//Play sound using built in speaker
void Beep(int frequency);

//make it shutup
void NoSound();

int usleep(uint64_t usec);
