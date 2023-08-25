#include <am.h>
#include <nemu.h>

uint32_t init_sec, init_usec;

void __am_timer_init() {
  init_usec = inl(RTC_ADDR);
  init_sec = inl(RTC_ADDR + 4);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uint32_t uptime_usec = inl(RTC_ADDR);
  uint32_t uptime_sec = inl(RTC_ADDR + 4);
  uptime->us = (uptime_sec - init_sec)*1000000 + (uptime_usec - init_usec);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
