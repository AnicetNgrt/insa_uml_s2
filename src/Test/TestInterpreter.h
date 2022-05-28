#pragma once

#include "../Dependencies/microtest.h"
#include "./MockService.h"
#include "../Lib/Interpreter.h"

TEST(interpreter_cmd_measurement_filter_by_sensor_id) {
    MockService s = MockService();
    Interpreter i = Interpreter(s);

    auto res = i.interpret("measurements -s A");
    if (failure(res)) {
        printf("%s\n", UnwrapError(res).c_str());
    }
    ASSERT(success(res));
    ASSERT_EQ("\n"+UnwrapValue(res), R"(
A NO2 2016-02-03 12:00:00 1
A PM10 2016-02-03 13:00:00 1
A NO2 2016-02-03 14:00:00 1
)");
}