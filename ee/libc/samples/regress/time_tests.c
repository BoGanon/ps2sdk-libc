#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <testsuite.h>

static const char *test_clock(void *arg)
{
    clock_t c = clock();

    if (c == 0)
        return "clock not working";

    printf("\n(clock start: %d)", (int) c);
    printf("\nwaiting 5 secs");

    clock_t w = c + 5 * CLOCKS_PER_SEC;
    while (clock() < w)
        ;

    clock_t e = clock();
    int diff = (int) ((e - c) / CLOCKS_PER_SEC);

    printf("\n(clock end: %d)", (int) e);
    printf("\n(elapsed: %d sec)", diff);

    if (diff != 5)
        return "clock pause not working";

    return NULL;
}

static const char *test_sleep(void *arg)
{
    printf("\nwaiting 5 secs");

    clock_t c = clock();
    sleep(5);
    clock_t e = clock();

    int diff = (int) ((e - c) / CLOCKS_PER_SEC);

    printf("\n(elapsed: %d sec)", diff);

    if (diff != 5)
        return "sleep not working";

    return NULL;
}


/* If local timezones are supported, then this test needs updating. */
// Seconds since Unix epoch at y2k at GMT-0
#define THE_YEAR_2000 946684800
static const char *test_mktime(void *arg)
{
    struct tm time_str;

    time_t my_time;
    time_t my_time2;

    // 1/1/1970 + THE_YEAR_2000 in seconds = 1/1/2000
    time_str.tm_year = 1970 - 1900;
    time_str.tm_mon = 0;
    time_str.tm_mday = 1;
    time_str.tm_hour = 0;
    time_str.tm_min = 0;
    time_str.tm_sec = THE_YEAR_2000;
    time_str.tm_isdst = -1;

    if ((my_time = mktime(&time_str)) == -1)
	return "mktime failed 1";

    // 1/1/2000
    time_str.tm_year = 100;
    time_str.tm_mon = 0;
    time_str.tm_mday = 1;
    time_str.tm_hour = 0;
    time_str.tm_min = 0;
    time_str.tm_sec = 0;
    time_str.tm_isdst = -1;

    if ((my_time2 = mktime(&time_str)) == -1)
    	return "mktime failed 2";

    if ((my_time != my_time2) || ((my_time != THE_YEAR_2000)
	|| (my_time2 != THE_YEAR_2000)))
	return "mktime is not correct";

   return NULL;

}

int time_add_tests(test_suite *p)
{
    add_test(p, "clock", test_clock, NULL);
    add_test(p, "sleep", test_sleep, NULL);
    add_test(p, "mktime", test_mktime, NULL);

	return 0;
}
