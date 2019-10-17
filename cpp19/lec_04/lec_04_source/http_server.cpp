#include <future>
#include <iostream>
#include <string>
#include <thread>

void Handler(std::string request) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(100ms);

  std::cout << request << std::endl;
}

void EchoServerStart() {
  while (true) {
    std::string request;

    // Ждем входящее соединение.
    std::cin >> request;

    // Запускаем новый поток, который обработает входящий запрос.
    std::thread th(Handler, std::move(request));

    // Текужему потоку не нужен результат выполнения запущенного потока,
    // поэтому вызываем detach.
    th.detach();
  }
}

int main() {
  EchoServerStart();
}
