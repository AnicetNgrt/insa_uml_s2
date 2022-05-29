#pragma once

#include "../Dependencies/microtest.h"
#include "../Lib/Command.h"

TEST(unit_command_expected_name_and_args)
{
    Command c = Command("bidule -o truc -p machin");

    ASSERT_STREQ(c.get_name(), "bidule");

    vector<string> expected_args = {
        "-o", "truc", "-p", "machin"
    };
    auto args = c.get_args();

    for (size_t i = 0; i < expected_args.size() && i < args.size(); i++) {
        ASSERT_STREQ(expected_args[i], args[i]);
    }

    ASSERT_EQ(expected_args.size(), args.size());
}

TEST(unit_command_expected_name_and_args_with_quotes)
{
    Command c = Command("bidule -o truc -p \"machin chouette\" -k okay --p \'voili voilou\'");

    ASSERT_STREQ(c.get_name(), "bidule");

    vector<string> expected_args = {
        "-o", "truc", "-p", "machin chouette", "-k", "okay", "--p", "voili voilou"
    };
    auto args = c.get_args();

    for (size_t i = 0; i < expected_args.size() && i < args.size(); i++) {
        ASSERT_STREQ(expected_args[i], args[i]);
    }

    ASSERT_EQ(expected_args.size(), args.size());
}

TEST(unit_command_find_args_string)
{
    Command c = Command("bidule -o truc -p \"machin chouette\" -k okay --p \'voili voilou\' -l");

    auto res = c.find_arg("-o");
    ASSERT(success(res));
    ASSERT_STREQ(UnwrapValue(res), "truc");

    res = c.find_arg("-p");
    ASSERT(success(res));
    ASSERT_STREQ(UnwrapValue(res), "machin chouette");

    res = c.find_arg("-k");
    ASSERT(success(res));
    ASSERT_STREQ(UnwrapValue(res), "okay");

    res = c.find_arg("--p");
    ASSERT(success(res));
    ASSERT_STREQ(UnwrapValue(res), "voili voilou");

    res = c.find_arg("-i");
    ASSERT(failure(res));
    ASSERT((UnwrapError(res) == ArgError::ARG_NOT_FOUND));

    res = c.find_arg("-l");
    ASSERT(failure(res));
    ASSERT((UnwrapError(res) == ArgError::VALUE_NOT_FOUND));
}

TEST(unit_command_find_args_double)
{
    Command c = Command("bidule -x AAAA -y 18.373");

    auto res = c.find_double("-x");
    ASSERT(failure(res));
    ASSERT((UnwrapError(res) == ArgError::VALUE_NOT_PARSABLE));

    res = c.find_double("-y");
    ASSERT(success(res));
    ASSERT_EQ(UnwrapValue(res), 18.373);
}

TEST(unit_command_find_args_timestamp)
{
    Command c = Command("bidule -t1 AAAA -t2 '2019-04-05 18:32:53'");

    auto res = c.find_timestamp("-t1");
    ASSERT(failure(res));
    ASSERT((UnwrapError(res) == ArgError::VALUE_NOT_PARSABLE));

    res = c.find_timestamp("-t2");
    ASSERT(success(res));
    ASSERT_TRUE(timestamp_equal(UnwrapValue(res), { 2019, 4, 5, 18, 32, 53 }));
}