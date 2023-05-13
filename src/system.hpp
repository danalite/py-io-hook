#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

#include <map>
#include <iostream>
#include <unordered_map>

#include <uiohook.h>
#include "uiohook_worker.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using std::map;
template <typename T> struct Callback;

template <typename Ret, typename... Params>
struct Callback<Ret(Params...)> {
  template <typename... Args>
  static Ret callback(Args... args) {
    return func(args...);
  }
  static std::function<Ret(Params...)> func;
};

template <typename Ret, typename... Params>
std::function<Ret(Params...)> Callback<Ret(Params...)>::func;

// callback function pointer type
// https://stackoverflow.com/a/29817048
typedef void (*callback_t)(uiohook_event *const);


// Convert uiohook_event to python dict
pybind11::dict uiohook_event_to_dict(uiohook_event *const event) {
    pybind11::dict result;
    std::cout << "event->type: " << event->type << std::endl;
    // result["type"] = event->type;
    // result["time"] = event->time;
    // result["mask"] = event->mask;
    // result["reserved"] = event->reserved;
    return result;
}

class System {
public:
  using py_callback_t = std::function<void(int)>;
  System() : t_(), py_cb_(), stop_(true) {}
  ~System() { stop(); }

  void dispatch_process(uiohook_event *const event) {
    uiohook_event* copied_event = (uiohook_event*) malloc(sizeof(uiohook_event));
    memcpy(copied_event, event, sizeof(uiohook_event));
    if (copied_event->type == EVENT_MOUSE_DRAGGED) {
      copied_event->type = EVENT_MOUSE_MOVED;
    }

    // Event type: 
    //    1. Task start (by hotkey or scheduled)
    //    2. Mouse
    if (py_cb_) {
      // auto event_dict = uiohook_event_to_dict(copied_event);
       std::cout << copied_event->type << std::endl;
       py_cb_(12);
    }
  }

  bool start();
  bool start_event_loop();

  bool stop() {
    int status = uiohook_worker_stop();
    return status == UIOHOOK_SUCCESS;
  }

  // register callback from python
  bool register_callback(py_callback_t cb) {
    py_cb_ = cb;
    return true;
  }

private:
  std::thread t_;
  py_callback_t py_cb_;
  std::atomic_bool stop_;
  std::unordered_map<int, int> eventTable;
};

bool System::start_event_loop() {
  Callback<void(uiohook_event *const)>::func =
      std::bind(&System::dispatch_process, this, std::placeholders::_1);
  callback_t func =
      static_cast<callback_t>(Callback<void(uiohook_event *const)>::callback);

  int worker_status = uiohook_worker_start(func);
  if (worker_status != UIOHOOK_SUCCESS) {
    std::cout << "Failed to start worker thread" << std::endl;
  }

  // std::cout << "Event loop started!!!!!!!" << std::endl;
  return true;
}

bool System::start() {
  if (t_.joinable()) return false;
  stop_ = false;

  t_ = std::thread([this]() {
    while (!stop_) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      std::cout << "111" << std::endl;
     }
  });
  return true;
}