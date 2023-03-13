#include <iostream>
#include "TCPScanner.h"

int main(int argc, char** argv) {
    TCPScanner T{};
    T.start(argv[1]);
    return 0;
}

