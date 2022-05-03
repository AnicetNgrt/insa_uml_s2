#pragma once

#include "Maybe.h"
#include <functional>
#include <vector>

using namespace std;

template <typename ItemType> class Stream {
public:
  virtual ~Stream() {};

  virtual Maybe<ItemType> receive() = 0;
};

template <typename ItemType> class StreamClosure : public Stream<ItemType> {
public:
  typedef function<Maybe<ItemType>()> NextFn;
  typedef function<void()> CloseFn;

  StreamClosure()
      : _receive([]() -> Maybe<ItemType> { return None; }), close([]() {}) {}

  StreamClosure(NextFn receive, CloseFn close)
      : _receive(receive), close(close) {}

  StreamClosure(NextFn receive) : _receive(receive), close([]() {}) {}

  ~StreamClosure() { close(); }

  Maybe<ItemType> receive() override { return _receive(); };

private:
  NextFn const _receive;
  CloseFn const close;
};