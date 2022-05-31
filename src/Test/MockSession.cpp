#include "MockSession.h"

Session mock_session(Database<User>* users, Database<Cleaner>* cleaners, Database<Provider>* providers, Database<Measurement>* measurements, Database<Sensor>* sensors)
{
    if (users == nullptr)
        users = new MockDatabase<User>({});
    if (cleaners == nullptr)
        cleaners = new MockDatabase<Cleaner>({});
    if (providers == nullptr)
        providers = new MockDatabase<Provider>({});
    if (measurements == nullptr)
        measurements = new MockDatabase<Measurement>({});
    if (sensors == nullptr)
        sensors = new MockDatabase<Sensor>({});
    return {
        users,
        cleaners,
        providers,
        measurements,
        sensors,
        None,
        unordered_map<string, ProviderFlag>()
    };
}