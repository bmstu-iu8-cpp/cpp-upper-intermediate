#include <iostream>
#include <vector>
#include <string>

struct Sentence
{
    std::vector<std::string> data;
    
    template <class T>
    void add_word(T&& word)
    {
        data.push_back(std::forward<T>(word));
    }

    friend std::ostream& operator << (std::ostream& out, const Sentence& sent);
};

std::string world_word()
{
    return std::string("World");
}

int main()
{
    Sentence s;
    std::string hello = "hello";
    s.add_word(hello);
    s.add_word(",");
    s.add_word(world_word());

    std::cout << s;
}

std::ostream& operator << (std::ostream& out, const Sentence& sent)
{
    for (const auto& w : sent.data)
        out << w << " ";
    return out;
}
