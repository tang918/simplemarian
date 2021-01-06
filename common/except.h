#pragma once
#include <stdexcept>
#include<sstream>
#include<iostream>
namespace marian{

class out_of_memory : public std::runtime_error {
 public:
  out_of_memory(const std::string& what_arg) : runtime_error(what_arg) {}
};

class cuda_not_implemented : public std::logic_error {
 public:
  cuda_not_implemented(const std::string& what_arg) : logic_error(what_arg) {}
};
class cuda_exception : public std::runtime_error {
 public:
  cuda_exception(const std::string& what_arg) : runtime_error(what_arg) {}
};
}//namespace marian

#define MARIAN_RUNTIME_ERR(msg) do {             \
    std::ostringstream oss;                     \
    oss << msg;                                 \
    throw std::runtime_error(oss.str()); }      \
  while (0);
