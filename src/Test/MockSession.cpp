#include "MockSession.h"

Session mock_session(Database<User>* users, Database<Cleaner>* cleaners, Database<Owner>* owners, Database<Measurement>* measurements, Database<Sensor>* sensors)
{
    if (users == nullptr)
        users = new MockDatabase<User>({});
    if (cleaners == nullptr)
        cleaners = new MockDatabase<Cleaner>({});
    if (owners == nullptr)
        owners = new MockDatabase<Owner>({});
    if (measurements == nullptr)
        measurements = new MockDatabase<Measurement>({});
    if (sensors == nullptr)
        sensors = new MockDatabase<Sensor>({});
    return {
        "mock_username",
        "mock_password",
        users,
        cleaners,
        owners,
        measurements,
        sensors,
        None,
        unordered_map<string, OwnerFlag>()
    };
}