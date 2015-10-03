#include "ctx.h"
#include "hw_config.h"
#include "hardware.h"

struct ctx_s *current_ctx;

void irq_enable() {
  _mask(0);
}
void irq_disable() {
  _mask(15);
}

int init_ctx(struct ctx_s* ctx, int stack_size, funct_t* f, void* args) {
  ctx->stack = malloc(stack_size);
  if (ctx->stack == NULL)
    return 0;
  ctx->ebp = &ctx->stack[stack_size-4];
  ctx->esp = &ctx->stack[stack_size-4];
  ctx->f = f;
  ctx->args = args;
  ctx->state = CTX_READY;
  ctx->ctx_magic = MAGIC;
  return 1;
}

void switch_to_ctx(struct ctx_s* ctx) {
  assert(ctx != NULL);
  assert(ctx->ctx_magic == MAGIC);
  assert(ctx->state == CTX_READY || ctx->state == CTX_ACTIVATED);
  if (current_ctx != NULL) {
    asm("mov %%esp, %0": "=R"(current_ctx->esp));
    asm("mov %%ebp, %0": "=R"(current_ctx->ebp));
  }
  current_ctx = ctx;
  asm("mov %0, %%ebp":: "R"(current_ctx->ebp));
  asm("mov %0, %%esp":: "R"(current_ctx->esp));

  irq_enable();
  if(current_ctx->state == CTX_READY) {
    exec_f(current_ctx);
    /* Cette fonction ne revient jamais */
  }
}

struct ctx_s* create_ctx(int stack_size, funct_t f, void* args) {
  irq_disable();
  struct ctx_s* tmp = malloc(sizeof(struct ctx_s));
  init_ctx(tmp, stack_size, f, args);
  if(!current_ctx) {
    tmp->next = tmp;
    current_ctx = tmp;
  } else {
    get_last_ctx()->next = tmp;
    tmp->next = current_ctx;
  }
  irq_enable();
  return tmp;
}

void yield() {
  _out(TIMER_ALARM,0xFFFFFEEE);
  switch_to_ctx(current_ctx->next);
}

void exec_f(struct ctx_s* ctx) {
  current_ctx->state = CTX_ACTIVATED;
  current_ctx->f(current_ctx->args);
  if (current_ctx->next != current_ctx) {
    irq_disable();

    current_ctx->state = CTX_TERMINATED;

    struct ctx_s* last_ctx = get_last_ctx();

    last_ctx->next = current_ctx->next;

    free(current_ctx->stack); /* /!\ on libère une stack qu'on est entrain d'utiliser */
    free(current_ctx);
    current_ctx = NULL;
    switch_to_ctx(last_ctx->next);
  } else {
    irq_enable();
    printf("Plus aucun proc\n");
    exit(0);
  }
}

struct ctx_s* get_last_ctx() {
  struct ctx_s* last_ctx;
  last_ctx = current_ctx->next;
  while(last_ctx->next != current_ctx) {
    last_ctx = last_ctx->next;
  }
  return last_ctx;
}
