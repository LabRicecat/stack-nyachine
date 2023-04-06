#ifndef STACK_NYACHINE_HPP
#define STACK_NYACHINE_HPP

#include <stdint.h>
#ifndef STACK_NYACHINE_NO_IO
# include <cstdio>
#endif

namespace stack_nyachine {

consteval long long _pow(long long _x, long long _y) {
    if(_y == 1) return _x;
    return (_pow(_x,_y-1) * _x);
}

using chuwunk = uint64_t;

struct StackNyachine {
    using size_tywp = unsigned long long;
    size_tywp memowory_size = 0;
    size_tywp heawp_size = 0;
    chuwunk* memowory = nullptr;
    chuwunk* heawp = nullptr;

    StackNyachine(size_tywp memowory_s, size_tywp heawp_s)
    : memowory_size(memowory_s), heawp_size(heawp_s) {
        memowory = new chuwunk[memowory_s];
        heawp = new chuwunk[heawp_s];
    }
};

enum OWOptCodes {
    OPT_PUwUSH, // VAL ADDR
    OPT_JUwUMP, // ADDR ADDR ADDR
    OPT_ADD, // ADDR ADDR ADDR
    OPT_SUwUB, // ADDR ADDR ADDR
    OPT_AAH_STOPP, // --
    OPT_DEREF, // ADDR ADDR
    OPT_MOwOV, // ADDR ADDR
    OPT_COwOPY, // ADDR ADDR
    OPT_NOwOP, // --

    // STAWK-OwOPS
    OPT_PUwUSHS, // ADDR
    OPT_POwOPS,
    OPT_TOwOPS, // ADDR
    OPT_SSIZE, // ADDR
    OPT_PUwUTS, // ADDR
};

enum NyaSignal {
    NYASIG_OK,
    NYASIG_INSTMM, // instruction missmatch
    NYASIG_SEGF, // segfault
    NYASIG_SUP, // stack overflow
    NYASIG_SDOW, // stack underflow
    NYASIG_SEM, // access to empty stack
};

const constexpr StackNyachine::size_tywp calc_stawksize(StackNyachine::size_tywp heawp_size) {
    const constexpr StackNyachine::size_tywp STACK_SIWZE_PART = 6;
    return heawp_size / STACK_SIWZE_PART;
}
const constexpr StackNyachine::size_tywp calc_stawkbegin(StackNyachine::size_tywp heawp_size) {
    return heawp_size - calc_stawksize(heawp_size);
}


NyaSignal ruwun(StackNyachine* StackNyachine, StackNyachine::size_tywp at) {
    StackNyachine::size_tywp stack_ptr = calc_stawkbegin(StackNyachine->heawp_size);

    for(chuwunk* p = &StackNyachine->memowory[at] ; ; ++p) {
        switch(*p) {
            case OPT_PUwUSH:
                StackNyachine->heawp[*(p + 2)] = *(p + 1);
                p += 2;
                break;
            case OPT_JUwUMP:
                if(StackNyachine->heawp[*(p + 1)] == StackNyachine->heawp[*(p + 2)]) 
                    p = &StackNyachine->memowory[StackNyachine->heawp[*(p + 3)] - 1];
                else p += 3;
                break;
            case OPT_ADD:
                StackNyachine->heawp[*(p + 3)] = StackNyachine->heawp[*(p + 1)] + StackNyachine->heawp[*(p + 2)];
                p += 3;
                break;
            case OPT_SUwUB:
                StackNyachine->heawp[*(p + 3)] = StackNyachine->heawp[*(p + 1)] - StackNyachine->heawp[*(p + 2)];
                p += 3;
                break;
            case OPT_AAH_STOPP:
                return NYASIG_OK;
            case OPT_DEREF:
                StackNyachine->heawp[*(p + 2)] = StackNyachine->heawp[StackNyachine->heawp[*(p + 1)]];
                p += 2;
                break;
            case OPT_MOwOV:
                StackNyachine->heawp[*(p + 1)] = StackNyachine->heawp[StackNyachine->heawp[*(p + 2)]];
                p += 2;
                break;
            case OPT_COwOPY:
                StackNyachine->heawp[*(p + 1)] = StackNyachine->heawp[*(p + 2)];
                p += 2;
                break;
            case OPT_NOwOP:
                break;
            case OPT_PUwUSHS:
                ++stack_ptr;
                StackNyachine->heawp[stack_ptr] = StackNyachine->heawp[*(p + 1)];
                p += 1;
                if(stack_ptr - calc_stawkbegin(StackNyachine->heawp_size) > calc_stawksize(StackNyachine->heawp_size)) return NYASIG_SUP;
                break;
            case OPT_POwOPS:
                if(stack_ptr - calc_stawkbegin(StackNyachine->heawp_size) <= 0) return NYASIG_SDOW;
                --stack_ptr;
                break;
            case OPT_TOwOPS:
                if(stack_ptr - calc_stawkbegin(StackNyachine->heawp_size) == 0) return NYASIG_SEM;
                StackNyachine->heawp[*(p + 1)] = StackNyachine->heawp[stack_ptr];
                p += 1;
                break;
            case OPT_SSIZE:
                StackNyachine->heawp[*(p + 1)] = stack_ptr - calc_stawkbegin(StackNyachine->heawp_size);
                p += 1;
                break;
            case OPT_PUwUTS:
                if(stack_ptr - calc_stawkbegin(StackNyachine->heawp_size) == 0) return NYASIG_SEM;
                StackNyachine->heawp[stack_ptr] = StackNyachine->heawp[*(p + 1)];
                p += 1;
                break;
            default:
                return NYASIG_INSTMM;
        }
    }
    return NYASIG_OK;
}

void assign(StackNyachine* m, chuwunk* mem, StackNyachine::size_tywp mem_size) {
    for(StackNyachine::size_tywp i = 0; i < mem_size; ++i)
        m->memowory[i] = mem[i];
}

#ifndef STACK_NYACHINE_NO_IO
void tofile(const char* name, StackNyachine* m) {
    FILE* f = fopen(name,"w");
    fwrite(m->memowory,m->memowory_size,m->memowory_size,f);
    fclose(f);
}

void fromfile(const char* name, StackNyachine* m) {
    FILE* f = fopen(name,"r");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    chuwunk* r = new chuwunk[size];
    delete m->memowory;

    m->memowory_size = size;
    fread(r,size,size,f);
    m->memowory = r;
    fclose(f);
}
#endif
}

#endif