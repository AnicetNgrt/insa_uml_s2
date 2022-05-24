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

Maybe<string> cmd_find_arg_string(vector<string> args, string name) {
    bool prev_is_arg;
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == name) {
            prev_is_arg = true;
        }
        if (prev_is_arg) {
            if (args[i][0] == '-' && !atoi(args[i].c_str())) {
                return None;
            } else {
                return Some(args[i]);
            }
        }
    }
    return None;
}

Maybe<Timestamp> cmd_find_arg_timestamp(vector<string> args, string name) {
    auto maybe_arg = cmd_find_arg_string(args, name);
    if (none(maybe_arg)) return None;
    return Some(timestamp_from_string(Unwrap(maybe_arg)));
}

Maybe<MeasurementType> cmd_find_arg_measurement_type(vector<string> args, string name) {
    auto maybe_arg = cmd_find_arg_string(args, name);
    if (none(maybe_arg)) return None;
    try {
        return Some(measurement_type_from_string(Unwrap(maybe_arg)));
    } catch(exception _) {
        return None;
    }
}

Maybe<double> cmd_find_arg_double(vector<string> args, string name) {
    auto maybe_arg = cmd_find_arg_string(args, name);
    if (none(maybe_arg)) return None;
    try {
        return Some(atof(Unwrap(maybe_arg).c_str()));
    } catch(exception _) {
        return None;
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
    auto maybe_username = cmd_find_arg_string(args, "-u");
    auto maybe_password = cmd_find_arg_string(args, "-p");
    if (none(maybe_password)) return Err("Password missing");
    if (none(maybe_username)) return Err("Username missing");

    auto maybe_error = service.authenticate(Unwrap(maybe_username), Unwrap(maybe_password));
    if (some(maybe_error)) return Err(Unwrap(maybe_error));

    auto user = Unwrap(service.authenticated_user()); // Should not be None here. If it is, crashing is better.
    return Ok("Connected as " + user.get_username());
}

Result<string, string> cmd_measurements(vector<string> args, IService& service) {
    auto maybe_id = cmd_find_arg_string(args, "-s");
    if (none(maybe_id)) return Err("Sensor id missing");

    auto maybe_time = cmd_find_arg_timestamp(args, "-ts");
    auto maybe_type = cmd_find_arg_measurement_type(args, "-t");

    auto measurements = service.measurements(Unwrap(maybe_id), maybe_type, maybe_time);
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