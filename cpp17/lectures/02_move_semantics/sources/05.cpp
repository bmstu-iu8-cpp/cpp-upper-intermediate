#include <iostream>
#include <string>
#include <vector>

struct Animal
{
    Animal(const char * str, size_t a)
        : name(str)
        , age(a)
    { }

    std::string name;
    size_t age;
};

template<typename ... Args>
Animal create_animal(Args&&... args)
{
    return Animal(std::forward<Args>(args)...);
}

int main()
{
    std::vector<Animal> zoo;
    zoo.push_back(Animal("cat", 10));
    zoo.emplace_back("dog", 3);
    
    auto b = create_animal("bird", 1);
    std::cout << b.name;
}