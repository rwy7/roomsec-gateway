/* -*- Mode: c++ -*- */
#ifndef _ROOMSEC_QUEUE_H_
#define _ROOMSEC_QUEUE_H_

#include <queue>
#include <boost/thread.hpp>

template<typename T>
class Queue {
private:
  std::queue<T> queue;
  mutable boost::mutex mutex;
  boost::condition_variable nonempty;

public:

  T front() {
    boost::unique_lock<boost::mutex> lock(this->mutex);
    while (this->queue.empty()) {
      this->nonempty.wait(lock);
    }
    return this->queue.front();
  }

  T back() const {
    boost::lock_guard<boost::mutex> guard(this->mutex);
    return this->queue.back();
  }

  void push(T const& data) {
    /* Atomic Section */
    {
      boost::lock_guard<boost::mutex> guard(this->mutex);
      this->queue.push(data);
    }

    this->nonempty.notify_one();
    return;
  }

  void pop() {
    boost::lock_guard<boost::mutex> guard(this->mutex);
    this->queue.pop();
    return;
  }

  T front_pop() {
    boost::unique_lock<boost::mutex> lock(this->mutex);
    while (this->queue.empty()) {
      this->nonempty.wait(lock);
    }
    T returnValue = this->queue.front();
    this->queue.pop();
    return returnValue;
  }

  bool empty() const {
    boost::lock_guard<boost::mutex> guard(this->mutex);
    return this->queue.empty();
  }
};

#endif /* _ROOMSEC_QUEUE_H_ */
