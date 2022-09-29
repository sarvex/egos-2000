#include "egos.h"

int quantum = 5000;

void handler1()  __attribute__((interrupt));
void handler1() {
    earth->tty_info("Got timer interrupt.");
    mtimecmp_set(mtime_get() + quantum);
}

void thread0() { while(1) {earth->tty_success("Something green");} }
void thread1() { while(1) {earth->tty_critical("Something yellow");} }

int next_thread = 0;
void handler2()  __attribute__((interrupt));
void handler2() {
    /* Forget previous context
     * and start a new thread calling thread0() or thread1() */
    next_thread = 1 - next_thread;
    asm("csrw mepc, %0" ::"r"((next_thread == 0)? thread0 : thread1));
    /* Reset timer and enter the new thread */
    mtimecmp_set(mtime_get() + quantum);
    asm("li sp,0x80002000");
    asm("mret");
}

int main() {
    earth->tty_success("A timer interrupt example.");

    /* Setup timer and register handler */
    //asm("csrw mtvec, %0" ::"r"(handler1));
    asm("csrw mtvec, %0" ::"r"(handler2));
    mtimecmp_set(mtime_get() + quantum);
    /* Enable timer interrupt */
    int mstatus, mie;
    asm("csrr %0, mstatus" : "=r"(mstatus));
    asm("csrw mstatus, %0" ::"r"(mstatus | 0x8));
    asm("csrr %0, mie" : "=r"(mie));
    asm("csrw mie, %0" ::"r"(mie | 0x80));

    while(1);
}
