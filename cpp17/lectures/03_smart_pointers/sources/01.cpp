#include <iostream>
#include <memory>

struct Object
{
    int data[10000];

    void Print() const
    {
        std::cout << "some message" << std::endl;
    }
};


int main()
{
    {
        Object * obj = new Object();
        obj->Print();
        // memory leak
    }

    {
        auto obj = std::make_unique<Object>();
        obj->Print();
    }
}
