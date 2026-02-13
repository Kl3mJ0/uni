#include <sstream>
#include <iostream>
#include "adder.h" // The worksheet left this out on purpose to trigger an error!

int main(int argc, char *argv[]) {
    int a, b, c;
    
    // Check if we provided two numbers
    if (argc != 3) return 1;
    
    std::string sa(argv[1]);
    std::string sb(argv[2]);
    
    std::stringstream ssa(sa);
    std::stringstream ssb(sb);
    
    ssa >> a;
    ssb >> b;
    
    // Call the add function from adder.cpp
    c = add(a, b);
    
    std::cout << c << std::endl;
    
    return 0;
}