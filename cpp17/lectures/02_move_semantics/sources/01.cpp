#include <iostream>

struct MyStruct
{
    MyStruct()
    {
        std::cout << "default ctor" << std::endl;
    }

    MyStruct(const MyStruct& oth)
    {
        std::cout << "copy ctor" << std::endl;
    }

    MyStruct(MyStruct&& oth)
    {
        std::cout << "move ctor" << std::endl;
    }

    MyStruct& operator=(MyStruct&& oth)
    {
        if (&oth == this)
            return *this;
        std::cout << "move operator" << std::endl;
        return *this;
    }

    MyStruct& operator=(const MyStruct& oth)
    {
        if (&oth == this)
            return *this;
        std::cout << "copy operator" << std::endl;
        return *this;
    }

    ~MyStruct()
    {
        std::cout << "dtor" << std::endl;
    }
};

MyStruct createStruct()
{
    return MyStruct();
}

int main()
{
    MyStruct a;
    MyStruct b(a);
    MyStruct c(std::move(a));
    b = c;
    c = std::move(b);
    c = createStruct();
    system("pause");
}