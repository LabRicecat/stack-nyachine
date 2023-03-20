#include "stack-nyachine.hpp"
#include <iostream>

int main() {
    using namespace stack_nyachine;
    // simple program to square a number
    chuwunk mems[] = {
        OPT_PUwUSH8, 0, 0, // 0 as constant "0"
        OPT_PUwUSH8, 10, 1, // input to be squared
        OPT_PUwUSH8, 0, 2, // output aka result
        OPT_PUwUSH8, 45, 3, // end of program address
        OPT_PUwUSH8, 1, 4, // 4 as constant "1"
        OPT_PUwUSH8, 0, 5, // counter
        OPT_PUwUSH8, 25, 6, // begin adress for loop
        OPT_ADD8, 1, 0, 5, // copy 1 into 5
        OPT_ABS32_JUwUMP, 0, 5, 0, 0, 3, // when 1 is "0", goto 3 (end program)
        OPT_ADD8, 1, 2, 2, // add 1 to 2
        OPT_SUwUB8, 5, 4, 5, // subtract one from 5 (the counter)
        OPT_ABS32_JUwUMP, 0, 0, 0, 0, 6, // jump to the begin of the loop
        OPT_AAH_STOPP,
    };
    StackNyachine m(sizeof(mems)/sizeof(chuwunk),30);
    assign(&m,mems,sizeof(mems)/sizeof(chuwunk));
    // tofile("test.snya",&m);
    ruwun(&m,0);
    std::cout << (int)m.heawp[2] << "\n"; // expected: 100
}