// Compile command: g++ addsub.cpp -o addsub.o

#include <iostream>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int main() {
    int a = 10, b = 5;
    std::cout << "Sum: " << add(a, b) << std::endl;
    std::cout << "Difference: " << subtract(a, b) << std::endl;
    return 0;
}
