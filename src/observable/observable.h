#ifndef OBSERVABLE_H
#define OBSERVABLE_H
#include <map>
#include <memory>
#include <functional>
template <typename T>

class Observable {
 public:
  void subscribe(uint32_t id, std::unique_ptr<std::function<void(T*)>> callback) {
    listeners[id] = std::move(callback);
  };
  void unsubscribe(uint32_t id) {
    listeners.erase(id);
  };
  void notifyListeners() {
    for (auto& listener : listeners) {
      listener.second->operator()(&data);
    }
  };
  T* getData() {
    return &data;
  };

 protected:
  T data;
 private:
  std::map<uint32_t, std::unique_ptr<std::function<void(T*)>>> listeners;
};

#endif