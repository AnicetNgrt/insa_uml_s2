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

    auto res = i.interpret("measurements -s A");
    if (failure(res)) {
        printf("%s\n", UnwrapError(res).c_str());
    }
    ASSERT(success(res));
    ASSERT_EQ("\n" + UnwrapValue(res), R"(
A NO2 2016-02-03 12:00:00 1
A PM10 2016-02-03 13:00:00 1
A NO2 2016-02-03 14:00:00 1
)");

    res = i.interpret("measurements -s Z");
    if (failure(res)) {
        printf("%s\n", UnwrapError(res).c_str());
    }
    ASSERT(success(res));
    ASSERT_EQ("\n" + UnwrapValue(res), R"(
Z O3 2016-02-03 14:00:00 1
)");
}