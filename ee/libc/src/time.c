/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2005, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * Standard libc time functions
 */

#include <errno.h>
#include <sys/times.h>

#include <time.h>
#include <kernel.h>
#include <timer.h>

#ifdef F_clock
static int      s_intrOverflowID = -1;
static clock_t  s_intrOverflowCount = 0;

#ifdef TIME_USE_T0
#define INTC_TIM       INTC_TIM0
#define T_COUNT        T0_COUNT
#define T_MODE         T0_MODE
#define T_COMP         T0_COMP
#else
#define INTC_TIM       INTC_TIM1
#define T_COUNT        T1_COUNT
#define T_MODE         T1_MODE
#define T_COMP         T1_COMP
#endif

static int intrOverflow(int ca)
{
   s_intrOverflowCount++;

   // A write to the overflow flag will clear the overflow flag
   // ---------------------------------------------------------
   *T_MODE |= (1 << 11);

   ExitHandler();
   return -1;
}

void _ps2sdk_time_init(void)
{
   *T_MODE = 0x0000; // Disable T_MODE

   if (s_intrOverflowID == -1)
   {
       s_intrOverflowID = AddIntcHandler(INTC_TIM, intrOverflow, 0);
       EnableIntc(INTC_TIM);
   }

   // Initialize the timer registers
   // CLKS: 0x02 - 1/256 of the BUSCLK (0x01 is 1/16th)
   //  CUE: 0x01 - Start/Restart the counting
   // OVFE: 0x01 - An interrupt is generated when an overflow occurs
   *T_COUNT = 0;
   *T_MODE = Tn_MODE(0x02, 0, 0, 0, 0, 0x01, 0, 0x01, 0, 0);

   s_intrOverflowCount = 0;
}

void _ps2sdk_time_deinit(void)
{
   *T_MODE = 0x0000; // Stop the timer

   if (s_intrOverflowID >= 0)
   {
      DisableIntc(INTC_TIM);
      RemoveIntcHandler(INTC_TIM, s_intrOverflowID);
      s_intrOverflowID = -1;
   }

   s_intrOverflowCount = 0;
}

clock_t clock(void)
{
   u64         t;

   // Tn_COUNT is 16 bit precision. Therefore, each s_intrOverflowCount is 65536 ticks
   t = *T_COUNT + (s_intrOverflowCount << 16);

   return t;
}
#endif /* F_clock */

#ifdef F_time
time_t time(time_t *t)
{
	if (t != 0) {
		*t = (time_t)-1;
	}

	return (time_t)-1;
}
#endif

#ifdef F_times
clock_t times(struct tms *buf)
{
  return ENOSYS;
}
#endif

#ifdef F_mktime
static unsigned char month_days[12] =
{
  31,28,31,30,31,30,31,31,30,31,30,31
};


#define SECS_PER_MIN	 60L
#define MINS_PER_HOUR	 60L
#define HOURS_PER_DAY	 24L
#define SECS_PER_HOUR    ((unsigned long)MINS_PER_HOUR * SECS_PER_MIN)
#define SECS_PER_DAY     ((unsigned long)SECS_PER_HOUR * HOURS_PER_DAY)
#define DAYS_PER_WEEK	 7
#define MONS_PER_YEAR	 12L
#define DAYS_PER_YEAR	 365L
#define LEAPDAY		 1
#define EPOCH_BASE	 1900
#define EPOCH_YEAR	 1970
#define EPOCH_DAY	 4
#define is_leapyear(y)   ((y) % 4 == 0 && \
		           ((y) % 100 != 0 || (y) % 400 == 0))
#define is_overflow(a,units) (a >= (units + 1))
#define is_underflow(a)	     ((a < 0))

/* Fixes up overflow (> units)
   a is the tm member to give units
   b is the tm member overflowing
   units is the number of units per a */
#define fix_overflow(a,b,units) \
	  a = a + (b/units); \
	  b = b % units;

/* Fixes up underflow (< 0)
  a is the tm member to borrow units
  b is the tm member underflowing
  units is the number of units per a */
#define fix_underflow(a,b,units) \
	  a = a - (1 + (-b)/units); \
	  b = units - ((-b)%units);

/* Fixes up number out of range
  a is the tm member to borrow or fill
  b is the tm member out of range
  units is the number of units per a */
#define do_fixup(a,b,units) \
	  if (is_overflow(b,units)) { \
	    fix_overflow(a,b,units);  \
	  } \
	  else if (is_underflow(b)) { \
	    fix_underflow(a,b,units); \
	  }

time_t mktime(struct tm *t)
{
  int i,days,years;
  int cur_year;
  int yday;
  struct tm temp_tm = *t;
  time_t seconds;

  /* Use minutes to fixup seconds */
  do_fixup(temp_tm.tm_min, temp_tm.tm_sec, SECS_PER_MIN);
  /* Use hours to fixup minutes */
  do_fixup(temp_tm.tm_hour, temp_tm.tm_min, MINS_PER_HOUR);
  /* Use days to fixup hours */
  do_fixup(temp_tm.tm_mday, temp_tm.tm_hour, HOURS_PER_DAY);

  /* If days this month are < 0
     Add year's worth of days until it isn't. */
  while (temp_tm.tm_mday <= 0) {
    temp_tm.tm_year -= 1;
    temp_tm.tm_mday += DAYS_PER_YEAR;

    if (is_leapyear(temp_tm.tm_year + EPOCH_BASE))
      temp_tm.tm_mday += LEAPDAY;
  }

  /* Use years to fixup months */
  do_fixup(temp_tm.tm_year, temp_tm.tm_mon, MONS_PER_YEAR);

  /* Now to figure out days > month_days[tm_mon]. */
  while (1) {
    if (is_leapyear(temp_tm.tm_year + EPOCH_BASE))
      month_days[1] = 28 + LEAPDAY;
    else
      month_days[1] = 28;

    days = month_days[temp_tm.tm_mon];
    if (temp_tm.tm_mday <= days)
      break;

    /* Too many days, so remove days and add month. */
    temp_tm.tm_mday -= days;
    temp_tm.tm_mon++;

    /* In case months goes over 12. */
    fix_overflow(temp_tm.tm_year,temp_tm.tm_mon, 12);
  }

  *t = temp_tm;

  if (t->tm_year + EPOCH_BASE < EPOCH_YEAR)
    return -1;

  /* Years since EPOCH_BASE */
  cur_year = t->tm_year + EPOCH_BASE;

  /* Years since EPOCH */
  years = t->tm_year + EPOCH_BASE - EPOCH_YEAR;

  days = (years) * DAYS_PER_YEAR;

  for (i = EPOCH_YEAR; i < cur_year; i++) {
    if (is_leapyear(i))
      days++;
  }

  if (is_leapyear(cur_year))
    month_days[1] = 28 + LEAPDAY;

  for (yday = 0,i = 0; i < t->tm_mon; i++)
    yday += month_days[i];

  yday += t->tm_mday - 1;

  days += yday;

  t->tm_yday = yday;
  t->tm_wday = (days + EPOCH_DAY) % DAYS_PER_WEEK;

  seconds = t->tm_sec;
  seconds += t->tm_min * SECS_PER_MIN;
  seconds += t->tm_hour * SECS_PER_HOUR;

  seconds = days * SECS_PER_DAY;

  return seconds;

}
#endif /* F_mktime */
