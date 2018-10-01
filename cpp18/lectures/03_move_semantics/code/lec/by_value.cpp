#include <string>

std::string get_engine_model_from_file()
{
    return "id134";
}

struct Engine
{
    Engine(std::string model) : model_(std::move(model)) { }

private:
    std::string model_;
};

struct Car
{
    Car(std::string engine_model) : engine_(std::move(engine_model)) { }

private:
    Engine engine_;
};

int main()
{
    Car car(get_engine_model_from_file()); // here call move ctor of std::string

    std::string model = "new awesome engine";
    Car new_car(model); // here call copy ctor of std::string
}