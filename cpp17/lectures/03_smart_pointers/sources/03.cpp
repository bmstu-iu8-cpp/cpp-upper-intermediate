#include <iostream>
#include <memory>

struct Object
{
    ~Object()
    {
        std::cout << "~Object" << std::endl;
    }
    void Print() const
    {
        std::cout << "some message" << std::endl;
    }
};

int main()
{
    auto ptrOne = std::make_shared<Object>();
    std::cout << ptrOne.use_count() << std::endl;
    std::cout << std::boolalpha << ptrOne.unique() << std::endl;

    auto ptrTwo = ptrOne;
    std::cout << ptrOne.use_count() << std::endl;
    std::cout << std::boolalpha << ptrOne.unique() << std::endl;

    auto ptrThree = ptrOne;
    std::cout << ptrOne.use_count() << std::endl;
    std::cout << std::boolalpha << ptrOne.unique() << std::endl;

    ptrThree->Print();
    
    ptrOne.reset();
    std::cout << "one" << std::endl;
    ptrTwo.reset();
    std::cout << "two" << std::endl;
    ptrThree.reset();
    std::cout << "three" << std::endl;

    return 0;
}
