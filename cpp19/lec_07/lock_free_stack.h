#pragma once
#include <atomic>
#include <memory>
#include <utility>

template <typename T>
struct LockFreeStack {
  void Push(const T& value) {
    Node* new_head = new Node(value, head_.load());

    while (!head_.compare_exchange_weak(new_head->next, new_head)) {
    }
  }

  Node* Pop() {
    Node* node = head_.load();

    while (node && !head_.compare_exchange_weak(node, node->next)) {
    }

    return node;
  }

 private:
  struct Node {
    T data;
    Node* next;
  };

  std::atomic<Node*> head_;
};