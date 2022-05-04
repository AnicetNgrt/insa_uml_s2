#include "Session.h"

#include "DatabaseCSV.h"

Result<Session, string> from_args(const char **argv, size_t argc)
{
	/*****************************
	 * -h Affiche ce manuel (pas important ici)
	 * -u Spécifie le nom d’utilisateur
	 * -p Spécifie le mot de passe
	 ******************************/
	string username = "";
	string password = "";

	for (int i = 0; i < argc; ++i)
	{
		string arg = argv[i];
		if (arg == "-u")
		{
			if (i+1 > argc || argv[i+1][0] == '-') return Err("username is missing");
			username = argv[++i];
		}
		else if (arg == "-p")
		{
			if (i+1 > argc || argv[i+1][0] == '-') return Err("password is missing");
			password = argv[++i];
		}
	}
	
	if(username.length() == 0)
		return Err("username is missing");
	
	if(password.length() == 0)
		return Err("password is missing");
	
	Session opened_session = {
		username,
		password,
		new DatabaseCSV<User>("dataset/users.csv"),
		new DatabaseCSV<Cleaner>("dataset/cleaners.csv"),
		new DatabaseCSV<Owner>("dataset/providers.csv"),
		new DatabaseCSV<Measurement>("dataset/measurements.csv"),
		new DatabaseCSV<Sensor>("dataset/sensors.csv"),
		None,
		unordered_map<string, OwnerFlag>()
	};
	return Ok(opened_session);
}