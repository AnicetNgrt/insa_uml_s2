#include "AirQuality.h"

AirQuality air_quality_compute(Stream<Measurement>* measurements)
{
    unordered_map<MeasurementType, pair<double, int>> averages;
    auto calculate_average = [](pair<double, int> data) -> double {
        return data.second != 0 ? data.first / (double)data.second : 0;
    };

    Maybe<Measurement> maybe_m = None;
    while (some((maybe_m = measurements->receive()))) {
        Measurement m = Unwrap(maybe_m);
        auto avg = averages[m.get_type()];
        avg.first += m.get_value(); // add to cumulated value
        avg.second += 1; // add to count
        averages[m.get_type()] = avg;
    }

    if (averages.find(MeasurementType::NO2) == averages.end())
        averages[MeasurementType::NO2] = make_pair(0.0, 0);
    double avg_no2 = calculate_average(averages[MeasurementType::NO2]);
    if (averages.find(MeasurementType::O3) == averages.end())
        averages[MeasurementType::O3] = make_pair(0.0, 0);
    double avg_o3 = calculate_average(averages[MeasurementType::O3]);
    if (averages.find(MeasurementType::PM10) == averages.end())
        averages[MeasurementType::PM10] = make_pair(0.0, 0);
    double avg_pm10 = calculate_average(averages[MeasurementType::PM10]);
    if (averages.find(MeasurementType::SO2) == averages.end())
        averages[MeasurementType::SO2] = make_pair(0.0, 0);
    double avg_so2 = calculate_average(averages[MeasurementType::SO2]);

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
}
