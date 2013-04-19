/* -*- Mode: c++ -*- */
#ifndef _ROOMSEC_QUEUE_H_
#define _ROOMSEC_QUEUE_H_

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace roomsec {

  template<typename T>
  class Queue {
  private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable nonempty;

  public:

    T front() {
      std::unique_lock<std::mutex> lock(this->mutex);
      while (this->queue.empty()) {
	this->nonempty.wait(lock);
      }
      return this->queue.front();
    }

    T back() const {
      std::unique_lock<std::mutex> lock(this->mutex);
      return this->queue.back();
    }

    void push(T const& data) {
      std::unique_lock<std::mutex> lock(this->mutex);
      this->queue.push(data);
      this->nonempty.notify_one();
      return;
    }

    void pop() {
      std::unique_lock<std::mutex> lock(this->mutex);
      this->queue.pop();
      return;
    }

    T front_pop() {
      std::unique_lock<std::mutex> lock(this->mutex);
      while (this->queue.empty()) {
	this->nonempty.wait(lock);
      }
      T returnValue = this->queue.front();
      this->queue.pop();
      return returnValue;
    }

    bool empty() const {
      std::unique_lock<std::mutex> lock(this->mutex);
      return this->queue.empty();
    }
  };
}

#endif /* _ROOMSEC_QUEUE_H_ */
