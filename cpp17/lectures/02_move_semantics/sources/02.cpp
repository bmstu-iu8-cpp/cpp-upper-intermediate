#include <memory>
#include <cstring>
#pragma warning(disable:4996)

class String
{
    char * data = nullptr;
public:
    String(const String& rhs)
    {
        if (!rhs.data)
            return;

        data = new char[strlen(rhs.data) + 1];
        strcpy(data, rhs.data);
    }

    String(String&& rhs)
    {
        data = rhs.data;
        rhs.data = nullptr;
        // shorter std::swap(data, rhs.data);
    }

    String(const char * const str)
    {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    ~String()
    {
        // need to implement!!!
    }
};

int main()
{
    {
        String s("string");
        String t = s;
        String u = std::move(s);
    }
}