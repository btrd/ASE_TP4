#include <stdio.h>
#include <stdlib.h>
#include "ctx.h"
#include "hardware.h"
#include "hw_config.h"

struct ctx_s* ctx_ping;
struct ctx_s* ctx_pong;
struct ctx_s* ctx_pang;

void f_ping(void *arg);
void f_pong(void *arg);
void f_pang(void *arg);

static void empty_it(void) {
  return;
}

int main(int argc, char *argv[]) {
  unsigned int i;

  create_ctx(16384, f_ping, NULL);
  create_ctx(16384, f_pong, NULL);
  create_ctx(16384, f_pang, NULL);
  
  /* init hardware */
  if (init_hardware(HARDWARE_INI) == 0) {
    fprintf(stderr, "Error in hardware initialization\n");
    exit(EXIT_FAILURE);
  }
  
  /* dummy interrupt handlers */
  for (i=0; i<16; i++)
    IRQVECTOR[i] = empty_it;
  
  /* program timer */
  IRQVECTOR[TIMER_IRQ] = yield;
  _out(TIMER_PARAM, 128+64+32+8); /* reset + alarm on + 8 tick / alarm */
  _out(TIMER_ALARM, 0xFFFFFEEE);  /* alarm at next tick (at 0xFFFFFFFF) */
  
  /* allows all IT */
  _mask(1);
  yield();

  exit(EXIT_SUCCESS);
}

void f_ping(void *args) {
  while(1) {
    printf("A");
    yield();
    printf("B");
    yield();
    printf("C");
    yield();
  }
}
void f_pong(void *args) {
  int i;
  for (i = 0; i < 100; i++){
    printf("1");
    yield();
    printf("2");
    yield();
  }
  printf("\n");
  exit(0);
}
void f_pang(void *args) {
  while(1) {
    printf("$");
    yield();
    printf("&");
    yield();
  }
}
