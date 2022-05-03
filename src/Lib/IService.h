#pragma once

#include <string>

using namespace std;

#include "Stream.h"
#include "Measurement.h"

class IService {
public:
  virtual Stream<Measurement>* measurements();
};