#pragma once

#include <functional>
#include <string>

using namespace std;

#include "Stream.h"

template <typename Data> 
class Database {
public:
  virtual Stream<Data> *
  filter_and_stream(function<bool(Data const &)> filter) const = 0;
  virtual Stream<Data> *stream() const = 0;
};