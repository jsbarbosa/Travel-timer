#include <stdint.h>

// PIN 2 is the interrupt pin
#define POWER_OUTPUT_PIN 13
#define TIMER_TIME_SECONDS 1
#define ACTIVE_TIME_SECONDS 7 // must be a multiple of TIMER_TIME_SECONDS

extern const int ACTIVE_TIME_ITERATIONS;
extern volatile int ACTIVE_TIME_ITERATOR;
extern volatile uint8_t TIMER_ACTIVE;

void enable_timer();
void disable_timer(void);
void power_down(void);
