#include "Session.h"

Result<Session, string> from_args(const char **argv, size_t argc)
{
	/*****************************
	 * -h Affiche ce manuel (pas important ici)
	 * -u Spécifie le nom d’utilisateur
	 * -p Spécifie le mot de passe
	 ******************************/
	string username, password;
	for (int i = 0; i < argc; ++i)
	{
		string arg = argv[i];
		if (arg == "-u")
		{
			username = argv[++i];
		}
		else if (arg == "-p")
		{
			password = argv[++i];
		}
	}
	/*
	 Database<User> const &users_db;
  Database<Cleaner> const &cleaners_db;
  Database<Owner> const &owners_db;
  Database<Measurement> const &measurements_db;
  Database<Sensor> const &sensors_db;
	*/
	
	Session opened_session = {
		username,
		password,
		new DatabaseCSV<User>("dataset/users.csv"),
		new DatabaseCSV<Cleaner>("dataset/cleaners.csv"),
		new DatabaseCSV<Owner>("dataset/providers.csv"),
		new DatabaseCSV<Measurement>("dataset/measurements.csv"),
		new DatabaseCSV<Sensor>("dataset/sensors.csv"),
		None,
		
	};
	return Ok(opened_session);
}