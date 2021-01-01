#include <time.h>
#include <sys/io.h>
#include <sys/rtc.h>

#define CMOS_ADDR       0x70
#define CMOS_DATA       0x71

static int RTC_updateInProgress()
{
    outb(CMOS_ADDR, 0x0A);
    return (inb(CMOS_DATA) & 0x80);
}

static unsigned char RTC_read(int reg)
{
    outb(CMOS_ADDR, reg);
    return inb(CMOS_DATA);
}

static void RTC_readTime(struct time *now)
{
    while (RTC_updateInProgress())
        ; // Make sure an update isn't in progress

    now->second = RTC_read(0x00);
    now->minute = RTC_read(0x02);
    now->hour = RTC_read(0x04);
    now->day = RTC_read(0x07);
    now->month = RTC_read(0x08);
    now->year = RTC_read(0x09);
}

void RTC_GetTime(struct time *now)
{
    struct time last;
    unsigned char status_reg_B;

    // Note: This uses the `read registers until you get the same values twice
    //       in a row` technique to avoid getting dodgy/inconsistent values due
    //       to RTC updates

    RTC_readTime(now);
    do {
        last = *now;
        RTC_readTime(now);
    } while (now->second != last.second || now->minute != last.minute ||
             now->hour != last.hour || now->day != last.day ||
             now->month != last.month || now->year != last.year);

    status_reg_B = RTC_read(0x0B);

    // Convert BCD to binary values if necessary
    if (!(status_reg_B & 0x04)) {
        now->second = (now->second & 0x0F) + ((now->second / 16) * 10);
        now->minute = (now->minute & 0x0F) + ((now->minute / 16) * 10);
        now->hour = ((now->hour & 0x0F) + (((now->hour & 0x70) / 16) * 10)) |
                    (now->hour & 0x80);
        now->day = (now->day & 0x0F) + ((now->day / 16) * 10);
        now->month = (now->month & 0x0F) + ((now->month / 16) * 10);
        now->year = (now->year & 0x0F) + ((now->year / 16) * 10);
    }

    // Convert 12 hour clock to 24 hour clock if necessary
    if (!(status_reg_B & 0x02) && (now->hour & 0x80)) {
        now->hour = ((now->hour & 0x7F) + 12) % 24;
    }
}
