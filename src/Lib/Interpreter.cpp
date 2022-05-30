#include <sstream>
#include <unordered_map>
#include <chrono>

#include "Interpreter.h"
#include "AirQuality.h"

auto MANUAL = R"(
Utilisation générale: airwatcher [ options ... ]

options:
    -d    (optionnel) Spécifie le répertoire du jeu de données, par défaut ./dataset/

L’application affiche la liste des commandes disponibles pour l’utilisateur ainsi qu’un prompt pour laisser l’utilisateur entrer une commande.

Commandes disponibles pour tous:

Afficher l’aide spécifique au type d’utilisateur
    help

Se connecter avec un compte utilisateur
    login -u [nom d’utilisateur] -p [mot de passe]

Se déconnecter et fermer l’application
    exit

Afficher une liste de mesures en fonction de filtres 
    measurements -ts [horodatage (YYYY-MM-DD HH:MM:SS) (optionnel)] -t [type (optionnel)] -s [sensor ID]

Afficher la qualité de l’air moyen dans une zone donnée pendant une période donnée
    quality-area -begin [horodatage début (YYYY-MM-DD HH:MM:SS) (optionnel)] -end [horodatage fin (YYYY-MM-DD HH:MM:SS) (optionnel)] -lt [latitude] -lg [longitude] -rad [rayon]

(Dans une version future) Afficher la qualité de l’air à un point donné à un moment donné
    quality-point -t [horodatage (YYYY-MM-DD HH:MM:SS)] -cx [coordonnée X] -cy [coordonnée Y]

(Dans une version future) Afficher les capteurs les plus similaires à un capteur donné pendant une période donnée
    sensors-matching -s [sensor ID] -begin [horodatage début (YYYY-MM-DD HH:MM:SS) (optionnel)] -end [horodatage fin (YYYY-MM-DD HH:MM:SS) (optionnel)]

Vérifier si un owner est classifié comme “fiable” ou “peu fiable”
    owner-flag -o [owner ID]

Commandes disponibles pour les comptes gouvernementaux:

Classifier un owner comme “fiable” ou “peu fiable”
    flag-owner -o [owner ID] -flag [“reliable” | “unreliable”]

)";

template <typename T>
static Result<string, string> map_arg_error_to_message(Result<T, ArgError> erronous_result, string arg_name, string friendly_name);

Interpreter::Interpreter(IService& service)
    : service(service)
{
}

Result<string, string> Interpreter::interpret(string command) const
{
    Command c = Command(command);
    string name = c.get_name();

    if (name == "exit") {
        return cmd_exit();
    } else if (name == "help") {
        return cmd_help();
    } else if (name == "measurements") {
        return cmd_measurements(c);
    } else if (name == "quality-area") {
        return cmd_quality_area(c);
    } else if (name == "login") {
        return cmd_login(c);
    } else if (name == "flag-owner") {
        return cmd_flag_owner(c);
    } else if (name == "owner-flag") {
        return cmd_owner_flag(c);
    }

    return Err("No corresponding command was found");
}

Result<string, string> Interpreter::cmd_exit() const
{
    exit(EXIT_SUCCESS);
    return Ok("");
}

Result<string, string> Interpreter::cmd_help() const
{
    return Ok(MANUAL);
}

Result<string, string> Interpreter::cmd_login(Command& cmd) const
{
    auto username = cmd.find_arg("-u");
    auto password = cmd.find_arg("-p");
    if (failure(username))
        return map_arg_error_to_message(username, "-u", "Username");
    if (failure(password))
        return map_arg_error_to_message(password, "-p", "Password");

    auto t1 = chrono::high_resolution_clock::now();
    auto maybe_error = service.authenticate(UnwrapValue(username), UnwrapValue(password));
    auto t2 = chrono::high_resolution_clock::now();
    cout << "PERF: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms" << endl;

    if (some(maybe_error)) {
        switch (Unwrap(maybe_error)) {
        case AuthError::USER_NOT_FOUND:
            return Err("User " + UnwrapValue(username) + " could not be found");
        case AuthError::WRONG_PASSWORD:
            return Err("Incorrect password for user " + UnwrapValue(username));
        }
    }

    auto user = Unwrap(service.authenticated_user()); // Should not be None here. If it is, crashing is better.
    return Ok("Connected as " + user.get_username());
}

