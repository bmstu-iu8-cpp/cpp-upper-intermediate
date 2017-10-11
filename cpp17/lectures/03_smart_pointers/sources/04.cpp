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

    auto ptrTwo = ptrOne;
    std::cout << ptrOne.use_count() << std::endl;

    std::weak_ptr<Object> weakPtr = ptrOne;
    std::cout << ptrOne.use_count() << std::endl;

    auto ptrThree = weakPtr.lock();
    std::cout << ptrOne.use_count() << std::endl;

    ptrOne.reset();
    ptrTwo.reset();
    ptrThree.reset();

    std::cout << std::boolalpha << weakPtr.expired() << std::endl;
    auto ptrFour = weakPtr.lock();
    std::cout << std::boolalpha << static_cast<bool>(ptrFour) << std::endl;

    return 0;
}
