#include "AirQuality.h"

typedef struct { double total; int count; } AverageData;

AirQuality air_quality_compute(Stream<Measurement>* measurements)
{
    unordered_map<MeasurementType, AverageData> averages = {
        { MeasurementType::NO2, { 0.0, 0 } },
        { MeasurementType::O3, { 0.0, 0 } },
        { MeasurementType::PM10, { 0.0, 0 } },
        { MeasurementType::SO2, { 0.0, 0 } }
    };

    Maybe<Measurement> maybe_m = None;
    while (some((maybe_m = measurements->receive()))) {
        Measurement m = Unwrap(maybe_m);
        auto avg = averages[m.get_type()];
        avg.total += m.get_value();
        avg.count += 1;
        averages[m.get_type()] = avg;
    }

    auto avg = averages[MeasurementType::NO2];
    double avg_no2 = avg.count != 0 ?  ((avg.total / (double)avg.count) - 45)*15 : 0;
    
    avg = averages[MeasurementType::O3];
    double avg_o3 = avg.count != 0 ? ((avg.total / (double)avg.count) - 49)*15 : 0;

    avg = averages[MeasurementType::PM10];
    double avg_pm10 = avg.count != 0 ? ((avg.total / (double)avg.count) - 53)*15 : 0;

    avg = averages[MeasurementType::SO2]; 
    double avg_so2 = avg.count != 0 ? ((avg.total / (double)avg.count) - 50)*15 : 0;

    // https://www.libelium.com/wp-content/uploads/2021/04/AQI-USA-Table.png
    if (avg_no2 > 1250 || avg_o3 > 405 || avg_pm10 > 425 || avg_so2 > 605)
        return AirQuality::HAZARDOUS;
    if (avg_no2 > 650 || avg_o3 > 106 || avg_pm10 > 355 || avg_so2 > 305)
        return AirQuality::VERY_UNHEALTHY;
    if (avg_no2 > 361 || avg_o3 > 86 || avg_pm10 > 255 || avg_so2 > 186)
        return AirQuality::UNHEALTHY;
    if (avg_no2 > 101 || avg_o3 > 71 || avg_pm10 > 155 || avg_so2 > 76)
        return AirQuality::UNHEALTHY_FOR_SENSITIVES;
    if (avg_no2 > 54 || avg_o3 > 55 || avg_pm10 > 55 || avg_so2 > 36)
        return AirQuality::MODERATE;
    return AirQuality::GOOD;
}

string air_quality_to_string(AirQuality air_quality)
{
    switch (air_quality) {
    case AirQuality::GOOD:
        return "good";
    case AirQuality::MODERATE:
        return "moderate";
    case AirQuality::UNHEALTHY_FOR_SENSITIVES:
        return "unhealthy for sensitives";
    case AirQuality::UNHEALTHY:
        return "unhealthy";
    case AirQuality::VERY_UNHEALTHY:
        return "very unhealthy";
    case AirQuality::HAZARDOUS:
        return "hazardous";
    }
    return "unknown";
}
