#include <iostream>
#include <vector>

int main(int argc, char const *argv[])
{
    std::cout << "Hello, World" << std::endl;
    std::vector<double> x = {1,2,3,4,5,};
    for (auto& i : x)
        std::cout << i << std::endl;

<<<<<<< HEAD
        std::cout << "Hello, Evan" << std::endl;
=======
    std::cout << "Hello, Brandon" << std::endl;
>>>>>>> 08d34ca3c7e3e543cebf801ec2952318ab9a27fd
    return 0;
}
