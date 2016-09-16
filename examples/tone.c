#include "hw.h"

#define TIMER0_RELOAD_VALUE (-OSCILLATOR/1000) // 0.999348ms for 11.059Mhz

static unsigned int toggle_count;
static unsigned char tl0, th0;

//void ClockIrqHandler (void) interrupt 1 using 3 {
void ClockIrqHandler (void) __interrupt (1) {
  if(toggle_count) {
    toggle_count--;  
    TL0 = tl0;
    TH0 = th0;
    P1_3 = !P1_3;
  } else
    P1_3 = 0;
}

void tone(unsigned int frequency, unsigned int duration) {
  unsigned int timer_reload_value;
  ET0=0;
  P1_3 = 0;
  toggle_count = (frequency * duration)  / 500;  // 2 * frequency * duration  / 1000
  timer_reload_value = (-(OSCILLATOR / 512) / frequency) * 512;
  tl0 = timer_reload_value & 0xff;
  th0 = timer_reload_value >> 8;
  TL0 = tl0;
  TH0 = th0;
  TR0 = 1; // start timer 0
  ET0 = 1;
}

unsigned char is_playing (void)
{
  unsigned char playing;
  ET0 = 0;
  playing = toggle_count != 0;
  ET0 = 1;
  return playing;
}

unsigned char _sdcc_external_startup() {
  toggle_count = 0;

  P1M0 &= ~(1<<3);
  P1M1 |= (1<<3);
  P1_3 = 0;

  // initialize timer0 for system clock
  TR0=0; // stop timer 0
  TMOD =(TMOD&0xf0)|0x01; // T0=16bit timer
  ET0=0;
  EA=1; // enable global interrupt

  return 0;
}

