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

TEST(integ_cmd_login)
{
    auto users = new MockDatabase<User>({
        User("Mik", default_password_hasher("1313"), UserPermissionLevel::BASIC),
        User("Mike", default_password_hasher("12345"), UserPermissionLevel::BASIC),
        User("Mikey", default_password_hasher("310731"), UserPermissionLevel::GOVERNMENT)
    });
    Session session = mock_session(users, 0, 0, 0, 0);
    Service s = Service(session);
    Interpreter i = Interpreter(s);

    ASSERT(none(s.authenticated_user()));

    auto res = i.interpret("login -u Mike -p 12345");
    ASSERT(success(res));
    ASSERT_STREQ(UnwrapValue(res), "Connected as Mike");

    res = i.interpret("login -u Patrick -p 12345");
    ASSERT(failure(res));
    ASSERT_STREQ(UnwrapError(res), "User Patrick could not be found");

    res = i.interpret("login -u Mike -p 54321");
    ASSERT(failure(res));
    ASSERT_STREQ(UnwrapError(res), "Incorrect password for user Mike");
}

TEST(integ_cmd_flag_provider)
{
    auto providers = new MockDatabase<Provider>({
        Provider("A")
    });
    Session session = mock_session(0, 0, providers, 0, 0);
    Service s = Service(session);
    Interpreter i = Interpreter(s);

    auto res2 = i.interpret("provider-flag -o A");
    ASSERT(success(res2));
    ASSERT_STREQ(UnwrapValue(res2), "Provider A is flagged as reliable");

    session.authed_user = Some(User("", "", UserPermissionLevel::GOVERNMENT));
    auto res = i.interpret("flag-provider -o A -f unreliable");
    ASSERT(success(res));
    ASSERT_STREQ(UnwrapValue(res), "Provider A was just flagged");
    
    res2 = i.interpret("provider-flag -o A");
    ASSERT(success(res2));
    ASSERT_STREQ(UnwrapValue(res2), "Provider A is flagged as unreliable");
    
    res = i.interpret("flag-provider -o A -f reliable");
    ASSERT(success(res));
    ASSERT_STREQ(UnwrapValue(res), "Provider A was just flagged");
    
    res2 = i.interpret("provider-flag -o A");
    ASSERT(success(res2));
    ASSERT_STREQ(UnwrapValue(res2), "Provider A is flagged as reliable");
}

TEST(integ_cmd_flag_provider_not_found)
{
    auto providers = new MockDatabase<Provider>({
        Provider("A")
    });
    Session session = mock_session(0, 0, providers, 0, 0);
    Service s = Service(session);
    Interpreter i = Interpreter(s);

    auto res2 = i.interpret("provider-flag -o B");
    ASSERT(failure(res2));
    ASSERT_STREQ(UnwrapError(res2), "Provider B could not be found");

    session.authed_user = Some(User("", "", UserPermissionLevel::GOVERNMENT));
    auto res = i.interpret("flag-provider -o B -f unreliable");
    ASSERT(failure(res));
    ASSERT_STREQ(UnwrapError(res), "Provider B could not be found");
}

TEST(integ_cmd_flag_provider_permission)
{
    auto providers = new MockDatabase<Provider>({
        Provider("A")
    });
    Session session = mock_session(0, 0, providers, 0, 0);
    Service s = Service(session);
    Interpreter i = Interpreter(s);
    
    session.authed_user = Some(User("", "", UserPermissionLevel::BASIC));
    auto res = i.interpret("flag-provider -o A -f unreliable");
    ASSERT(failure(res));
    ASSERT_STREQ(UnwrapError(res), "Permission denied");
}