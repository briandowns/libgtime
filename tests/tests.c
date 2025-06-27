#include <stdint.h>
#include <stdbool.h>

#include "crosscheck.h"
#include "../gtime.h"

void
cc_setup(){}

void
cc_tear_down(){}

cc_result_t
test_gtime_now_success()
{
    const gtime_t now = gtime_now();
    CC_ASSERT_INT64_NOT_EQUAL((int64_t)now.seconds, 0);
    CC_ASSERT_INT64_NOT_EQUAL(now.nanoseconds, 0);
    CC_SUCCESS;
}

cc_result_t
test_gtime_leap_year_success()
{
    bool ret1 = gtime_is_leap_year(1980);
    bool ret2 = gtime_is_leap_year(1981);
    CC_ASSERT_TRUE(ret1);
    CC_ASSERT_FALSE(ret2);
    
    CC_SUCCESS;
}

cc_result_t
test_gtime_duration_until_success()
{
    const gtime_t now = gtime_now();
    const gtime_duration_t ten_seconds = GTIME_SECOND * 10;
    gtime_t val = gtime_add(now, ten_seconds);
    const gtime_duration_t x = gtime_until(val);
    CC_ASSERT_DOUBLE_EQUAL(gtime_dur_seconds(x), 10);

    CC_SUCCESS;
}

cc_result_t
test_gtime_day_success()
{
    const gtime_t now = gtime_now();
    const uint8_t dow = gtime_day(now);

    CC_ASSERT_UINT8_EQUAL(5, dow);

    CC_SUCCESS;
}

cc_result_t
test_gtime_equal_false()
{
    const gtime_t now = gtime_now();
    const gtime_t aMomentLater = gtime_now();
    bool ret = gtime_equal(now, aMomentLater);

    CC_ASSERT_FALSE(ret);

    CC_SUCCESS;
}

int
main(void)
{
    CC_INIT;

    CC_RUN(test_gtime_now_success);
    CC_RUN(test_gtime_leap_year_success);
    CC_RUN(test_gtime_duration_until_success);
    CC_RUN(test_gtime_day_success);
    CC_RUN(test_gtime_equal_false);

    CC_COMPLETE;
}
