#include <stdio.h>
#include <stdlib.h>
#include "ctx.h"
#include "hardware.h"
#include "hw_config.h"

struct sem_s *sem1;
struct sem_s *sem2;
struct sem_s *sem3;
struct sem_s *sem4;

void f_philo1(void *arg);
void f_philo2(void *arg);
void f_philo3(void *arg);
void f_philo4(void *arg);

static void empty_it(void) {
  return;
}

int main(int argc, char *argv[]) {
  unsigned int i;

  sem1 = malloc(sizeof(struct sem_s));
  sem2 = malloc(sizeof(struct sem_s));
  sem3 = malloc(sizeof(struct sem_s));
  sem4 = malloc(sizeof(struct sem_s));
  sem_init(sem1, 1);
  sem_init(sem2, 1);
  sem_init(sem3, 1);
  sem_init(sem4, 1);

  create_ctx(16384, f_philo1, NULL);
  create_ctx(16384, f_philo2, NULL);
  create_ctx(16384, f_philo3, NULL);
  create_ctx(16384, f_philo4, NULL);

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

void f_philo1(void *args) {
  while(1) {
    sem_down(sem1);
    sem_down(sem2);
    printf("Philo1: 'Miam'\n");
    sem_up(sem2);
    sem_up(sem1);
  }
}
void f_philo2(void *args) {
  while(1) {
    sem_down(sem2);
    sem_down(sem3);
    printf("Philo2: 'Miam miam'\n");
    sem_up(sem3);
    sem_up(sem2);
  }
}
void f_philo3(void *args) {
  while(1) {
    sem_down(sem3);
    sem_down(sem4);
    printf("Philo3: 'Miam miam miam'\n");
    sem_up(sem4);
    sem_up(sem3);
  }
}
void f_philo4(void *args) {
  while(1) {
    sem_down(sem4);
    sem_down(sem1);
    printf("Philo4: 'Miam miam miam miam'\n");
    sem_up(sem1);
    sem_up(sem4);
  }
}
