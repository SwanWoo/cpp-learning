#include "main.h"

void HELLO::say_hello() {
    cout << "Hello C++ with CMake!" << endl;
}

int main() {
    HELLO hello;
    hello.say_hello();
    return 0;
}

