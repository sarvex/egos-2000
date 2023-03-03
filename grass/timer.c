#define CLINT0_MTIME     0x200bff8
#define CLINT0_MTIMECMP  0x2004000

long long mtime_get() {
    int low, high;
    do {
        high = *(int*)(CLINT0_MTIME + 4);
        low  = *(int*)(CLINT0_MTIME);
    }  while ( *(int*)(CLINT0_MTIME + 4) != high );

    return (((long long)high) << 32) | low;
}

void mtimecmp_set(long long time) {
    *(int*)(CLINT0_MTIMECMP + 4) = 0xFFFFFFFF;
    *(int*)(CLINT0_MTIMECMP + 0) = (int)time;
    *(int*)(CLINT0_MTIMECMP + 4) = (int)(time >> 32);
}
