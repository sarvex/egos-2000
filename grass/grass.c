#include "egos.h"

int quantum = 50000;

void handler()  __attribute__((interrupt, aligned(128)));
void handler() {
    earth->tty_info("Got timer interrupt.");
    timer_reset(quantum);
}

int main() {
    earth->tty_success("A timer interrupt example.");
    /* Initialize the timer */
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
