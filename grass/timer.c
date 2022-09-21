#define CLINT0_MTIME     0x200bff8
#define CLINT0_MTIMECMP  0x2004000

static long long mtime_get() {
    int low, high;
    /* Guard against rollover when reading mtime */
    do {
        low = *(int*)(CLINT0_MTIME);
        high = *(int*)(CLINT0_MTIME + 4);
    } while (*(int*)(CLINT0_MTIME + 4) != high);

    return (((long long)high) << 32) | low;
}

static void mtimecmp_set(long long time) {
    *(int*)(CLINT0_MTIMECMP + 4) = 0xFFFFFFFF;
    *(int*)(CLINT0_MTIMECMP + 0) = (int)time;
    *(int*)(CLINT0_MTIMECMP + 4) = (int)(time >> 32);
}

void timer_init()  {
    mtimecmp_set(0);
}

void timer_reset(int quantum) {
    mtimecmp_set(mtime_get() + quantum);
}
