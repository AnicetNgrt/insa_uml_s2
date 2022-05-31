#pragma once

#include "../Lib/Interpreter.h"
#include "../Lib/Service.h"
#include "../Lib/Session.h"
#include "./MockDatabase.h"

Session mock_session(Database<User>* users, Database<Cleaner>* cleaners, Database<Provider>* providers, Database<Measurement>* measurements, Database<Sensor>* sensors);
