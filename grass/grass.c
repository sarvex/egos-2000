/*
 * (C) 2022, Cornell University
 * All rights reserved.
 */

/* Author: Yunhao Zhang
 * Description: a simple example of timer interrupt handler
 */

#include "egos.h"

void timer_init();
void timer_reset(int);
struct earth *earth = (void*)GRASS_STACK_TOP;

int quantum = 50000;

static void handler()  __attribute__((interrupt, aligned(128)));
static void handler() {
    earth->tty_info("Got timer interrupt.");
    timer_reset(quantum);
}

int main() {
    earth->tty_success("Enter the timer interrupt example.");

    timer_init();
    timer_reset(quantum);

    /* Register timer handler */
    __asm__ volatile("csrw mtvec, %0" ::"r"(handler));

    /* Enable global interrupt */
    int mstatus;
    __asm__ volatile("csrr %0, mstatus" : "=r"(mstatus));
    mstatus |= 0x8;
    __asm__ volatile("csrw mstatus, %0" ::"r"(mstatus));

    /* Enable timer interrupt */
    int mie;
    __asm__ volatile("csrr %0, mie" : "=r"(mie));
    mie |= 0x80;
    __asm__ volatile("csrw mie, %0" ::"r"(mie));
    
    while(1);
}
