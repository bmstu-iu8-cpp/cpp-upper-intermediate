#include <string>

struct Engine {
  Engine(const std::string& model) : model_(model) {}
  Engine(std::string&& model) : model_(std::move(model)) {}

 private:
  std::string model_;
};

struct Car {
  Car(const std::string& engine_model) : engine_(engine_model) {}
  Car(std::string&& engine_model) : engine_(std::move(engine_model)) {}

 private:
  Engine engine_;
};

std::string get_engine_model_from_file() {
  return "id134";
}

int main() {
  Car car(get_engine_model_from_file());

  std::string model = "new awesome engine";
  Car new_car(model); // here call copy ctor of std::string
}