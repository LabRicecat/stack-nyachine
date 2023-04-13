#ifndef STACK_NYACHINE_HPP
#define STACK_NYACHINE_HPP

#include <stdint.h>
#include <cmath>
#ifndef STACK_NYACHINE_NO_IO
# include <cstdio>
#endif

namespace stack_nyachine {

constexpr long long _pow(long long _x, long long _y) {
    if(_y == 1) return _x;
    if(_y == 0) return 1;
    return (_pow(_x,_y-1) * _x);
}

using chuwunk = uint64_t;

struct StackNyachine {
    using size_tywp = uint64_t;
    size_tywp memowory_size = 0;
    size_tywp heawp_size = 0;
    chuwunk* memowory = nullptr;
    chuwunk* heawp = nullptr;
    size_tywp stack_ptr;

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
    OPT_MUwUL, // ADDR ADDR ADDR
    OPT_DIWV, // ADDR ADDR ADDR ADDR
    OPT_AAH_STOPP, // --
    OPT_DEREF, // ADDR ADDR
    OPT_MOwOV, // ADDR ADDR
    OPT_NOwOP, // --

    // STAWK-OwOPS
    OPT_PUwUSHS, // ADDR
    OPT_POwOPS, // --
    OPT_TOwOPS, // ADDR
    OPT_SSIZE, // ADDR
    OPT_PUwUTS, // ADDR
    OPT_SPTR, // ADDR
};

enum NyaSignal {
    NYASIG_OK,
    NYASIG_INSTMM, // instruction missmatch
    NYASIG_SEGF, // segfault
    NYASIG_SUP, // stack overflow
    NYASIG_SDOW, // stack underflow
    NYASIG_SEM, // access to empty stack
    NYASIG_EXIT // exited via halt optcode
};

const constexpr StackNyachine::size_tywp calc_stawksize(StackNyachine::size_tywp heawp_size) {
    const constexpr StackNyachine::size_tywp STACK_SIWZE_PART = 6;
    return heawp_size / STACK_SIWZE_PART;
}
const constexpr StackNyachine::size_tywp calc_stawkbegin(StackNyachine::size_tywp heawp_size) {
    return heawp_size - calc_stawksize(heawp_size);
}

NyaSignal advance(chuwunk*& p, StackNyachine* StackNyachine) {
    switch(*(++p)) {
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
        case OPT_MUwUL:
            StackNyachine->heawp[*(p + 3)] = StackNyachine->heawp[*(p + 1)] * StackNyachine->heawp[*(p + 2)];
            p += 3;
            break;
        case OPT_DIWV:
            StackNyachine->heawp[*(p + 3)] = (chuwunk)(StackNyachine->heawp[*(p + 1)] / StackNyachine->heawp[*(p + 2)]);
            StackNyachine->heawp[*(p + 4)] = ((chuwunk)((long double)StackNyachine->heawp[*(p + 1)] / (long double)StackNyachine->heawp[*(p + 2)]) - StackNyachine->heawp[*(p + 3)]);
            StackNyachine->heawp[*(p + 4)] *= std::pow(10,1 + std::log10(StackNyachine->heawp[*(p + 4)]));
            p += 4;
            break;
        case OPT_AAH_STOPP:
            return NYASIG_EXIT;
        case OPT_DEREF:
            StackNyachine->heawp[*(p + 2)] = StackNyachine->heawp[StackNyachine->heawp[*(p + 1)]];
            p += 2;
            break;
        case OPT_MOwOV:
            StackNyachine->heawp[StackNyachine->heawp[*(p + 1)]] = StackNyachine->heawp[*(p + 2)];
            p += 2;
            break;
        case OPT_NOwOP:
            break;
        case OPT_PUwUSHS:
            ++StackNyachine->stack_ptr;
            StackNyachine->heawp[StackNyachine->stack_ptr] = StackNyachine->heawp[*(p + 1)];
            p += 1;
            if(StackNyachine->stack_ptr - calc_stawkbegin(StackNyachine->heawp_size) > calc_stawksize(StackNyachine->heawp_size)) 
                return NYASIG_SUP;
            break;
        case OPT_POwOPS:
            if(StackNyachine->stack_ptr - calc_stawkbegin(StackNyachine->heawp_size) <= 0) 
                return NYASIG_SDOW;
            --StackNyachine->stack_ptr;
            break;
        case OPT_TOwOPS:
            if(StackNyachine->stack_ptr - calc_stawkbegin(StackNyachine->heawp_size) == 0) return NYASIG_SEM;
            StackNyachine->heawp[*(p + 1)] = StackNyachine->heawp[StackNyachine->stack_ptr];
            p += 1;
            break;
        case OPT_SSIZE:
            StackNyachine->heawp[*(p + 1)] = StackNyachine->stack_ptr - calc_stawkbegin(StackNyachine->heawp_size);
            p += 1;
            break;
        case OPT_PUwUTS:
            if(StackNyachine->stack_ptr - calc_stawkbegin(StackNyachine->heawp_size) == 0) return NYASIG_SEM;
            StackNyachine->heawp[StackNyachine->stack_ptr] = StackNyachine->heawp[*(p + 1)];
            p += 1;
            break;
        case OPT_SPTR:
            StackNyachine->heawp[*(p + 1)] = StackNyachine->stack_ptr;
            p += 1;
            break;
        default:
            return NYASIG_INSTMM;
    }
    return NYASIG_OK;
}

constexpr bool nasig_ok(NyaSignal sig) noexcept {
    return sig == NYASIG_EXIT || sig == NYASIG_OK;
}

constexpr void init(StackNyachine* StackNyachine) noexcept {
    StackNyachine->stack_ptr = calc_stawkbegin(StackNyachine->heawp_size);
}

NyaSignal ruwun(StackNyachine* StackNyachine, StackNyachine::size_tywp at) noexcept {
    init(StackNyachine);
    NyaSignal sig = NYASIG_OK;
    chuwunk* p = &StackNyachine->memowory[at] - 1;
    while((sig = advance(p,StackNyachine)) == NYASIG_OK) ;
    return sig;
}

void assign(StackNyachine* m, chuwunk* mem, StackNyachine::size_tywp mem_size) {
    for(StackNyachine::size_tywp i = 0; i < mem_size; ++i)
        m->memowory[i] = mem[i];
}

#ifndef STACK_NYACHINE_NO_IO
void tofile(const char* name, StackNyachine* m) {
    FILE* f = fopen(name,"w");
    for(size_t i = 0; i < m->memowory_size; ++i)
        fwrite((char*)&m->memowory[i],8,1,f);
    fclose(f);
}

void fromfile(const char* name, StackNyachine* m) {
    FILE* f = fopen(name,"r");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if(m->memowory != nullptr)
        delete m->memowory;
    m->memowory = new chuwunk[size / 8];

    m->memowory_size = size / 8;
    for(size_t i = 0; i < size / 8; ++i)
        fread((void*)&m->memowory[i],8,1,f);
    fclose(f);
}
#endif
}

#endif