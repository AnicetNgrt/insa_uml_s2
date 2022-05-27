#include <unordered_map>
#include <sstream>

#include "Measurement.h"
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

Interpreter::Interpreter(IService& service) : service(service) {}

enum class ArgError {
    ARG_NOT_FOUND,
    VALUE_NOT_FOUND,
    VALUE_NOT_PARSABLE
};

function<string (ArgError)> map_arg_error_to_string(string arg, string friendly_name) {
    return function([=] (ArgError e) -> string {
        switch(e) {
            case ArgError::ARG_NOT_FOUND: return friendly_name + " (argument " + arg + ") was not found";
            case ArgError::VALUE_NOT_FOUND: return friendly_name + " (argument " + arg + ") had no value given";
            case ArgError::VALUE_NOT_PARSABLE: return friendly_name + " (argument " + arg + ") had an invalid value";
        }
    });
}

Result<string, ArgError> cmd_find_arg(vector<string> args, string name) {
    bool prev_is_arg;
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == name) {
            prev_is_arg = true;
        }
        if (prev_is_arg) {
            if (args[i][0] == '-' && !atoi(args[i].c_str())) {
                return Err(ArgError::VALUE_NOT_FOUND);
            } else {
                return Ok(args[i]);
            }
        }
    }
    return Err(ArgError::ARG_NOT_FOUND);
}

template<typename T>
Result<T, ArgError> cmd_find_arg(vector<string> args, string name, T (*conversion)(string)) {
    Result<string, ArgError> maybe_arg = cmd_find_arg(args, name);
    try {
        return map_success(maybe_arg, conversion);
    } catch (exception e) {
        return Err(ArgError::VALUE_NOT_PARSABLE);
    }
}

pair<string, vector<string>> cmd_name_and_args(string command) {
    vector<string> args();
    istringstream iss(command);
    string cname;
    getline(iss, cname, ' ' );
    if (iss.good()) {
        string s;
        while (getline( iss, s, ' ' )) {
            printf( "`%s'\n", s.c_str() );
        }
    }

    make_pair(cname, args);
}

Result<string, string> cmd_exit() {
    exit(EXIT_SUCCESS);
    return Ok("");
}

Result<string, string> cmd_help() {
    return Ok(MANUAL);
}

Result<string, string> cmd_reconnect(vector<string> args, IService& service) {
    auto username = cmd_find_arg(args, "-u");
    auto password = cmd_find_arg(args, "-p");
    if (failure(username)) return map_error(username, map_arg_error_to_string("-u", "Username"));
    if (failure(password)) return map_error(password, map_arg_error_to_string("-p", "Password"));

    auto maybe_error = service.authenticate(UnwrapValue(username), UnwrapValue(password));
    if (some(maybe_error)) return Err(Unwrap(maybe_error));

    auto user = Unwrap(service.authenticated_user()); // Should not be None here. If it is, crashing is better.
    return Ok("Connected as " + user.get_username());
}

Result<string, string> cmd_measurements(vector<string> args, IService& service) {
    auto id = cmd_find_arg(args, "-s");
    if (failure(id)) return map_error(id, map_arg_error_to_string("-s", "Sensor id"));

    auto time = cmd_find_arg(args, "-ts", &timestamp_from_string);
    if (is_error(time, ArgError::VALUE_NOT_PARSABLE)) {
        return map_error(time, map_arg_error_to_string("-ts", "Timestamp"));
    }
    auto type = cmd_find_arg(args, "-t", &measurement_type_from_string);
    if (is_error(type, ArgError::VALUE_NOT_PARSABLE)) {
        return map_error(type, map_arg_error_to_string("-t", "Measurement type"));
    }

    auto measurements = service.measurements(UnwrapValue(id), to_maybe(type), to_maybe(time));
    stringstream formatted;

    Maybe<Measurement> maybe_m = None;
    while(some((maybe_m = measurements->receive()))) {
        Measurement m = Unwrap(maybe_m);
        formatted << m.to_string();
    }
    return Ok(formatted.str());
}

Result<string, string> cmd_quality_area(vector<string> args, IService& service) {
    auto maybe_lt = cmd_find_arg_double(args, "-lt");
    if (none(maybe_lt)) return Err("Latitude missing");
    auto maybe_lg = cmd_find_arg_double(args, "-lg");
    if (none(maybe_lg)) return Err("Longitude missing");
    auto maybe_rad = cmd_find_arg_double(args, "-rad");
    if (none(maybe_rad)) return Err("Radius missing");

    auto maybe_start = cmd_find_arg_timestamp(args, "-begin");
    auto maybe_end = cmd_find_arg_timestamp(args, "-end");

    auto result = service.air_quality_area(Unwrap(maybe_lt), Unwrap(maybe_lg), Unwrap(maybe_rad), maybe_start, maybe_start);
    if (success(result)) {
        auto quality_area = UnwrapValue(result);
        stringstream formatted;
        formatted << "Qualité de l'air en (" << Unwrap(maybe_lg) << "," << Unwrap(maybe_lt) << ") dans un rayon de " << Unwrap(maybe_rad) << " : " << quality_area;
        return Ok(formatted.str());
    } else {
        return Err(UnwrapError(result));
    }
}

Result<string, string> cmd_flag_owner(vector<string> args, IService& service) {
    auto maybe_id = cmd_find_arg(args, "-u");
    if (none(maybe_id)) return Err("Owner id missing");

}

Result<string, string> Interpreter::interpret(string command) const {
    auto name_and_args = cmd_name_and_args(command);
    auto name = name_and_args.first;
    auto args = name_and_args.second;

    if (name == "exit") {
        cmd_exit();
        return Ok();
    }
    if (name == "help") {

    }
}