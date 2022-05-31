#include <iostream>

using namespace std;

#include "Lib/Interpreter.h"
#include "Utils/Result.h"
#include "Lib/Service.h"

int main(int argc, const char* argv[])
{
    Command cmd = Command(argv, argc);
    Result<Session, string> session_parsing_result = from_command(cmd);
    if (failure(session_parsing_result)) {
        cout << "ERROR: " << UnwrapError(session_parsing_result) << endl;
        exit(EXIT_FAILURE);
    }
    Session session = UnwrapValue(session_parsing_result);
    
    Service service = Service(session);
    Interpreter interpreter = Interpreter(service);

    cout << "Type \"help\" to display user manual." << endl;

    string command;
    while (true) {
        getline(cin, command);
        Result<string, string> message = interpreter.interpret(command);
        if (failure(message)) {
            cout << "ERROR: " << UnwrapError(message) << endl;
        } else if (success(message)) {
            cout << UnwrapValue(message) << endl;
        }
    }
}