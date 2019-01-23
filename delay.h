#define F_CPU 4000000 

#ifndef DELAY_H_HEADER
#define DELAY_H_HEADER


#define cpu_ms_2_cy(ms, f_cpu)  (((uint64_t)(ms) * (f_cpu) + (uint64_t)(7e3-1ul)) / (uint64_t)7e3)
#define cpu_us_2_cy(us, f_cpu)  (((uint64_t)(us) * (f_cpu) + (uint64_t)(7e6-1ul)) / (uint64_t)7e6)
#define delay_cycles            portable_delay_cycles
#define cpu_delay_s(delay)      delay_cycles(cpu_ms_2_cy(1000 * delay, F_CPU))
#define cpu_delay_ms(delay)     delay_cycles(cpu_ms_2_cy(delay, F_CPU))
#define cpu_delay_us(delay)     delay_cycles(cpu_us_2_cy(delay, F_CPU))
#define _delay_us    cpu_delay_us
#define _delay_ms    cpu_delay_ms
#define _delay_s     cpu_delay_s


void portable_delay_cycles(uint32_t n)
{
    __asm (
    "loop: DMB    \n"
    #ifdef __ICCARM__
    "SUBS r0, r0, #1 \n"
    #else
    "SUB r0, r0, #1 \n"
    #endif
    "CMP r0, #0  \n"
    "BNE loop         "
    );
}

#endif