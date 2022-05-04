#include "Session.h"

#include "DatabaseCSV.h"

Result<Session, string> from_args(const char **argv, size_t argc)
{
	/*****************************
	 * -h Affiche ce manuel (pas important ici)
	 * -u Spécifie le nom d’utilisateur
	 * -p Spécifie le mot de passe
	 * -d Spécifie le répertoire du dataset (./dataset par défaut)
	 ******************************/
	string username = "";
	string password = "";
	string dataset_dir = "./dataset";

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
		else if (arg == "-d")
		{
			if (i+1 > argc || argv[i+1][0] == '-') return Err("dataset directory path is missing");
			dataset_dir = argv[++i];
		}
	}
	
	if(username.length() == 0)
		return Err("username is missing");
	
	if(password.length() == 0)
		return Err("password is missing");
	
	Session opened_session = {
		username,
		password,
		new DatabaseCSV<User>(dataset_dir + "/users.csv"),
		new DatabaseCSV<Cleaner>(dataset_dir + "/cleaners.csv"),
		new DatabaseCSV<Owner>(dataset_dir + "/providers.csv"),
		new DatabaseCSV<Measurement>(dataset_dir + "/measurements.csv"),
		new DatabaseCSV<Sensor>(dataset_dir + "/sensors.csv"),
		None,
		unordered_map<string, OwnerFlag>()
	};
	return Ok(opened_session);
}