#pragma once

#include "Maybe.h"
#include <functional>

using namespace std;

template <typename ItemType> class Stream {
public:
  typedef function<Maybe<ItemType>()> NextFn;
  typedef function<void()> CloseFn;

  Stream() {
    receive = []() -> Maybe<ItemType> { return None; };
    close = []() {};
  };

  Stream(NextFn receive, CloseFn close) : receive(receive), close(close){};
  ~Stream() { close(); }

  NextFn const receive;

private:
  CloseFn const close;
};