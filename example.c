#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "gtime.h"

int
main(void) 
{
    gtime_t now = gtime_now();

    char buf[64];
    gtime_format(now, "%Y-%m-%d %H:%M:%S", buf, sizeof(buf));
    printf("Now: %s\n", buf);

    gtime_duration_t d = gtime_duration_new(2500000000); // 2.5 seconds
    gtime_t later = gtime_add(now, d);

    gtime_format(later, "%H:%M:%S", buf, sizeof(buf));
    printf("Later: %s\n", buf);

    printf("Sleeping for 2.5 seconds...\n");
    gtime_sleep(d);

    gtime_duration_t elapsed = gtime_since(now);
    char dur_str[1024];
    gtime_dur_str(elapsed, dur_str, sizeof(dur_str));
    printf("Elapsed: %s\n", dur_str);

    gtime_duration_t dur = GTIME_SECOND * 60;

    printf("Seconds 1: %f\n", gtime_dur_seconds(elapsed));
    printf("Seconds 2: %.2f\n", gtime_dur_seconds(dur));
    printf("Milliseconds: %f ms\n", gtime_dur_milliseconds(elapsed));
    printf("Nanoseconds: %f ns\n", gtime_dur_nanoseconds(elapsed));

    return 0;
}
