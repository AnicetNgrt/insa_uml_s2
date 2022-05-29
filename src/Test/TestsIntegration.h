#pragma once

#include "../Dependencies/microtest.h"
#include "./MockSession.h"

TEST(integ_cmd_measurement_filter_by_sensor_id)
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

TEST(integ_cmd_measurement_filter_by_timestamp)
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

TEST(integ_cmd_measurement_filter_by_type)
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

    auto res = i.interpret("measurements -t");
    ASSERT(failure(res));
    ASSERT_EQ(UnwrapError(res), "Measurement type (argument -t) had no value given");

    res = i.interpret("measurements -t NO2");
    ASSERT(success(res));
    ASSERT_EQ("\n" + UnwrapValue(res), R"(
A NO2 2016-02-03 12:00:00 1
A NO2 2016-02-03 14:00:00 1
)");
}

TEST(integ_cmd_reconnect)
{
    auto users = new MockDatabase<User>({
        User("Mik", default_password_hasher("1313"), UserPermissionLevel::BASIC),
        User("Mike", default_password_hasher("12345"), UserPermissionLevel::BASIC),
        User("Mikey", default_password_hasher("310731"), UserPermissionLevel::GOVERNMENT)
    });
    Session session = mock_session(users, 0, 0, 0, 0);
    Service s = Service(session);
    Interpreter i = Interpreter(s);

    auto res = i.interpret("reconnect -u Mike -p 12345");
    ASSERT(success(res));
    ASSERT_STREQ(UnwrapValue(res), "Connected as Mike");

    res = i.interpret("reconnect -u Patrick -p 12345");
    ASSERT(failure(res));
    ASSERT_STREQ(UnwrapError(res), "User Patrick was not found");

    res = i.interpret("reconnect -u Mike -p 54321");
    ASSERT(failure(res));
    ASSERT_STREQ(UnwrapError(res), "Incorrect password for user Mike");
}