#include "stack-nyachine.hpp"
#include <iostream>

int main() {
    using namespace stack_nyachine;
#if 1
    // simple program to multiply a number by 2
    chuwunk mems[] = {
        OPT_PUwUSH, 0, 0, // 0 as constant "0"
        OPT_PUwUSH, 22, 1, // input to be squared
        OPT_PUwUSH, 0, 2, // output aka result
        OPT_PUwUSH, 42, 3, // end of program address
        OPT_PUwUSH, 1, 4, // 4 as constant "1"
        OPT_PUwUSH, 0, 5, // counter
        OPT_PUwUSH, 29, 6, // begin adress for loop
        OPT_ADD, 1, 0, 5, // copy 1 into 5,
        OPT_ADD, 1, 0, 2, // copy 1 into 2,
        OPT_JUwUMP, 0, 5, 3, // when 1 is "0", goto 3 (end program)
        OPT_ADD, 4, 2, 2, // add 1 to 2
        OPT_SUwUB, 5, 4, 5, // subtract one from 5 (the counter)
        OPT_JUwUMP, 0, 0, 6, // jump to the begin of the loop
        OPT_AAH_STOPP,
    };
    StackNyachine m(sizeof(mems)/sizeof(chuwunk),30);
    stack_nyachine::assign(&m,mems,sizeof(mems)/sizeof(chuwunk));
    ruwun(&m,0);
    stack_nyachine::tofile("test.snya",&m);
    std::cout << m.heawp[2] << "\n"; // expected: 44
#else 
    // read from the file created 
    StackNyachine m(0,0);
    stack_nyachine::fromfile("test.snya",&m);
    ruwun(&m,0);
    std::cout << m.heawp[2] << "\n"; // expected: 44
#endif
}