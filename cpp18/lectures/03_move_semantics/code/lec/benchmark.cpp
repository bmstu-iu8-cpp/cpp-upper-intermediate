#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>


int main() {
  const int N = 1 << 23;
  const int K = 25;

  for (int k = 0; k < K; ++k) {
    // copy benchmark
    {
      std::vector<std::string> vec(N);
      auto startTime = std::chrono::high_resolution_clock::now();
      srand(1);
      for (int i = 0; i < N; ++i) {
        std::string s = std::to_string(rand());
        vec[i] = s;
        s.clear();
      }
      auto endTime = std::chrono::high_resolution_clock::now();
      auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
                      endTime - startTime)
                      .count();
      std::cout << "copy - " << diff << std::endl;
    }

    // move benchmark
    {
      std::vector<std::string> vec(N);
      auto startTime = std::chrono::high_resolution_clock::now();
      srand(1);
      for (int i = 0; i < N; ++i) {
        std::string s = std::to_string(rand());
        vec[i] = std::move(s);
        s.clear();
      }
      auto endTime = std::chrono::high_resolution_clock::now();
      auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
                      endTime - startTime)
                      .count();
      std::cout << "move - " << diff << std::endl;
    }
  }
  system("pause");

  return 0;
}
