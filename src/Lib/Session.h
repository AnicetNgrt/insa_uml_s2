#pragma once

#include <string>

using namespace std;

#include "Cleaner.h"
#include "Command.h"
#include "../Utils/Database.h"
#include "../Utils/Maybe.h"
#include "Measurement.h"
#include "Provider.h"
#include "../Utils/Result.h"
#include "Sensor.h"
#include "User.h"

struct Session {
    Database<User> const* users_db;
    Database<Cleaner> const* cleaners_db;
    Database<Provider> const* providers_db;
    Database<Measurement> const* measurements_db;
    Database<Sensor> const* sensors_db;

    Maybe<User> authed_user;
    unordered_map<string, ProviderFlag> providers_flags;
};

void end_session(Session& session);

Result<Session, string> from_command(Command& cmd);