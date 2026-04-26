#include "datetime.h"
#include "../cmos.h"
#include "../text.h"
#include "../string.h"
#include "../arch/i686/keyboard.h"

static datetime_t current_time = {
    .hour = 0,
    .minute = 0,
    .day = 1,
    .month = 1,
    .year = 2026
};

static int parse_int(const char* str, int* i)
{
    int val = 0;
    int found = 0;

    while (str[*i] >= '0' && str[*i] <= '9')
    {
        val = val * 10 + (str[*i] - '0');
        (*i)++;
        found = 1;
    }

    return found ? val : -1;
}

void datetime_prompt()
{
    char* input;
    int i;

    text_print("[{0x08}DATETIME{0x0F}]    Enter new time (HH:MM DD-MM-YYYY)\n");
    text_print("[{0x08}DATETIME{0x0F}]    Press ENTER to keep current\n");

    while (1)
    {
        text_print("[{0x08}DATETIME{0x0F}]    Timedate> ");

        input = keyboard_get_line();

        // SKIP use CMOS time
        if (input[0] == 0)
        {
            cmos_time_t rtc;
            cmos_read_rtc(&rtc);

            current_time.hour   = rtc.hour;
            current_time.minute = rtc.minute;
            current_time.day    = rtc.day;
            current_time.month  = rtc.month;
            current_time.year   = rtc.year;

            text_print("[{0x08}DATETIME{0x0F}]    Using CMOS RTC time.\n\n");
            return;
        }

        i = 0;

        int h = parse_int(input, &i);
        if (h < 0 || input[i] != ':')
        {
            text_print("[{0x08}DATETIME{0x0F}]    Invalid time format\n");
            continue;
        }
        i++;

        int m = parse_int(input, &i);
        if (m < 0 || input[i] != ' ')
        {
            text_print("[{0x08}DATETIME{0x0F}]    Invalid time format\n");
            continue;
        }
        i++;

        int d = parse_int(input, &i);
        if (d < 0 || input[i] != '-')
        {
            text_print("[{0x08}DATETIME{0x0F}]    Invalid date format\n");
            continue;
        }
        i++;

        int mo = parse_int(input, &i);
        if (mo < 0 || input[i] != '-')
        {
            text_print("[{0x08}DATETIME{0x0F}]    Invalid date format\n");
            continue;
        }
        i++;

        int y = parse_int(input, &i);
        if (y < 0)
        {
            text_print("[{0x08}DATETIME{0x0F}]    Invalid year format\n");
            continue;
        }

        // checks
        if (h > 23 || m > 59)
        {
            text_print("[{0x08}DATETIME{0x0F}]    Haven't they taught you in school that 1 hour = 60 mins? Invalid.\n");
            continue;
        }

        if (mo < 1 || mo > 12)
        {
            text_print("[{0x08}DATETIME{0x0F}]    That's not how months work. Invalid.\n");
            continue;
        }

        if (y < 1)
        {
            text_print("[{0x08}DATETIME{0x0F}]    What year are you in bro?\n");
            continue;
        }

        if (y < 1971)
        {
            text_print("[{0x08}DATETIME{0x0F}]    You really love UNIX, don't you?\n");
            continue;
        }

        if (y < 2026)
        {
            text_print("[{0x08}DATETIME{0x0F}]    I'm gonna pretend I believe you.\n");
            continue;
        }

        if (y > 3000)
        {
            text_print("[{0x08}DATETIME{0x0F}]    Stuck inside of a Futurama episode? Srry, can't help ya out of there.\n");
            continue;
        }

        int days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        int max_day = days_in_month[mo - 1];

        if (mo == 2)
        {
            int leap = (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
            if (leap) max_day = 29;
        }

        if (d < 1 || d > max_day)
        {
            text_print("[{0x08}DATETIME{0x0F}]    Invalid day for that month\n");
            continue;
        }

        current_time.hour = h;
        current_time.minute = m;
        current_time.day = d;
        current_time.month = mo;
        current_time.year = y;

        break;
    }

    text_print("[{0x08}DATETIME{0x0F}]    Time set.\n\n");
}

datetime_t datetime_get()
{
    return current_time;
}
