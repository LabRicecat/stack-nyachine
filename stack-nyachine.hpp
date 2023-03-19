#ifndef STACK_NYACHINE_HPP
#define STACK_NYACHINE_HPP

#include <stdint.h>
#ifndef STACK_NYACHINE_NO_IO
# include <cstdio>
#endif

namespace stack_nyachine {

using chuwunk = uint8_t;

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

enum OptCodes {
    OPT_PUwUSH, // VAL ADDR
    OPT_REL8_JUwUMP, // ADDR ADDR ADDR MODE
    OPT_REL16_JUwUMP, // ADDR ADDR ADDR ADDR MODE
    OPT_ABS32_JUwUMP, // ADDR ADDR ADDR ADDR ADDR
    OPT_ADD, // ADDR ADDR ADDR
    OPT_SUwUB, // ADDR ADDR ADDR
    OPT_AAH_STOPP, // --
    OPT_DEREF, // ADDR ADDR
    OPT_NOwOP, // --
};

void ruwun(StackNyachine* StackNyachine, StackNyachine::size_tywp at) {
    if(StackNyachine->memowory_size <= at) return;

    for(chuwunk* p = &StackNyachine->memowory[at] ; ; ++p) {
        switch(*p) {
            case OPT_PUwUSH:
                StackNyachine->heawp[*(p + 2)] = *(p + 1);
                p += 2;
                break;
            case OPT_REL8_JUwUMP:
                if(StackNyachine->heawp[*(p + 1)] == StackNyachine->heawp[*(p + 2)]) 
                    if(*(p + 4) == 1) p -= StackNyachine->heawp[*(p + 3)] - 1;
                    else p += StackNyachine->heawp[*(p + 3)] - 1;
                else p += 4;
                break;
            case OPT_REL16_JUwUMP:
                if(StackNyachine->heawp[*(p + 1)] == StackNyachine->heawp[*(p + 2)]) 
                    if(*(p + 5) == 1) p -= StackNyachine->heawp[*(p + 3)] + StackNyachine->heawp[*(p + 4)] - 1;
                    else p += StackNyachine->heawp[*(p + 3)] + StackNyachine->heawp[*(p + 4)] - 1;
                else p += 5;
                break;
            case OPT_ABS32_JUwUMP:
                if(StackNyachine->heawp[*(p + 1)] == StackNyachine->heawp[*(p + 2)]) 
                    p = &StackNyachine->memowory[
                                                 StackNyachine->heawp[*(p + 3)] +
                                                 StackNyachine->heawp[*(p + 4)] +
                                                 StackNyachine->heawp[*(p + 5)]
                                                 ] - 1;
                else p += 5;
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
                return;
            case OPT_DEREF:
                StackNyachine->heawp[*(p + 1)] = StackNyachine->heawp[StackNyachine->heawp[*(p + 2)]];
                p += 2;
                break;
            case OPT_NOwOP:
                break;
            default:
#ifndef STACK_NYACHINE_NO_IO
                printf("unrecognised: %d\n", *p);
#endif
                return;
        }
    }
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