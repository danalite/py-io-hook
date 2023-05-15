#include <pybind11/pybind11.h>

#include <iostream>

#include "pybind11/functional.h"
#include "uiohook_worker.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)
#define PYBIND11_DETAILED_ERROR_MESSAGES

namespace py = pybind11;

int add(int i, int j) { return i + j; }

PYBIND11_MODULE(pyiohook, m) {
  m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------
        .. currentmodule:: iohook
        .. autosummary::
           :toctree: _generate
           add
           subtract
    )pbdoc";

  m.def("add", &add, R"pbdoc(
        Add two numbers
        Some other explanation about the add function.
    )pbdoc");

  m.def(
      "subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers
        Some other explanation about the subtract function.
    )pbdoc");

  m.def(
      "start",
      [](const std::function<void(int)> callback) {
        dispatcher_t dispatcher = [](uiohook_event *const event) {
          std::cout << "event->type: " << event->type << std::endl;
          // callback(event->type);
        };

        int worker_status = uiohook_worker_start(dispatcher);
        if (worker_status != UIOHOOK_SUCCESS) {
          std::cout << "Failed to start worker thread" << std::endl;
        }
      },
      R"pbdoc(
        Start the event loop
    )pbdoc");

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}