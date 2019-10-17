#include <future>
#include <iostream>
#include <string>
#include <thread>

struct Match {
  std::string title;
  int relevance;
};

std::vector<Match> RemoteQuery(std::string input_text) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(100ms);

  std::vector<Match> matches = {
      {std::move(input_text), 1500},
      {"hello world", 650},
  };
  return matches;
}

std::vector<Match> QuickAndSyncDBQuery() {
  std::vector<Match> matches = {
      {"habr.com", 1000},
      {"hello.ru", 850},
  };
  return matches;
}

void MakeSuggest(std::vector<Match> matches) {
  std::sort(matches.begin(), matches.end(), [](const Match& a, const Match& b) {
    return a.relevance > b.relevance;
  });

  for (const auto& match : matches) {
    std::cout << match.title << std::endl;
  }
  std::cout << "---------------------" << std::endl;
}

int main() {
  std::string input = "h";

  // Создаем асинхронный запрос.
  std::future<std::vector<Match>> f = std::async(RemoteQuery, std::move(input));

  // Пока выполняется асинхронный вызов, выполняем полезный код.
  auto matches = QuickAndSyncDBQuery();
  MakeSuggest(matches);

  // Больше нечего выполнять, нужен результат асинхронного вызова.
  auto remote_matches = f.get();
  matches.insert(matches.end(), remote_matches.begin(), remote_matches.end());
  MakeSuggest(matches);

  return 0;
}
