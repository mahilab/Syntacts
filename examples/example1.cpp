#include <iostream>
#include <vector>

int main(int argc, char const *argv[])
{
    std::cout << "Hello, World" << std::endl;
    std::vector<double> x = {1,2,3,4,5,};
    for (auto& i : x)
        std::cout << i << std::endl;
        std::cout << "Hello, Evan" << std::endl;

    return 0;
}
