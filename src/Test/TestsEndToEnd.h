#pragma once

#include "../Dependencies/microtest.h"
#include "./MockSession.h"

TEST(cmd_measurement_filter_by_sensor_id)
{   
    auto measurements = new MockDatabase<Measurement>({
        Measurement(MeasurementType::NO2, "A", 1, timestamp_from_string("2016-02-03 12:00:00")),
        Measurement(MeasurementType::PM10, "A", 1, timestamp_from_string("2016-02-03 13:00:00")),
        Measurement(MeasurementType::NO2, "A", 1, timestamp_from_string("2016-02-03 14:00:00")),
        Measurement(MeasurementType::O3, "Z", 1, timestamp_from_string("2016-02-03 14:00:00"))
    });
    Session session = mock_session(0, 0, 0, measurements, 0);
    Service s = Service(session);
    Interpreter i = Interpreter(s);

    auto res = i.interpret("measurements -s");
    ASSERT(failure(res));
    ASSERT_EQ(UnwrapError(res), "Sensor id (argument -s) had no value given");

    res = i.interpret("measurements -s A");
    ASSERT(success(res));
    ASSERT_EQ("\n" + UnwrapValue(res), R"(
A NO2 2016-02-03 12:00:00 1
A PM10 2016-02-03 13:00:00 1
A NO2 2016-02-03 14:00:00 1
)");

    res = i.interpret("measurements -s Z");
    ASSERT(success(res));
    ASSERT_EQ("\n" + UnwrapValue(res), R"(
Z O3 2016-02-03 14:00:00 1
)");
}

TEST(cmd_measurement_filter_by_timestamp)
{
    auto measurements = new MockDatabase<Measurement>({
        Measurement(MeasurementType::NO2, "A", 1, timestamp_from_string("2016-02-03 12:00:00")),
        Measurement(MeasurementType::PM10, "A", 1, timestamp_from_string("2016-02-03 13:00:00")),
        Measurement(MeasurementType::NO2, "A", 1, timestamp_from_string("2016-02-03 14:00:00")),
        Measurement(MeasurementType::O3, "Z", 1, timestamp_from_string("2016-02-03 14:00:00"))
    });
    Session session = mock_session(0, 0, 0, measurements, 0);
    Service s = Service(session);
    Interpreter i = Interpreter(s);

    auto res = i.interpret("measurements -ts");
    ASSERT(failure(res));
    ASSERT_EQ(UnwrapError(res), "Timestamp (argument -ts) had no value given");

    res = i.interpret("measurements -ts '2016-02-03 14:00:00'");
    ASSERT(success(res));
    ASSERT_EQ("\n" + UnwrapValue(res), R"(
A NO2 2016-02-03 14:00:00 1
Z O3 2016-02-03 14:00:00 1
)");
}