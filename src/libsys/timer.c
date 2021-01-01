#include <sys/io.h>
#include <sys/pit.h>
#include <sys/ps2.h>
#include <sys/timer.h>

void SetIntervalTimer(int frequency) {
    int divider;

    // Set the PIT ch0 to the desired frequency
    divider = PIT_CLK_SRC / frequency;
    outb(PIT_MODE_CMD, 0x34); // ch2, lobyte/highbyte, rate generator
    outb(PIT_CH0_DATA, divider);
    outb(PIT_CH0_DATA, divider >> 8);
}

// output a frequency to the built in speaker
void Beep(int frequency) {
    int divider;

    // Set the PIT ch2 to the desired frequency
    divider = PIT_CLK_SRC / frequency;
    outb(PIT_MODE_CMD, 0xb6); // ch2, latch count value, square wave
    outb(PIT_CH2_DATA, divider);
    outb(PIT_CH2_DATA, divider >> 8);
    outb(KB_PORTB, 3);
}

// silence the beep
void NoSound() {
    outb(KB_PORTB, 0);
}


int usleep(unsigned long usec)
{
    unsigned long until = system_time + usec;

    while (system_time < until)
        ;

    return 0;
}
