#include <iostream>
#include <memory>
#include <vector>


void foo(std::unique_ptr<std::vector<int>> ptr)
{
    std::cout << ptr->size() << std::endl;
}

int main()
{
    auto ptr = std::make_unique<std::vector<int>>(100000);
    std::cout << std::boolalpha << static_cast<bool>(ptr) << std::endl;

    // foo(ptr); // ptr is lvalue. error
    foo(std::move(ptr));

    std::cout << std::boolalpha << static_cast<bool>(ptr) << std::endl;

    return 0;
}
