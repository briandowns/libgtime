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
    gtime_t now = gtime_now();
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

int
main(void)
{
    CC_INIT;

    CC_RUN(test_gtime_now_success);
    CC_RUN(test_gtime_leap_year_success);

    CC_COMPLETE;
}
