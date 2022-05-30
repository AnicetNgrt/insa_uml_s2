#include <iostream>

using namespace std;

#include "Lib/Interpreter.h"
#include "Lib/Result.h"
#include "Lib/Service.h"

int main(int argc, const char* argv[])
{
    Result<Session, string> session_parsing_result = from_args(argv, argc);
    if (failure(session_parsing_result)) {
        cout << "ERROR: " << UnwrapError(session_parsing_result) << endl;
        exit(EXIT_FAILURE);
    }

    Session session = UnwrapValue(session_parsing_result);
    Service service = Service(session);

    Interpreter interpreter = Interpreter(service);

    string command;
    while (true) {
        cin >> command;
        Result<string, string> message = interpreter.interpret(command);
        if (failure(message)) {
            cout << "ERROR: " << UnwrapError(message) << endl;
        } else if (success(message)) {
            cout << UnwrapValue(message) << endl;
        }
    }
}