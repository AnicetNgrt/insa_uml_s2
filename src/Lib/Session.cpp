#include "Session.h"

#include "../Utils/DatabaseCSV.h"

Result<Session, string> from_command(Command& cmd)
{
    auto maybe_dataset_dir = cmd.find_arg("-d");
    if (failure(maybe_dataset_dir) && !is_error(maybe_dataset_dir, ArgError::ARG_NOT_FOUND))
        return map_error(maybe_dataset_dir, arg_error_to_string("-d", "Dataset directory"));

    auto dataset_dir = failure(maybe_dataset_dir) ? "./dataset/" : UnwrapValue(maybe_dataset_dir);

    Session opened_session = {
        new DatabaseCSV<User>(dataset_dir + "/users.csv"),
        new DatabaseCSV<Cleaner>(dataset_dir + "/cleaners.csv"),
        new DatabaseCSV<Provider>(dataset_dir + "/providers.csv"),
        new DatabaseCSV<Measurement>(dataset_dir + "/measurements.csv"),
        new DatabaseCSV<Sensor>(dataset_dir + "/sensors.csv"),
        None,
        unordered_map<string, ProviderFlag>()
    };
    return Ok(opened_session);
}