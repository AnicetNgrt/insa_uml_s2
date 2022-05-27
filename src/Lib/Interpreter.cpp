#include <sstream>
#include <unordered_map>

#include "Interpreter.h"

auto MANUAL = R"(
Utilisation générale: airwatcher [ options ... ]

options:
    -h    Affiche ce manuel
    -u     Spécifie le nom d’utilisateur
    -p    Spécifie le mot de passe


Dans le cas d’une connexion réussie, l’application affiche la liste des commandes disponibles pour l’utilisateur ainsi qu’un prompt pour laisser l’utilisateur entrer une commande.

Commandes disponibles pour tous:

Afficher l’aide spécifique au type d’utilisateur
    help

Se connecter avec un autre compte
    reconnect -u [nom d’utilisateur] -p [mot de passe]

Se déconnecter et fermer l’application
    exit

Afficher une liste de mesures en fonction de filtres 
    measurements -ts [horodatage (JJ:MM:AAAA::HH) (optionnel)] -t [type (optionnel)] -s [sensor ID]

Afficher la qualité de l’air moyen dans une zone donnée pendant une période donnée
    quality-area -begin [horodatage début (JJ:MM:AAAA:HH) (optionnel)] -end [horodatage fin (JJ:MM:AAAA:HH) (optionnel)] -lt [latitude] -lg [longitude] -rad [rayon]

Afficher la qualité de l’air à un point donné à un moment donné
    quality-point -t [horodatage (JJ:MM:AAAA:HH)] -cx [coordonnée X] -cy [coordonnée Y]

Afficher les capteurs les plus similaires à un capteur donné pendant une période donnée
    sensors-matching -s [sensor ID] -begin [horodatage début (JJ:MM:AAAA:HH) (optionnel)] -end [horodatage fin (JJ:MM:AAAA:HH) (optionnel)]

Commandes disponibles pour les comptes d’entreprises:

Afficher l’efficacité d’un cleaner ou de tous les cleaners d’un fournisseur
    cleaners-stats [-c [cleaner ID]] | [-f [nom fournisseur]]

Commandes disponibles pour les comptes gouvernementaux:

Classifier un utilisateur comme “fiable” ou “peu fiable”
    flag-user -u [user ID] -flag [“reliable” | “unreliable”]

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
        cmd_exit();
        return Ok();
    }
    if (name == "help") {
    }
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

Result<string, string> Interpreter::cmd_reconnect(Command& cmd) const
{
    auto username = cmd.find_arg("-u");
    auto password = cmd.find_arg("-p");
    if (failure(username))
        return map_arg_error_to_message(username, "-u", "Username");
    if (failure(password))
        return map_arg_error_to_message(password, "-p", "Password");

    auto maybe_error = service.authenticate(UnwrapValue(username), UnwrapValue(password));
    if (some(maybe_error))
        return Err(Unwrap(maybe_error));

    auto user = Unwrap(service.authenticated_user()); // Should not be None here. If it is, crashing is better.
    return Ok("Connected as " + user.get_username());
}

Result<string, string> Interpreter::cmd_measurements(Command& cmd) const
{
    auto id = cmd.find_arg("-s");
    if (failure(id))
        return map_arg_error_to_message(id, "-s", "Sensor id");

    auto time = cmd.find_timestamp("-ts");
    if (is_error(time, ArgError::VALUE_NOT_PARSABLE))
        return map_arg_error_to_message(time, "-ts", "Timestamp");
    auto type = cmd.find_measurement_type("-t");
    if (is_error(type, ArgError::VALUE_NOT_PARSABLE))
        return map_arg_error_to_message(type, "-t", "Measurement type");

    auto measurements = service.measurements(UnwrapValue(id), to_maybe(type), to_maybe(time));
    stringstream formatted;

    Maybe<Measurement> measurement = measurements->receive();
    while (some(measurement)) {
        formatted << Unwrap(measurement).to_string();
        measurement = measurements->receive();
    }
    return Ok(formatted.str());
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
    if (is_error(start, ArgError::VALUE_NOT_PARSABLE))
        return map_arg_error_to_message(start, "-begin", "Beginning timestamp");
    auto end = cmd.find_timestamp("-end");
    if (is_error(end, ArgError::VALUE_NOT_PARSABLE))
        return map_arg_error_to_message(end, "-end", "End timestamp");

    auto quality_area = service.air_quality_area(UnwrapValue(lt), UnwrapValue(lg), UnwrapValue(rad), to_maybe(start), to_maybe(end));

    auto quality_area_to_message = [&](double quality_area) -> string {
        stringstream formatted;
        formatted << "Qualité de l'air en (" << UnwrapValue(lg) << "," << UnwrapValue(lt) << ") dans un rayon de " << UnwrapValue(rad) << " : " << quality_area;
        return formatted.str();
    };

    return map_success(quality_area, quality_area_to_message);
}

Result<string, string> Interpreter::cmd_flag_owner(Command& cmd) const
{
    auto id = cmd.find_arg("-u");
}

template <typename T>
static Result<string, string> map_arg_error_to_message(Result<T, ArgError> erronous_result, string arg_name, string friendly_name)
{
    return map_error(erronous_result, arg_error_to_string(arg_name, friendly_name));
}