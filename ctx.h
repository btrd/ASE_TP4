#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "hw_config.h"

typedef void(funct_t)(void*);

enum ctx_state_e { CTX_READY, CTX_ACTIVATED, CTX_TERMINATED, CTX_LOCKED };

struct ctx_s {
  void* esp;
  void* ebp;
  funct_t* f;
  void* args;
  unsigned char* stack;
  enum ctx_state_e state;
  unsigned int ctx_magic;
  struct ctx_s* next;
  #define MAGIC 0xDEADBEEF 
};

struct sem_s {
  int count;
  struct ctx_s *ctx_locked;
};

void irq_enable();
void irq_disable();
int init_ctx(struct ctx_s* ctx, int stack_size, funct_t* f, void* args);
void switch_to_ctx(struct ctx_s* ctx);
void exec_f(struct ctx_s* ctx);
struct ctx_s* create_ctx(int stack_size, funct_t f, void* args);
void yield();
struct ctx_s* get_last_ctx();

void sem_init(struct sem_s *sem, unsigned int val);
void sem_down(struct sem_s *sem);
void sem_up(struct sem_s *sem);

#endif