Result<string, string> Interpreter::cmd_measurements(Command& cmd) const
{
    auto id = cmd.find_arg("-s");
    if (failure(id) && !is_error(id, ArgError::ARG_NOT_FOUND))
        return map_arg_error_to_message(id, "-s", "Sensor id");

    auto time = cmd.find_timestamp("-ts");
    if (failure(time) && !is_error(time, ArgError::ARG_NOT_FOUND))
        return map_arg_error_to_message(time, "-ts", "Timestamp");

    auto type = cmd.find_measurement_type("-t");
    if (failure(type) && !is_error(type, ArgError::ARG_NOT_FOUND))
        return map_arg_error_to_message(type, "-t", "Measurement type");

    auto t1 = chrono::high_resolution_clock::now();
    auto measurements_stream = service.measurements(to_maybe(id), to_maybe(type), to_maybe(time));

    stringstream formatted;
    Maybe<Measurement> measurement = measurements_stream->receive();
    while (some(measurement)) {
        formatted << Unwrap(measurement).to_string() << endl;
        measurement = measurements_stream->receive();
    }

    delete measurements_stream;
    auto formatted_str = formatted.str();

    auto t2 = chrono::high_resolution_clock::now();
    cout << "PERF: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms" << endl;

    return Ok(formatted_str);
}

Result<string, string> Interpreter::cmd_quality_area(Command& cmd) const
{
    auto lt = cmd.find_double("-lt");
    if (failure(lt))
        return map_arg_error_to_message(lt, "-lt", "Latitude");
    auto lg = cmd.find_double("-lg");
    if (failure(lg))
        return map_arg_error_to_message(lt, "-lg", "Longitude");
    auto rad = cmd.find_double("-rad");
    if (failure(rad))
        return map_arg_error_to_message(lt, "-rad", "Radius");

    auto start = cmd.find_timestamp("-begin");
    if (failure(start) && !is_error(start, ArgError::ARG_NOT_FOUND))
        return map_arg_error_to_message(start, "-begin", "Beginning timestamp");
    auto end = cmd.find_timestamp("-end");
    if (failure(end) && !is_error(end, ArgError::ARG_NOT_FOUND))
        return map_arg_error_to_message(end, "-end", "End timestamp");

    auto t1 = chrono::high_resolution_clock::now();
    auto quality_area = service.air_quality_area(UnwrapValue(lt), UnwrapValue(lg), UnwrapValue(rad), to_maybe(start), to_maybe(end));
    auto t2 = chrono::high_resolution_clock::now();
    cout << "PERF: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms" << endl;

    auto quality_area_to_message = [&](AirQuality quality_area) {
        stringstream formatted;
        formatted << "Air quality at (" << UnwrapValue(lg) << "," << UnwrapValue(lt) << ") with radius " << UnwrapValue(rad) << " : " << air_quality_to_string(quality_area);
        return formatted.str();
    };

    if (success(quality_area)) {
        return map_success(quality_area, quality_area_to_message);
    } else {
        return err_from(quality_area);
    }
}

Result<string, string> Interpreter::cmd_flag_owner(Command& cmd) const
{
    auto id = cmd.find_arg("-o");
    if (failure(id))
        return map_arg_error_to_message(id, "-o", "Owner id");

    auto flag = cmd.find_owner_flag("-f");
    if (failure(id))
        return map_arg_error_to_message(flag, "-f", "Flag");
    
    auto t1 = chrono::high_resolution_clock::now();    
    auto maybe_error = service.flag_owner(UnwrapValue(id), UnwrapValue(flag));
    auto t2 = chrono::high_resolution_clock::now();
    cout << "PERF: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms" << endl;

    if (some(maybe_error)) {
        switch (Unwrap(maybe_error)) {
        case FlagError::OWNER_NOT_FOUND:
            return Err("Owner " + UnwrapValue(id) + " could not be found");
        case FlagError::PERMISSION_DENIED:
            return Err("Permission denied");
        }
    }

    return Ok("Owner " + UnwrapValue(id) + " was just flagged");
}

Result<string, string> Interpreter::cmd_owner_flag(Command& cmd) const
{
    auto id = cmd.find_arg("-o");
    if (failure(id))
        return map_arg_error_to_message(id, "-o", "Owner id");

    auto t1 = chrono::high_resolution_clock::now();
    auto flag = service.get_owner_flag(UnwrapValue(id));
    auto t2 = chrono::high_resolution_clock::now();
    cout << "PERF: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms" << endl;
    
    if (some(flag)) {
        switch (Unwrap(flag)) {
        case OwnerFlag::RELIABLE:
            return Ok("Owner " + UnwrapValue(id) + " is flagged as reliable");
        case OwnerFlag::UNRELIABLE:
            return Ok("Owner " + UnwrapValue(id) + " is flagged as unreliable");
        }
    } else {
        return Err("Owner " + UnwrapValue(id) + " could not be found");
    }
}

template <typename T>
static Result<string, string> map_arg_error_to_message(Result<T, ArgError> erronous_result, string arg_name, string friendly_name)
{
    return map_error(erronous_result, arg_error_to_string(arg_name, friendly_name));
}