#pragma once

#include <string>
#include <unordered_map>

#include "Measurement.h"
#include "Stream.h"

using namespace std;

// https://www.libelium.com/wp-content/uploads/2021/04/AQI-USA-Table.png
enum class AirQuality {
    GOOD,
    MODERATE,
    UNHEALTHY_FOR_SENSITIVES,
    UNHEALTHY,
    VERY_UNHEALTHY,
    HAZARDOUS
};

typedef AirQuality (*AirQualityComputer)(Stream<Measurement>&);

AirQuality air_quality_compute(Stream<Measurement>& measurements);
string air_quality_to_string(AirQuality air_quality);